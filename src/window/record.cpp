
#include "record.h"

#include <cstddef>
#include <libavutil/error.h>
#include <string.h> // memcpy
#include <stdint.h>
#ifdef PARALLEL_FFMPEG
    #include <thread>
    static std::thread* ffmpeg_thread = NULL;
#endif


#include "VideoState.h"
#include "../util/logging.h"
extern "C" {
    #include "record_ffmpeg_log.h"
};


#include "../dependencies/glad.h"



static uint8_t* frameraw = 0;
static uint8_t* frameraw_mirror = 0;

static void pushFrame(uint8_t* data, const Config& config) {
    int err;
    if (!video_state.videoFrame) {
        video_state.videoFrame = av_frame_alloc();
        video_state.videoFrame->format = AV_PIX_FMT_YUV420P;
        video_state.videoFrame->width = video_state.cctx->width;
        video_state.videoFrame->height = video_state.cctx->height;
        if ((err = av_frame_get_buffer(video_state.videoFrame, 32)) < 0) {
            logging::error("record::pushFrame",  "Failed to allocate picture.");
            return;
        }
    }
    if (!video_state.swsCtx) {
        video_state.swsCtx = sws_getContext(video_state.cctx->width, video_state.cctx->height,
            AV_PIX_FMT_RGB24, video_state.cctx->width, // from rgb24, AV_PIX_FMT_YUV420P
            video_state.cctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, 0, 0, 0);
    }
    int inLinesize[1] = { 3 * video_state.cctx->width };
    // From RGB to YUV
    sws_scale(video_state.swsCtx, (const uint8_t* const*)&data, inLinesize, 0, video_state.cctx->height, 
        video_state.videoFrame->data, video_state.videoFrame->linesize);
    video_state.videoFrame->pts = (1.0 / config.fps) * 90000 * (video_state.frameCounter++);//90000
    //std::cout << video_state.videoFrame->pts << " " << video_state.cctx->time_base.num << " " << 
    //    video_state.cctx->time_base.den << " " << video_state.frameCounter << std::endl;
    if ((err = avcodec_send_frame(video_state.cctx, video_state.videoFrame)) < 0) {
        logging::error("record::pushFrame", "Failed to send frame.");
        return;
    }
    AV_TIME_BASE;
    AVPacket pkt;
    av_init_packet(&pkt); // av_packet_alloc(), av_packet_free(**).
    pkt.data = NULL;
    pkt.size = 0;
    pkt.flags |= AV_PKT_FLAG_KEY;
    if (avcodec_receive_packet(video_state.cctx, &pkt) == 0) {
        
        uint8_t* size = ((uint8_t*)pkt.data);
        av_interleaved_write_frame(video_state.ofctx, &pkt);
        av_packet_unref(&pkt);
        
        //av_frame_free(&video_state.videoFrame);
    }

}



static void free() {
    if (video_state.videoFrame) {
        av_frame_free(&video_state.videoFrame);
    }
    if (video_state.cctx) {
        avcodec_free_context(&video_state.cctx);
    }
    if (video_state.ofctx) {
        avformat_free_context(video_state.ofctx);
    }
    if (video_state.swsCtx) {
        sws_freeContext(video_state.swsCtx);
    }
    delete[] frameraw;
    delete[] frameraw_mirror;
}

void finish() {

#ifdef PARALLEL_FFMPEG
    if (ffmpeg_thread != NULL){ffmpeg_thread->join(); delete ffmpeg_thread;}
#endif

    //DELAYED FRAMES
    AVPacket pkt;
    av_init_packet(&pkt); // av_packet_alloc(), av_packet_free(**).
    pkt.data = NULL;
    pkt.size = 0;

    for (;;) {
        avcodec_send_frame(video_state.cctx, NULL);
        if (avcodec_receive_packet(video_state.cctx, &pkt) == 0) {
            av_interleaved_write_frame(video_state.ofctx, &pkt);
            av_packet_unref(&pkt);
        }
        else {
            break;
        }
    }

    av_write_trailer(video_state.ofctx);
    if (!(video_state.oformat->flags & AVFMT_NOFILE)) {
        int err = avio_close(video_state.ofctx->pb);
        if (err < 0) {
            logging::error("record::finish", "Failed to close file.");
        }
    }
    free();
}


int initialize(const Config& config)
{
   
    //av_register_all();
    //avcodec_register_all();
    logging::debug("record", "Firing up video codec.");
    const std::string filename = config.name + "." + config.video_format;

    ffmpeg_log_to_file();

    // retrieve the output format given the extension
    // the output format incorporates the name of the codec
    video_state.oformat = av_guess_format(nullptr, 
                filename.c_str(), 
                nullptr);
    if (!video_state.oformat)
    {
        logging::error("record::initiate", "Unknown extension " + config.video_format);
        return -1;
    }

    // video (mp4): ffmpeg will by default choose H264 which is more performant
    // but produces larger files. Overwrites this choice.
    // if the extension is webm and use_H265, it will fail later 
    if (config.use_H265)
    {
        video_state.oformat->video_codec = AV_CODEC_ID_H265;
        logging::info("record::initiate", "Enabling codec h265. Increases workload.");
    }
    

    // set up the required output context that holds all streams, etc.
    int err = avformat_alloc_output_context2(&video_state.ofctx, 
        video_state.oformat, 
        nullptr, 
        filename.c_str()
    );
    if (err)
    {
        logging::error("record::initiate", "Can't create output context.");
        return -1;
    }


    // Now we create a new stream, a video stream, and put 
    // it onto the output context. First, we need the corresponding
    // codec whose name we already have. Then we create the stream.
    AVCodec* codec = nullptr;
    codec = avcodec_find_encoder(video_state.oformat->video_codec);
    if (!codec)
    {
        logging::error("record::initiate", "Can't find codec.");
        return -1;
    }
    AVStream* stream = avformat_new_stream(video_state.ofctx, codec);
    if (!stream)
    {
        logging::error("record::initiate", "Can't create a new stream.");
        return -1;
    }


    // We now need a context for this stream.
    video_state.cctx = avcodec_alloc_context3(codec);
    if (!video_state.cctx)
    {
        logging::error("record::initiate", "Can't create codec context.");
        return -1;
    }

    stream->codecpar->codec_id = video_state.oformat->video_codec;
    stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    stream->codecpar->width = config.width;
    stream->codecpar->height = config.height;
    stream->codecpar->format = AV_PIX_FMT_YUV420P;
    stream->codecpar->bit_rate = video_state.bitrate * 1000;
    avcodec_parameters_to_context(video_state.cctx, stream->codecpar);
    video_state.cctx->time_base = AVRational{ 1, 1 };
    video_state.cctx->max_b_frames = 2;
    video_state.cctx->gop_size = 12;
    video_state.cctx->framerate = AVRational{ config.fps, 1 };


    // We can now take some time to save all custom flags
    // these will be forewarded to the codec once we are running.
    AVDictionary *param = 0;
    if (stream->codecpar->codec_id == AV_CODEC_ID_H264) {
        av_dict_set(&param, "preset", 
                config.preset.c_str(), 0);
    }
    else if (stream->codecpar->codec_id == AV_CODEC_ID_H265)
    {
        // x265 happens to be verbose even if ffmpeg is set to be quiet
        av_dict_set(&param, "x265-params", 
                "log-level=none", 0);
        av_dict_set(&param, "preset", 
                config.preset.c_str(), 0);
    }
    avcodec_parameters_from_context(stream->codecpar, 
                                    video_state.cctx);


    // start up the codec and pass on the arguments
    if ((err = avcodec_open2(video_state.cctx, codec, &param)) < 0) {
        logging::error("record::initiate", "Failed to open codec.");
        return -1;
    }

    if (!(video_state.oformat->flags & AVFMT_NOFILE)) {
        if ((err = avio_open(&video_state.ofctx->pb, filename.c_str(), AVIO_FLAG_WRITE)) < 0) {
            logging::error("record::initiate", "Failed to open file.");
            return -1;
        }
    }

    if ((err = avformat_write_header(video_state.ofctx, NULL)) < 0) {
        logging::error("record::initiate", 
            "Failed to write header. Did you enable h265 but request e.g. webm?");
        return -1;
    }

    av_dump_format(video_state.ofctx, 0, filename.c_str(), 1);
    frameraw = new uint8_t[config.width*config.height*3];
    frameraw_mirror = new uint8_t[config.width*config.height*3];

    return 0;
}


inline static void commit(const Config& config)
{
    const int ROWLENGTH = config.width * 3;
    int j = (config.height - 1) * ROWLENGTH;
    int k = 0;
    for (int i = 0; i < config.height; i++){
        memcpy(frameraw_mirror + j, frameraw + k, ROWLENGTH);
        j -= ROWLENGTH;
        k += ROWLENGTH;
    }

    pushFrame(frameraw_mirror, config);
}

void save_frame(const Config& config)
{
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0,
		config.width, config.height,
		GL_RGB, GL_UNSIGNED_BYTE, (void*)frameraw); // this is as expensive as saving the video frame

#ifdef PARALLEL_FFMPEG
    // we currently do not need a mutex as the main thread makes sure the other thread
    // is done before accessing frameraw, frameraw_mirror and only instanciates 
    // a thread when it itself is done handling this memory area.
    if (ffmpeg_thread != NULL){ffmpeg_thread->join(); delete ffmpeg_thread;}
    ffmpeg_thread = new std::thread{commit};
#else
    commit(config);
#endif
	
}

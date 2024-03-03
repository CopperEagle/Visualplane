#ifndef VIDEOSTATE_H
#define VIDEOSTATE_H

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavutil/time.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
}

static struct VideoState{
	const int bitrate = 2000;
	

	AVFrame* videoFrame = nullptr;
	AVCodecContext* cctx = nullptr;
	SwsContext* swsCtx = nullptr;
	int frameCounter = 0;
	AVFormatContext* ofctx = nullptr;
	AVOutputFormat* oformat = nullptr;
} video_state;


#endif // VIDEOSTATE_H
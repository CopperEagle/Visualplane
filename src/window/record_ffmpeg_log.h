#ifndef RECORD_FFMPEG_LOG_H
#define RECORD_FFMPEG_LOG_H


#include <stdarg.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>


#define ffmpeg_log_to_file()  av_log_set_callback(ffmpeg_logging_callback)
#define ffmpeg_no_log() av_log_set_level(AV_LOG_QUIET)


void ffmpeg_logging_callback(void* ptr, int level, const char* fmt, va_list vl);

#endif // RECORD_FFMPEG_LOG_H
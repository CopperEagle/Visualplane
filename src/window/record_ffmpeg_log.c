#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavutil/bprint.h>


static int av_log_level = AV_LOG_INFO;
static int opened_the_log = 0;
static int flags;


static const char *get_level_str(int level)
{
    switch (level) {
    case AV_LOG_QUIET:
        return "quiet";
    case AV_LOG_DEBUG:
        return "debug";
    case AV_LOG_TRACE:
        return "trace";
    case AV_LOG_VERBOSE:
        return "verbose";
    case AV_LOG_INFO:
        return "info";
    case AV_LOG_WARNING:
        return "warning";
    case AV_LOG_ERROR:
        return "error";
    case AV_LOG_FATAL:
        return "fatal";
    case AV_LOG_PANIC:
        return "panic";
    default:
        return "";
    }
}

static int get_category(void *ptr){
    AVClass *avc = *(AVClass **) ptr;
    if(    !avc
        || (avc->version&0xFF)<100
        ||  avc->version < (51 << 16 | 59 << 8)
        ||  avc->category >= AV_CLASS_CATEGORY_NB) return AV_CLASS_CATEGORY_NA + 16;

    if(avc->get_category)
        return avc->get_category(ptr) + 16;

    return avc->category + 16;
}

static void format_line(void *avcl, int level, const char *fmt, va_list vl,
                        AVBPrint part[4], int *print_prefix, int type[2])
{
    AVClass* avc = avcl ? *(AVClass **) avcl : NULL;
    av_bprint_init(part+0, 0, AV_BPRINT_SIZE_AUTOMATIC);
    av_bprint_init(part+1, 0, AV_BPRINT_SIZE_AUTOMATIC);
    av_bprint_init(part+2, 0, AV_BPRINT_SIZE_AUTOMATIC);
    av_bprint_init(part+3, 0, 65536);

    if(type) type[0] = type[1] = AV_CLASS_CATEGORY_NA + 16;
    if (*print_prefix && avc) {
        if (avc->parent_log_context_offset) {
            AVClass** parent = *(AVClass ***) (((uint8_t *) avcl) +
                                   avc->parent_log_context_offset);
            if (parent && *parent) {
                av_bprintf(part+0, "[%s @ %p] ",
                         (*parent)->item_name(parent), (void*)parent);
                if(type) type[0] = get_category(parent);
            }
        }
        av_bprintf(part+1, "[%s @ %p] ",
                 avc->item_name(avcl), avcl);
        if(type) type[1] = get_category(avcl);
    }

    if (*print_prefix && (level > AV_LOG_QUIET) && (flags & AV_LOG_PRINT_LEVEL))
        av_bprintf(part+2, "[%s] ", get_level_str(level));

    av_vbprintf(part+3, fmt, vl);

    if(*part[0].str || *part[1].str || *part[2].str || *part[3].str) {
        char lastc = part[3].len && part[3].len <= part[3].size ? part[3].str[part[3].len - 1] : 0;
        *print_prefix = lastc == '\n' || lastc == '\r';
    }
}



static void sanitize(char *line){
    while(*line){
        if(*line < 0x08 || (*line > 0x0D && *line < 0x20))
            *line='?';
        line++;
    }
}


/*
static int err_report(char* msg, ...)
{
    printf(msg);
    return 1;
}
static int redirect_fprint_to_file(char *file)
{
    // Connect standard output to given file 
    fflush(stdout);

    int fd1;
    if (opened_the_log) fd1 = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    opened_the_log = 1;

    if (fd1 < 0)
        return  err_report("Failed to open %s for writing\n", file);
    int fd2 = dup(STDOUT_FILENO);
    if (fd2 < 0)
        return err_report("Failed to duplicate standard output\n");
    if (dup2(fd1, STDOUT_FILENO) < 0)
        return err_report("Failed to duplicate %s to standard output\n", file);
    close(fd1);
    return fd2;
}

static int closeit(int fd2)
{
    //putchar('\n');

    // Reconnect original standard output 
    fflush(stdout);
    if (dup2(fd2, STDOUT_FILENO) < 0)
        return err_report("Failed to reinstate standard output\n");
    close(fd2);
    return 0;
}
*/

void ffmpeg_logging_callback(void* ptr, int level, const char* fmt, va_list vl)
{
    //int fd1 = redirect_fprint_to_file("the_av_log.txt");
    static int print_prefix = 1;
    AVBPrint part[4];
    int type[2];
    
    FILE* fd2;
    if (opened_the_log) 
        fd2 = fopen("the_av_log.txt", "a");
    else 
        fd2 = fopen("the_av_log.txt", "w");
    opened_the_log = 1;

    if (level >= 0) {
        level &= 0xff;
    }
    if (level > av_log_level){
        fclose(fd2);
        //closeit(fd1);
        return;
    }

    format_line(ptr, level, fmt, vl, part, &print_prefix, type);
    
    sanitize(part[0].str);
    fprintf(fd2, "%s", part[0].str);
    sanitize(part[1].str);
    fprintf(fd2, "%s", part[1].str);
    sanitize(part[2].str);
    fprintf(fd2, "%s", part[2].str);
    sanitize(part[3].str);
    fprintf(fd2, "%s", part[3].str);
    av_bprint_finalize(part+3, NULL);
    fclose(fd2);
    //closeit(fd1);
    return;
}



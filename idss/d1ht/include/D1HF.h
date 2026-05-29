/**
 * @file D1HF.h
 * @brief 
 */

#ifndef D1HF_H_
#define D1HF_H_

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef D1HF_PRELOAD
    #define open	D1HTfile_open
    #define read	D1HTfile_read
    #define close	D1HTfile_close
    #define fstat	fstat
#endif

    extern void D1HF_stats_start();
    extern void D1HF_stats_stop();

    extern int D1HTfile_open(const char *name, int flags, ...);
    extern int D1HTfile_close(const int fd);
    extern ssize_t D1HTfile_read(const int fd, void *buf, const size_t count);

#ifdef D1HF_PRELOAD
    extern int D1HTfile_fstat64(int ver, int filedes, struct stat64 *buf);
#else
    extern int D1HTfile_fstat64(int filedes, struct stat *buf);
#endif

#ifdef __cplusplus
    }
#endif

#endif /*D1HF_H_*/

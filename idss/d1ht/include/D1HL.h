/**
 * @file D1HL.h
 * @brief 
 */

#ifndef D1HL_H_
#define D1HL_H_

#include <limits.h>

#ifdef __cplusplus
    extern "C" {
#endif

    extern int      D1HTfile_open(const char *name, const int flags);
    extern ssize_t  D1HTfile_read(const int fd, void *buf, const size_t count);
    extern int      D1HTfile_close(const int fd);

#ifdef __cplusplus
    }
#endif

#endif /*D1HL_H_*/ 
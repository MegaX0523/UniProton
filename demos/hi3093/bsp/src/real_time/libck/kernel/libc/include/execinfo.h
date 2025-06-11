#ifndef _EXECINFO_H
#define _EXECINFO_H

#ifdef __cplusplus
extern "C" {
#endif

extern int backtrace(void **buffer, int size);
extern char **backtrace_symbols(void *const *array, int size);
extern void backtrace_symbols_fd(void *const *array, int size, int fd);

#ifdef __cplusplus
}
#endif

#endif /* execinfo.h  */

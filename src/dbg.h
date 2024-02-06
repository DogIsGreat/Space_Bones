#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

//FILE *log_file = NULL;

//#define OPEN_LOG_FILE() (log_file = fopen("debug.log", "a"))

//#define CLOSE_LOG_FILE() do { if (log_file) fclose(log_file); } while (0)

/*
#define log_to_file(M, ...) do { \
        if (log_file) { \
        fprintf(log_file, M "\n", ##__VA_ARGS__); \
        fflush(log_file);\
        } \
} while (0)
*/
int init_logging(const char *log_filename);

void log_messages(const char *format, ...);

void close_logging(void);

#ifdef NDEBUG
#define debug(M, ...)
#else
/*
#define debug(M, ...) do { \
        fprintf(stderr, "DEBUG %s:%d: " M "\n",  __FILE__, __LINE__, ##__VA_ARGS__); \
        log_to_file("DEBUG %sa:%d: " M, __FILE__, __LINE__, ##__VA_ARGS__); \
} while (0)
*/
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) do { \
        log_messages("[ERROR] (%s:%d errno: %s) " M "\n",__FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); \
} while (0)

#define log_warn(M, ...) do { \
        log_messages("[WARN] (%s:%d errno: %s) " M "\n",__FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); \
} while (0)

#define log_info(M, ...) do { \
        log_messages("[INFO] (%s:%d) " M "\n",__FILE__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define log_value(M, ...) do { \
        log_messages("[VALUE] (%s:%d) " M "\n",__FILE__, __LINE__, ##__VA_ARGS__); \
} while (0)


#define check(A, M, ...) if(!(A)) {\
    log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__);\
    errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__);\
    errno=0; goto error; }

#endif
#ifndef _LOG_H_
#define _LOG_H_

extern void open_log_file(void);
extern void close_log_file(void);
extern void Log(const char *fmt, ...);

#endif

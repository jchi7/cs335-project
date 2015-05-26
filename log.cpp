//#include "defs.h"
#include <stdio.h>
#include <stdarg.h>

FILE *fpxx;

void open_log_file(void)
{
	fpxx = fopen("x.x","w");
}

void close_log_file(void)
{
	fclose(fpxx);
}

void Log(const char *fmt, ...)
{
	va_list ap;

	if (fmt == NULL) return;
	va_start(ap, fmt);
	vfprintf(fpxx, fmt, ap);
	fflush(fpxx);
	va_end(ap);
}

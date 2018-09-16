#include "CLog.h"
namespace CBase
{

void CLog::V(const char *tag, const char* format, ...)
{
	char buf[LOG_BUF_SIZE];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, LOG_BUF_SIZE, format, args);
	va_end(args);

	full_printf(VERBOSE_PRI, LOG_ID, tag, buf);
}

void CLog::D(const char *tag, const char* format, ...)
{
	char buf[LOG_BUF_SIZE];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, LOG_BUF_SIZE, format, args);
	va_end(args);

	full_printf(DEBUG_PRI, LOG_ID, tag, buf);
}

void CLog::I(const char *tag, const char* format, ...)
{
	char buf[LOG_BUF_SIZE];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, LOG_BUF_SIZE, format, args);
	va_end(args);

	full_printf(INFO_PRI, LOG_ID, tag, buf);
}

void CLog::W(const char *tag, const char* format, ...)
{
	char buf[LOG_BUF_SIZE];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, LOG_BUF_SIZE, format, args);
	va_end(args);

	full_printf(WORNING_PRI, LOG_ID, tag, buf);
}

void CLog::E(const char *tag, const char* format, ...)
{
	char buf[LOG_BUF_SIZE];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, LOG_BUF_SIZE, format, args);
	va_end(args);

	full_printf(ERROR_PRI, LOG_ID, tag, buf);
}

void CLog::full_printf(int priority, int log_id, const char *tag, const char *buf)
{
	printf("%c/\t%s\t:\t%s\n", priority_to_char[priority], tag, buf);
}

}
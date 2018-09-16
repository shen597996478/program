#ifndef __CLog_H__
#define __CLog_H__

#include "CLog_ID.h"
#include "stdarg.h"
#include <sstream>

namespace CBase
{

#ifndef LOG_ID
#define LOG_ID (DEFAULT)
#endif

#define LOG_BUF_SIZE 1024

// log priority
const int VERBOSE_PRI = 0;
const int DEBUG_PRI = 1;
const int INFO_PRI = 2;
const int WORNING_PRI = 3;
const int ERROR_PRI = 4;

const char priority_to_char[] = {'V','D','I','W','E'};

class CLog
{

public:

	static void V(const char *tag, const char* format, ...);
	static void D(const char *tag, const char* format, ...);
	static void I(const char *tag, const char* format, ...);
	static void W(const char *tag, const char* format, ...);
	static void E(const char *tag, const char* format, ...);

private:
	static void full_printf(int priority, int log_id, const char *tag, const char *buf);
};

}

#endif
#ifndef __POSIXPipe_H__
#define __POSIXPipe_H__

#include "CLog.h"
#include <unistd.h>
#include <stdio.h>

namespace CBase {

class POSIXPipe
{
public:
	POSIXPipe();
	virtual ~POSIXPipe();

	void initialize();
	int getReadFd();
	int getWriteFd();

	void closeReadFd();
	void closeWriteFd();

	void destory();

private:
	int fd[2];

	POSIXPipe(const POSIXPipe &);
	POSIXPipe &operator=(const POSIXPipe &);
};

}
#endif
#include "POSIXPipe.h"

namespace CBase
{
POSIXPipe::POSIXPipe()
{
	fd[0] = -1;
	fd[1] = -1;
}

POSIXPipe::~POSIXPipe()
{
	destory();
}

void POSIXPipe::initialize()
{
	pipe(fd);
}

int POSIXPipe::getReadFd()
{
	return fd[0];
}

int POSIXPipe::getWriteFd()
{
	return fd[1];
}

void POSIXPipe::closeReadFd()
{
	if(fd[0] != -1) {
		close(fd[0]);
		fd[0] = -1;
	}
}

void POSIXPipe::closeWriteFd()
{
	if(fd[1] != -1) {
		close(fd[1]);
		fd[1] = -1;
	}
}

void POSIXPipe::destory()
{
	if(fd[0] != -1) {
		close(fd[0]);
		fd[0] = -1;
	}

	if(fd[1] != -1) {
		close(fd[1]);
		fd[1] = -1;
	}
}

}
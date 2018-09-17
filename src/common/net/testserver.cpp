#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>

#include "CTcpConnection.h"
#include "CEpoll.h"
#include "NetCommon.h"
#include "NetDefine.h"
//#include "CTransConnection.h"
#include "IConnection.h"
#include "CTcpListener.h"
#include "CConnManager.h"
#include "CTransConnection.h"

long long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    printf("tv:%lld,%ld\n",(long long)tv.tv_sec*1000,tv.tv_usec/1000);
    return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}
using namespace CNet;
int main(int argc, char **args)
{
	__AUTO_NET_LOG__

	NET_LOGD("argc %d.",argc);

	if(args[1][0] == 's') {

		CConnManager *connhandler = new CConnManager(4096);
		connhandler->createThread();

		CTcpListener listener("127.0.0.1", 9999);
		listener.setConnManager(connhandler);
		listener.create(4096);
		connhandler->addConnection((IConnection *)&listener, EVENT_OR);

		sleep(99999);
	} else if (args[1][0] == 'c') {
		
		CTcpConnection tcpCli("127.0.0.1", 9999);

		tcpCli.open(SOCK_STREAM);

		int fd = tcpCli.getFd();

		int error;

		error = tcpCli.connect(error);
		NET_LOGD("file:%s connect finish(%d)",__FILE__, error);
		sleep(5);
		int count = 250;
		while(count --) {
			struct NetPkgHeader head;

			head.len = sizeof("new data.");		// body
			head.type = 1;		// control or data ... etc
			head.cmd = 2;
			head.ver = 1;
			head.res = 0;

			::send(fd, (void*)&head, sizeof(struct NetPkgHeader), 0);
			::send(fd, "new data.", sizeof("new data."), 0);
		}
		NET_LOGD("file:%s send finish(%d)",__FILE__, error);
		//sleep(5);
		::close(fd);
		NET_LOGD("file:%s close finish",__FILE__);
		sleep(3);

		NET_LOGD("file:%s main finish",__FILE__);
		
	}

	return 0;
}
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#include "CTcpConnection.h"
//#include "base/MacroDefine.h"
//#include "base/ErrorCode.h"


//using namespace CBase;
//using namespace NErrorCode;

namespace CNet {

CTcpConnection::CTcpConnection()
{
	m_isListening = false;
}

CTcpConnection::CTcpConnection(const char* ip, const int port)
: CSocket(ip, port)
{
	m_isListening = false;
}

CTcpConnection::~CTcpConnection()
{
	destroy();
}

int CTcpConnection::create(int listenNum)
{
	int rc = open(SOCK_STREAM);
	if (rc != Success)
	{
		return rc;
	}
	
	rc = setReuseAddr(1);
	if (rc != Success)
	{
		return rc;
	}
	
	rc = setNagle(1);
	if (rc != Success)
	{
		return rc;
	}
	
	rc = setNoBlocking();
	if (rc != Success)
	{
		return rc;
	}
	
	rc = bind();
	if (rc != Success)
	{
		return rc;
	}
	
	rc = listen(listenNum);
	m_isListening = (rc == Success);

	return rc;
}

void CTcpConnection::destroy()
{
	m_isListening = false;
	close();
}

bool CTcpConnection::isListening()
{
	return (m_fd > 0) && m_isListening;
}
	
int CTcpConnection::accept(int& fd, in_addr& peerIp, unsigned short& peerPort, int& errorCode)
{
	errorCode = Success;
	struct sockaddr_in peerAddr;
	socklen_t peerSize = sizeof(peerAddr);
	memset(&peerAddr, 0, peerSize);
	fd = ::accept(m_fd, (struct sockaddr*)&peerAddr, &peerSize);
	if (fd == -1)
	{
		errorCode = errno;
		if (errorCode != EAGAIN && errorCode != EINTR)
		{
		    NET_LOGE("accept connect server ip = %s, port = %d, error = %d, info = %s", m_ip, m_port, errno, strerror(errno));
		}
		return AcceptConnectFailed;
	}

    memcpy(&peerIp, &peerAddr.sin_addr, sizeof(peerIp));
	peerPort = ntohs(peerAddr.sin_port);

	return Success;
}



}
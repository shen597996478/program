#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#include "CUdpConnection.h"

namespace CNet {


CUdpConnection::CUdpConnection(const char* ip, const int port) : CSocket(ip, port)
{

}

CUdpConnection::~CUdpConnection()
{

}

int CUdpConnection::create(int listenNum)
{
	int rc = open(SOCK_DGRAM);  // udp连接socket
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

	return rc;
}

void CUdpConnection::destroy()
{
	close();
}

size_t CUdpConnection::recv(void *buf, size_t len)
{
	return ::recvfrom(m_fd, buf, len, 0, (struct sockaddr *)&m_addr, &m_addrLen);
}

size_t CUdpConnection::send(const void *buf, size_t len)
{
	return ::sendto(m_fd, buf, len, 0, (struct sockaddr *)&m_addr, m_addrLen);
}

}
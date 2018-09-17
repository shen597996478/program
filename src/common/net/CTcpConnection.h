#ifndef __CTcpConnection_H__
#define __CTcpConnection_H__

#include "CSocket.h"

namespace CNet {

class CTcpConnection : public CSocket
{
public:
	CTcpConnection();
	CTcpConnection(const char* ip, const int port);
	virtual ~CTcpConnection();

	int create(int listenNum);
	void destroy();
	bool isListening();

	int accept(int& fd, in_addr& peerIp, unsigned short& peerPort, int& errorCode);

	
private:
	bool 	m_isListening;

	CTcpConnection(const CTcpConnection&);
	CTcpConnection& operator =(const CTcpConnection&);
};

}

#endif
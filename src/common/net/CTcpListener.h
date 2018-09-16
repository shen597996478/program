#ifndef __CTcpListener_H__
#define __CTcpListener_H__

#include "CConnManager.h"
#include "IConnection.h"
#include "CTcpConnection.h"

namespace CNet
{

class CTcpListener : public IConnection
{

public:
	CTcpListener();
	CTcpListener(const char* ip, const int port);
	virtual ~CTcpListener();

	void initialize(CConnManager *conn, const char* ip, const int port);
	int setConnManager(CConnManager *conn);
	int setConnInfo(const char* ip, const int port);

	int create(int num);
	int destroy();

	virtual void onRecv();

private:
	CConnManager *m_connManager;
	CTcpConnection m_tcpSock;

	CTcpListener(const CTcpListener&);
	CTcpListener& operator =(const CTcpListener&);
};

}

#endif
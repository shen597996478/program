#include "CTcpListener.h"
#include "NetDefine.h"
#include "CTransConnection.h"
namespace CNet
{

CTcpListener::CTcpListener()
: IConnection(&m_tcpSock)
, m_connManager(NULL)
, m_tcpSock()
{

}

CTcpListener::CTcpListener(const char* ip, const int port)
: IConnection(&m_tcpSock)
, m_connManager(NULL)
, m_tcpSock()
{
	setConnInfo(ip, port);
}

CTcpListener::~CTcpListener()
{

}

void CTcpListener::initialize(CConnManager *conn, const char* ip, const int port)
{
	setConnManager(conn);
	setConnInfo(ip, port);
}
	

int CTcpListener::setConnManager(CConnManager *conn)
{
	m_connManager = conn;
	return Success;
}

int CTcpListener::setConnInfo(const char* ip, const int port)
{
	m_tcpSock.initialize(ip, port);
	return Success;
}

int CTcpListener::create(int num)
{
	m_tcpSock.create(num);
	return Success;
}

int CTcpListener::destroy()
{
	m_tcpSock.destroy();
	return Success;
}

void CTcpListener::onRecv()
{
	int cliFd;
	int error;
	in_addr peerIp;
	unsigned short peerPort;
	m_tcpSock.accept(cliFd, peerIp, peerPort, error);

	if(m_connManager == NULL) {
		NET_LOGE("%s: connectHandler is NULL, can't deal new connection.", __FILE__);
		return;
	}

	CTransConnection *transConn = (CTransConnection *)m_connManager->getFreeTransConn();
	transConn->initialize(peerIp, peerPort);
	transConn->create(cliFd);
	transConn->setConnManager(m_connManager);

	m_connManager->addConnection(transConn, EPOLLIN | EPOLLET );
}

}
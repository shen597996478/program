#ifndef __CTransConnection_H__
#define __CTransConnection_H__

#include "IConnection.h"
#include "CConnManager.h"
#include "CNetBuf.h"
#include "CNetPkgMsg.h"

namespace CNet {

enum ConnectStatus
{
	norMal_status = 0,
	closed_status,
	exception_status,
	sendClose_status
};

enum LogicStatus
{
	idle_status = 0,
	recvHead_status,
	dealHead_status,
	recvBody_status,
	recvFinish_status
};

//class CConnManager;

class CTransConnection : public IConnection
{

public:
	CTransConnection();
	CTransConnection(const struct in_addr& ipInAddr, unsigned short port);
	virtual ~CTransConnection();

	void initialize(struct in_addr& ipInAddr, unsigned short port);
	int setConnManager(CConnManager *connMgr);

	int create(int fd);
	int create();
	int destory();

	size_t send(const void* buf, int len);

	virtual void onRecv();
protected:
	size_t recv(void* buf, int len);

private:

	bool chkIsFullPkgHead();
	void dealPkgHead();
	bool chkIsFullPkgBody();
	void postPkg();

	int getHead(void *head, int headLen);
	unsigned int noParseBufSize();
	int moveIndex(CNetBuf* &buf, int &index, int dist);

private:
	CNetBuf* m_recvBuf;
	CNetBuf* m_curParseBuf;
	int m_curParseIndex;

	CNetPkgMsg* m_curPkg;

	unsigned char m_logicStatic;
	unsigned char m_connStatus;

	CConnManager *m_connManager;
	CSocket m_sock;

	CTransConnection(const CTransConnection&);
	CTransConnection& operator =(const CTransConnection&);
};

}

#endif
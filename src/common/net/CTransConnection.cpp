#include "CTransConnection.h"
#include <errno.h>

using namespace CBase;
namespace CNet {

CTransConnection::CTransConnection()
: IConnection(&m_sock)
, m_recvBuf(NULL)
, m_curParseBuf(NULL)
, m_curParseIndex(0)
, m_curPkg(NULL)
, m_logicStatic(idle_status)
, m_connStatus(closed_status)
, m_connManager(NULL)
, m_sock()
{

}

CTransConnection::CTransConnection(const struct in_addr& ipInAddr, unsigned short port)
: IConnection(&m_sock)
, m_recvBuf(NULL)
, m_curParseBuf(NULL)
, m_curParseIndex(0)
, m_curPkg(NULL)
, m_logicStatic(idle_status)
, m_connStatus(closed_status)
, m_connManager(NULL)
, m_sock()
{
	
}

CTransConnection::~CTransConnection()
{
	// TODO
}

void CTransConnection::initialize(struct in_addr& ipInAddr, unsigned short port)
{
	//TODO
	m_sock.initialize(CSocket::toIPStr(ipInAddr), port);
}

int CTransConnection::create(int fd)
{
	m_sock.setFd(fd);
	m_connStatus = norMal_status;
	int rc = m_sock.setNoBlocking();
	if (rc != Success)
	{
		return rc;
	}

	return rc;

}

int CTransConnection::create()
{
	int rc = m_sock.open(SOCK_STREAM);

	if (rc != Success)
	{
		return rc;
	}
	m_connStatus = norMal_status;

	rc = m_sock.setNoBlocking();
	if (rc != Success)
	{
		return rc;
	}

	return rc;
	
}

int CTransConnection::destory()
{
	return m_sock.close();
}

int CTransConnection::setConnManager(CConnManager *connMgr)
{
	m_connManager = connMgr;
	return 0;
}

void CTransConnection::onRecv()
{
	__AUTO_NET_LOG__
	CNetMemeryPoll* memPoll = m_connManager->getMemeryPoll();

	int size = 0;
	while(true) {
		if(m_recvBuf == NULL) {
			m_recvBuf = memPoll->getBuf();
		} else if (m_recvBuf->m_curIndex == m_recvBuf->m_size) {
			CNetBuf *nextRecvBuf = memPoll->getBuf();
			m_recvBuf->m_nextBuf = nextRecvBuf;
			nextRecvBuf->m_prevBuf = m_recvBuf;
			m_recvBuf = nextRecvBuf;
		}

		size = m_recvBuf->m_size - m_recvBuf->m_curIndex;
		int n = recv((uint8_t *)m_recvBuf->data + m_recvBuf->m_curIndex, size);

		NET_LOGD("recv [size:%d] [n:%d] [errno:%d] [m_recvBuf->m_size:%d] [m_recvBuf->m_curIndex:%d]", size, n, errno, m_recvBuf->m_size, m_recvBuf->m_curIndex);

		if(n < 0) {
			// read error or noblock
			break;
		} else if (n == 0) {
			// socket close
			m_connStatus = closed_status;
			break;

		} else {
			// test
			m_recvBuf->m_curIndex = m_recvBuf->m_curIndex + n;
			
			while(true) {
				if(m_logicStatic == idle_status || m_logicStatic == recvHead_status) {
					bool ret = chkIsFullPkgHead();
					if(ret) {
						m_logicStatic = dealHead_status;
					} else {
						break;
					}

				} else if (m_logicStatic == dealHead_status) {
					dealPkgHead();
					
					m_logicStatic = recvBody_status;
				} else if (m_logicStatic == recvBody_status) {
					bool ret = chkIsFullPkgBody();
					if(ret) {
						m_logicStatic = recvFinish_status;
					} else {
						break;
					}
				} else if (m_logicStatic == recvFinish_status) {

					m_logicStatic = idle_status;
					

					postPkg();

				} else {
					// ERROR
					break;
				}

			}
			
		}
		
	}

}


bool CTransConnection::chkIsFullPkgHead()
{
	__AUTO_NET_LOG__
	if(sizeof(struct NetPkgHeader) <= noParseBufSize()) {

		return true;
	}
	return false;
}

void CTransConnection::dealPkgHead()
{
	__AUTO_NET_LOG__

	if(m_curPkg == NULL) {
		CHandler *handler = m_connManager->getHandlerThread();
		m_curPkg = (CNetPkgMsg*)handler->getMessage();
		m_curPkg->setConnection(this);
	}

	char buf[8];
	void* pbuf = buf;
	getHead(pbuf, sizeof(struct NetPkgHeader));
	//NET_LOGD("%s: m_curParseIndex:[%d], m_curParseBuf->m_curIndex:[%d].", __FILE__, m_curParseIndex, m_curParseBuf->m_curIndex);
	struct NetPkgHeader *head = (struct NetPkgHeader *)pbuf;
	//NET_LOGD("%s: len:[%d], type:[%d], cmd:[%d], ver:[%d].", __FILE__, head->len, head->type, head->cmd, head->ver);

	m_curPkg->setHead(head);

	return;
}

bool CTransConnection::chkIsFullPkgBody()
{
	__AUTO_NET_LOG__
	unsigned int bodyLen = m_curPkg->getHead()->len;
	NET_LOGW("%s: noParseBufSize:[%d], body size:[%d].", __FILE__, noParseBufSize(), bodyLen);
	if(noParseBufSize() >= bodyLen) {
		
		return true;
	}
	return false;
}

void CTransConnection::postPkg()
{
	__AUTO_NET_LOG__

	CHandler *handler = m_connManager->getHandlerThread();
	unsigned int bodyLen = m_curPkg->getHead()->len;
	m_curPkg->setBodyData(m_curParseBuf, m_curParseIndex);
	handler->sendMessage(m_curPkg);
	moveIndex(m_curParseBuf, m_curParseIndex, bodyLen);

	if(m_curPkg != NULL) {
		m_curPkg = NULL;
	}
	return;
}

int CTransConnection::getHead(void *head, int headLen)
{
	__AUTO_NET_LOG__
	if(m_curParseIndex == m_curParseBuf->m_size) {
		m_curParseBuf = m_curParseBuf->m_nextBuf;
		m_curParseIndex = 0;
	}

	void* srcData = (uint8_t *)m_curParseBuf->data + m_curParseIndex;
	int size = m_curParseBuf->m_curIndex - m_curParseIndex;
	size = size > headLen? headLen : size;
	memcpy(head, srcData, size);

	m_curParseIndex += size;
	//NET_LOGD("%s: m_curParseIndex:[%d], size:[%d], headLen:[%d], remain:[%d].", __FILE__, m_curParseIndex, size, headLen, headLen - size);

	int remain = headLen - size;
	if(remain) {
		getHead((uint8_t *)head + size, remain);
	}

	return 0;
}

unsigned int CTransConnection::noParseBufSize()
{
	CNetBuf *buf;
	if(m_curParseBuf == NULL) {
		m_curParseBuf = m_recvBuf;
	}
	buf = m_curParseBuf;
	int noParseSize = 0;
	while(buf != NULL) {
		if(buf->m_curIndex != buf->m_size) {
			if(buf == m_curParseBuf) {
				noParseSize += (buf->m_curIndex - m_curParseIndex);
			} else {
				noParseSize += buf->m_curIndex;
			}
			
			break;
		} else {
			// this buffer already recieves fully.
			if(m_curParseBuf == buf) {
				noParseSize += (buf->m_size - m_curParseIndex);
			} else {
				noParseSize += buf->m_size;
			}
			buf = buf->m_nextBuf;
		}
	}

	return noParseSize;
}

int CTransConnection::moveIndex(CNetBuf* &buf, int &index, int dist)
{
	CNetBuf* preBuf = buf;
	int preIndex = index;
	while(buf != NULL) {
		if (buf->m_curIndex - index >= dist) {
			index += dist;
			dist = 0;
			break;
		} else {
			if(buf->m_size == buf->m_curIndex) {
				dist = dist - (buf->m_curIndex - index);
				index = 0;
				buf = buf->m_nextBuf;
			} else {
				// ERROR: data size less than dist
				break;
			}
		}
	}
	if(dist == 0) {
		// move success
		return 0;
	} else {
		// ERROR: recovery
		buf = preBuf;
		index = preIndex;
		NET_LOGW("%s: remain buf less than distance that you want to move.", __FILE__);
		return -1;
	}
}

size_t CTransConnection::recv(void* buf, int len)
{
	__AUTO_NET_LOG__
	int fd = m_pSock->getFd();
	return ::recv(fd, buf, len, 0);
}

size_t CTransConnection::send(const void* buf, int len)
{
	int fd = m_pSock->getFd();
	return ::send(fd, buf, len, 0);
}

}
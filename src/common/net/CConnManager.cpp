#include "CConnManager.h"
#include "CTransConnection.h"
#include "CNetHandler.h"
using namespace CBase;

namespace CNet {

CConnManager::CConnManager(int size)
: CNetEvent(size, "CConnManager")
{
	m_lastThreadIndex = MAX_HANDLER_THREAD - 1;
	memPoll = new CNetMemeryPoll(4096, 1024);
	for(int i = 0; i < MAX_HANDLER_THREAD; ++i) {
		m_handlerThread[i] = (CHandler*)new CNetHandler();
		m_handlerThread[i]->start();
	}
}

CConnManager::~CConnManager()
{
	if(memPoll != NULL) {
		delete memPoll;
	}
	for(int i = 0; i < MAX_HANDLER_THREAD; ++i) {
		if(m_handlerThread[i]) {
			m_handlerThread[i]->stop();
			delete m_handlerThread[i];
		}
	}
}

int CConnManager::addConnection(IConnection* conn, int opt)
{
	int fd = conn->m_pSock->getFd();
	return addEvent(fd, opt, conn);
}

int CConnManager::delConnection(IConnection* conn)
{
	int fd = conn->m_pSock->getFd();
	return delEvent(fd);
}

IConnection* CConnManager::getFreeTransConn()
{
	return new CTransConnection();
}

void CConnManager::freeTransConn(IConnection* conn)
{
	delete conn;
}

CHandler* CConnManager::getHandlerThread()
{
	int curThreadIndex = (m_lastThreadIndex + 1) % MAX_HANDLER_THREAD;
	return m_handlerThread[curThreadIndex];
}

CNetMemeryPoll* CConnManager::getMemeryPoll()
{
	return memPoll;
}

void CConnManager::depatch(CBase::CChainList *connList)
{
	NET_LOGD("%s:%s:%d", __FILE__, __func__, __LINE__);
	int size = connList->length();
	IConnection *conn = NULL;
	Node *curNode = connList->head;
	for(int i = 0 ; i < size ; ++i) {
		if(curNode == NULL) break;
		conn = (IConnection *)curNode->data;
		conn->onRecv();
		curNode = curNode->next;
	}
}

}
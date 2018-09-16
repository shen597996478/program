#ifndef __CConnManager_H__
#define __CConnManager_H__

#include "CNetEvent.h"
#include "base/CChain.h"
#include "base/CHandler.h"
#include "NetCommon.h"
#include "IConnection.h"
#include "CNetMemeryPoll.h"

namespace CNet {

#define MAX_HANDLER_THREAD (10)

class CConnManager : public CNetEvent
{
public:
	CConnManager(int size);
	virtual ~CConnManager();

	int addConnection(IConnection* conn, int opt);
	int delConnection(IConnection* conn);

	IConnection* getFreeTransConn();
	void freeTransConn(IConnection* conn);
	CBase::CHandler* getHandlerThread();

	CNetMemeryPoll* getMemeryPoll();
protected:
	virtual void depatch(CBase::CChainList *connList);

private:
	int m_lastThreadIndex;
	CBase::CHandler* m_handlerThread[MAX_HANDLER_THREAD];
	CNetMemeryPoll *memPoll;

	CConnManager(const CConnManager &);
	CConnManager &operator=(const CConnManager &);
};

}

#endif
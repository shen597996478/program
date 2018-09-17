#ifndef __CHandler_H__
#define __CHandler_H__

#include "CCommon.h"
#include "CMessage.h"
#include "CThread.h"
#include <queue>

namespace CBase
{

class CHandler : public CThread
{
public:
	CHandler(const char *name);
	CHandler();
	virtual ~CHandler();
	virtual CMessage* getMessage();
	virtual void freeMessage(CMessage* msg);
	bool sendMessage(CMessage* msg);
	void start();
	void stop();

protected:
	virtual void processMessage(CMessage *msg);

private:
	void onStart();
	virtual void onRun();
	void onStop();

private:
	bool 					m_isStop;
	bool 					m_isExit;
	std::queue<CMessage *> 	m_MessageQueue;

	CHandler(const CHandler &);
	CHandler &operator=(const CHandler &);

};

}

#endif
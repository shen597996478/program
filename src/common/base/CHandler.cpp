#include "CHandler.h"
#include "unistd.h"

namespace CBase
{

CHandler::CHandler(const char *name)
: CThread(name)
, m_isStop(true)
, m_isExit(false)
{

}

CHandler::CHandler()
: CThread("CHandler")
, m_isStop(true)
, m_isExit(false)
{

}

CHandler::~CHandler()
{
	if(!m_isStop) {
		onStop();
		int cnt = 5;
		while(cnt--) {
			if(!m_isExit) {
				// sleep 100ms
				usleep(100000);
			}
		}
	}
}

CMessage*
CHandler::getMessage()
{
	__AUTO_LOG__
	return new CMessage();
}

 void 
 CHandler::freeMessage(CMessage* msg)
 {
 	delete msg;
 }

bool
CHandler::sendMessage(CMessage* msg)
{
	__AUTO_LOG__
	if(!m_isStop) {
		m_MessageQueue.push(msg);
		postNotify();
		return true;
	} else {
		return false;
	}
}

void
CHandler::processMessage(CMessage *msg)
{
	__AUTO_LOG__
}

void
CHandler::onRun()
{
	__AUTO_LOG__
	while(isActive()) {
		waitNotify();
		
		while(!m_MessageQueue.empty()) {
			if(m_isStop) break;
			CMessage* msg = m_MessageQueue.front();
			processMessage(msg);
			delete msg;
			m_MessageQueue.pop();
		}

		if(m_isStop) break;
	}
	m_isExit = true;
}

void
CHandler::start()
{
	__AUTO_LOG__
	onStart();
}

void
CHandler::stop()
{
	__AUTO_LOG__
	onStop();
}

void
CHandler::onStart()
{
	__AUTO_LOG__
	m_isStop = false;
	createThread();
}

void
CHandler::onStop()
{
	__AUTO_LOG__
	m_isStop = true;
	stopThread();
	cancelThread();
}

}
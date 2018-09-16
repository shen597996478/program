#include "CNetHandler.h"
#include "CNetPkgMsg.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "NetCommon.h"

using namespace CBase;

namespace CNet
{

CNetHandler::CNetHandler(const char *name)
: CHandler(name)
{

}


CNetHandler::CNetHandler()
: CHandler("CNetHandler")
{

}

CNetHandler::~CNetHandler()
{

}

CMessage* CNetHandler::getMessage()
{
	return (CMessage*)new CNetPkgMsg();
}

void CNetHandler::freeMessage(CMessage* msg)
{
	delete msg;
}

void CNetHandler::processMessage(CMessage *msg)
{
	__AUTO_NET_LOG__
	CNetPkgMsg* pkgMsg = (CNetPkgMsg*) msg;
	// gettid();
	int bodyLen = pkgMsg->getHead()->len;
	NET_LOGD("===============bodyLen:%d================",bodyLen);
	
}

}
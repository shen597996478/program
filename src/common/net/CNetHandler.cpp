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
#if 0
	// test
	CNetPkgMsg* pkgMsg = (CNetPkgMsg*) msg;
	// gettid();
	int bodyLen = pkgMsg->getHead()->len;
	NET_LOGD("===============bodyLen:%d================",bodyLen);

	char* data = new char[bodyLen];
	int n = pkgMsg->readBodyData(data, bodyLen);
	NET_LOGD("===============body:%s======readLen:%d==========", data, n);
#endif
}

}
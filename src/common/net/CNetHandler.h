#ifndef __CNetHandler_H__
#define __CNetHandler_H__

#include "base/CHandler.h"
#include "base/CMessage.h"

namespace CNet
{

class CNetHandler : CBase::CHandler
{
public:
	CNetHandler(const char *name);
	CNetHandler();
	virtual ~CNetHandler();
	
	virtual CBase::CMessage* getMessage();
	virtual void freeMessage(CBase::CMessage* msg);

protected:
	virtual void processMessage(CBase::CMessage *msg);

private:
	CNetHandler(const CNetHandler &);
	CNetHandler &operator=(const CNetHandler &);
};

}



#endif
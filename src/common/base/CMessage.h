#ifndef __CMessage_H__
#define __CMessage_H__

#include "CCommon.h"
namespace CBase
{

class CMessage
{
public:
	CMessage();
	virtual ~CMessage();

private:
	CMessage(const CMessage &);
	CMessage &operator=(const CMessage &);
};

}

#endif
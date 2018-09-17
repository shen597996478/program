#ifndef __CNetMemeryPoll_H__
#define __CNetMemeryPoll_H__

#include "base/CChain.h"
#include "CNetBuf.h"

namespace CNet {

class CNetMemeryPoll
{
public:
	CNetMemeryPoll(int bufSize, int count);
	virtual ~CNetMemeryPoll();
	
	CNetBuf* getBuf();
	void freeBuf(CNetBuf* buf);

private:
	int createOrIncreaseMemeryPoll(int bufSize, int count);

private:
	int 				m_bufSize;
	int 				m_count;
	CBase::CChainList 	m_freeBuf;

	CNetMemeryPoll(const CNetMemeryPoll &);
	CNetMemeryPoll &operator=(const CNetMemeryPoll &);

};

}

#endif
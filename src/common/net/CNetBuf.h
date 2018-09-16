#ifndef __CNetBuf_H__
#define __CNetBuf_H__

#include "base/CChain.h"

namespace CNet {


class CNetBuf : public CBase::Node
{
public:
	CNetBuf(int size, void* memeryPool);
	virtual ~CNetBuf();

	int initialize();
	void dump();
public:
	CNetBuf *m_prevBuf;
	CNetBuf *m_nextBuf;
	void* m_memeryPool;
	//void* data;
	int m_size;
	int m_used;
	int m_curIndex;
	bool m_needFree;

	CNetBuf(const CNetBuf &);
	CNetBuf &operator=(const CNetBuf &);
};

}

#endif
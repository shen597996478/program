#include "CNetBuf.h"
#include "NetCommon.h"

using namespace CBase;

namespace CNet {

CNetBuf::CNetBuf(int size, void* memeryPool)
: Node()
, m_prevBuf(NULL)
, m_nextBuf(NULL)
, m_memeryPool(memeryPool)
, m_size(size)
, m_used(0)
, m_curIndex(0)
, m_needFree(false)
{
	data = malloc(size);
}

CNetBuf::~CNetBuf()
{
	if(data) {
		free(data);
		data = NULL;
	}
}

int CNetBuf::initialize()
{
	m_prevBuf = NULL;
	m_nextBuf = NULL;
	m_used = 0;
	m_curIndex = 0;
	m_needFree = false;

	return Success;
}

void CNetBuf::dump()
{
	NET_LOGD("%s--------------------------------------", __FILE__);
	NET_LOGD("data:%p", data);
	NET_LOGD("next:%p", next);
	NET_LOGD("m_prevBuf:%p", m_prevBuf);
	NET_LOGD("m_nextBuf:%p", m_nextBuf);
	NET_LOGD("m_used:%d", m_used);
	NET_LOGD("m_curIndex:%d", m_curIndex);
	NET_LOGD("m_needFree:%d", m_needFree);
	NET_LOGD("m_size:%d", m_size);
}

}
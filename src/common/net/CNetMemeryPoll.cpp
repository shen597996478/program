#include "CNetMemeryPoll.h"
#include "NetCommon.h"

using namespace CBase;
namespace CNet {

CNetMemeryPoll::CNetMemeryPoll(int bufSize, int count)
: m_bufSize(bufSize)
, m_count(count)
, m_freeBuf()
{
	createOrIncreaseMemeryPoll(m_bufSize, m_count);
}

CNetMemeryPoll::~CNetMemeryPoll()
{
	Node* freeNode = NULL;
	while( !m_freeBuf.isEmpty() ) {
		m_freeBuf.getElem(0, freeNode, true);
		if(freeNode != NULL) {
			delete freeNode;
		}
	}
}

CNetBuf* CNetMemeryPoll::getBuf()
{
	if(m_freeBuf.isEmpty()) {
		createOrIncreaseMemeryPoll(m_bufSize, m_count);
	}
	Node* freeNode = NULL;

	m_freeBuf.getElem(0, freeNode, false);
	m_freeBuf.removeNode(freeNode);

	if(freeNode != NULL) {
		((CNetBuf*)freeNode)->initialize();
	} else {
		NET_LOGE("%s:free buffer is NULL!", __FILE__);
	}
	
	return (CNetBuf*)freeNode;

}

void CNetMemeryPoll::freeBuf(CNetBuf* buf)
{
	m_freeBuf.addToEnd((Node*) buf);
}


int CNetMemeryPoll::createOrIncreaseMemeryPoll(int bufSize, int count)
{
	for(int i = 0; i < count; ++i) {
		m_freeBuf.addToEnd((Node*)new CNetBuf(bufSize, (void*)this));
	}

	return Success;
}

}
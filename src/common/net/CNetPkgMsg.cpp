#include "CNetPkgMsg.h"
#include "string.h"

namespace CNet
{

CNetPkgMsg::CNetPkgMsg()
: CMessage()
, m_remainLen(0)
, m_conn(NULL)
, m_bodyBuf(NULL)
, m_bodyReadIndex(0)
, m_head()
{

}

CNetPkgMsg::~CNetPkgMsg()
{
	CNetBuf* preBuf = NULL;
	if(m_bodyBuf != NULL) {
		preBuf = m_bodyBuf->m_prevBuf;
	}

	while(preBuf && preBuf->m_needFree == false) {
		preBuf->m_needFree = true;
		preBuf = preBuf->m_prevBuf;
	}
}

struct NetPkgHeader* CNetPkgMsg::getHead()
{
	return &m_head;
}


int CNetPkgMsg::readBodyData(void* data, int dataLen)
{
	if(m_remainLen == 0) {
		return 0;
	}

	if(dataLen > m_remainLen) {
		dataLen = m_remainLen;
	}

	if(m_bodyReadIndex == m_bodyBuf->m_size) {
		m_bodyBuf = m_bodyBuf->m_nextBuf;
		m_bodyReadIndex = 0;
	}

	void* srcData = (uint8_t *)m_bodyBuf->data + m_bodyReadIndex;
	int size = m_bodyBuf->m_curIndex - m_bodyReadIndex;
	size = size > dataLen? dataLen : size;
	if(size != 0) {
		memcpy(data, srcData, size);
	}
	m_bodyReadIndex += size;
	m_remainLen -= size;

	if(m_remainLen) {
		return size + readBodyData((uint8_t *)data + size, m_remainLen);
	} else {
		return size;
	}
}

int CNetPkgMsg::setConnection(IConnection* conn)
{
	m_conn = conn;
	return Success;
}

IConnection* CNetPkgMsg::getConnection()
{
	return m_conn;
}


int CNetPkgMsg::setBodyData(CNetBuf* bodyBuf, int index)
{
	m_bodyBuf = bodyBuf;
	m_bodyReadIndex = index;

	return Success;
}

int CNetPkgMsg::setHead(struct NetPkgHeader* head)
{
	
	m_head.len = head->len;
	m_head.type = head->type;
	m_head.cmd = head->cmd;
	m_head.ver = head->ver;
	m_head.res = head->res;

	m_remainLen = m_head.len;

	return Success;
}


}
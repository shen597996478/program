#include "CNetPkgMsg.h"

namespace CNet
{

CNetPkgMsg::CNetPkgMsg()
: CMessage()
,m_conn(NULL)
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
	return 0;
}

int CNetPkgMsg::setConnection(IConnection* conn)
{
	m_conn = conn;
	return 0;
}

IConnection* CNetPkgMsg::getConnection()
{
	return m_conn;
}


int CNetPkgMsg::setBodyData(CNetBuf* bodyBuf, int index)
{
	m_bodyBuf = bodyBuf;
	m_bodyReadIndex = index;

	return 0;
}

int CNetPkgMsg::setHead(struct NetPkgHeader* head)
{
	
	m_head.len = head->len;
	m_head.type = head->type;
	m_head.cmd = head->cmd;
	m_head.ver = head->ver;
	m_head.res = head->res;

	return 0;
}


}
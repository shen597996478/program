#ifndef __CNetPkgMsg_H__
#define __CNetPkgMsg_H__

#include "CNetBuf.h"
#include "IConnection.h"
#include "base/CMessage.h"

namespace CNet
{

#pragma pack(1)
struct NetPkgHeader
{
	uint32_t len;		// body
	uchar8_t type;		// control or data ... etc
	uchar8_t cmd;
	uchar8_t ver;
	uchar8_t res;		// now, not use
	
	NetPkgHeader() {}
	NetPkgHeader(uint32_t _len, uchar8_t _type, uchar8_t _cmd) : len(_len), type(_type), cmd(_cmd) {}
	~NetPkgHeader() {}
};
#pragma pack()

class CNetPkgMsg : public CBase::CMessage
{
public:
	CNetPkgMsg();
	virtual ~CNetPkgMsg();

	int setConnection(IConnection* conn);
	IConnection* getConnection();

	int setBodyData(CNetBuf* bodyBuf, int index);
	int readBodyData(void* data, int dataLen);

	int setHead(struct NetPkgHeader* head);
	struct NetPkgHeader* getHead();

private:
	IConnection* m_conn;
	CNetBuf* m_bodyBuf;
	int m_bodyReadIndex;
	struct NetPkgHeader m_head;

	CNetPkgMsg(const CNetPkgMsg &);
	CNetPkgMsg &operator=(const CNetPkgMsg &);
};

}

#endif
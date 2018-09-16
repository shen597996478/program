#ifndef __IConnection_H__
#define __IConnection_H__

#include "CSocket.h"

namespace CNet {

class IConnection
{
public:
	IConnection() {}
	IConnection(CSocket* socket) {m_pSock = socket;}
	virtual ~IConnection() {};
	void set(CSocket* socket) {m_pSock = socket;}

	virtual void onRecv() = 0;

public:
	CSocket* m_pSock;
	IConnection(const IConnection&);
	IConnection& operator =(const IConnection&);
};

}

#endif
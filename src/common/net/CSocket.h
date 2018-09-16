#ifndef __CSocket_H__
#define __CSocket_H__

#include <netinet/in.h>
#include "NetCommon.h"

namespace CNet {

class CSocket
{
public:
	CSocket();
	CSocket(const char* ip, unsigned short port);
	CSocket(const struct in_addr& ipInAddr, unsigned short port);
	virtual ~CSocket();
//----------------------------------------------------------
	static const char* toIPStr(const struct in_addr& ip);
	static unsigned int toIPInt(const char* ip);
	static int toIPInAddr(const char* ipStr, struct in_addr& ipInAddr);
	static int getOpt(int sockfd, int level, int optname, void* optval, socklen_t* optlen);
//----------------------------------------------------------
	int initialize(const char* ip, unsigned short port);
	void finalize();
//----------------------------------------------------------
	int open(int type); // define ipv4
	int open(int family, int type, int protocol);
	int close();
//----------------------------------------------------------
	int bind();
	int listen(int num);
//----------------------------------------------------------
	int connect(int& errorCode);
//----------------------------------------------------------
	int setLinger(int onOff, int val);
	int setRcvBuffSize(int val);
	int setSndBuffSize(int val);
	int setNagle(int val);
	int setReuseAddr(int val);
	int setNoBlocking();
	int setFdControl(int flag);
//----------------------------------------------------------
	int setFd(int fd);
	int getFd() const;
	const char* getIp() const;
	unsigned short getPort() const;

protected:
	int m_fd;
	// MAX 2^16 -1
	unsigned short m_port;
	strIP_t m_ip;
	struct sockaddr_in m_addr;
	unsigned int m_addrLen;

private:
	CSocket(const CSocket&);
	CSocket& operator =(const CSocket&);
};

}

#endif
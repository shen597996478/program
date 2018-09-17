#ifndef __NetDefine_H__
#define __NetDefine_H__

namespace CNet {

typedef unsigned char uchar8_t;

static const int StrIPLen = 32;
typedef char strIP_t[StrIPLen];

#define EVENT_OR (EPOLLIN | EPOLLET)
#define EVENT_OW (EPOLLOUT | EPOLLET)
#define EVENT_RW (EPOLLIN | EPOLLOUT | EPOLLET)

#define	MAX_EVENT	1024

#define MAX_HANDLER_THREAD (10)

}

#endif
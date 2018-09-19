#ifndef __DBErrorCode_H__
#define __DBErrorCode_H__


namespace ERedis
{
enum Error
{
	NormalError = -101,		          // redis常规错误，说明用的不对
	NetError = -102,			      // 网络连接错误
	SpaceNotEnough = -103,	          // 存储空间不够
};

}

#endif
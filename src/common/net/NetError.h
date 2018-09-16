#ifndef __NetError_H__
#define __NetError_H__

namespace CNet {

enum ECommon
{
	Success = 0,                      // 调用成功
	InvalidParam = 1,                 // 无效参数
	NoMemory = 2,                     // new 失败，没内存了
	SystemCallErr = 3,                // 系统调用失败
};

enum EConnect
{
	CreateSocketFailed = 121,         // 创建socket失败
	CloseSocketFailed = 122,          // 关闭socket失败
	InValidIp = 123,                  // 无效ip地址
	BindIpPortFailed = 124,           // 绑定ip、端口失败
	ListenConnectFailed = 125,        // 启动监听连接失败
	ConnectIpPortFailed = 126,        // 连接失败
	SetLingerFailed = 127,            // 选项设置失败
	SetRcvBuffFailed = 128,           // 选项设置失败 
	SetSndBuffFailed = 129,           // 选项设置失败
	SetReuseAddrFailed = 130,         // 选项设置失败
	SetNagleFailed = 131,             // 选项设置失败
	SetNonBlockFailed = 132,          // 选项设置失败
	AcceptConnectFailed = 133,        // 建立连接失败
};

// 连接管理服务 错误码范围[151---170]
enum EConnServer
{
	CreateEPollFailed = 151,          // 创建epoll模型失败
	AddEPollListenerFailed = 152,     // 增加监听者失败
	RemoveEPollListenerFailed = 153,  // 删除监听者失败
	ModifyEPollListenerFailed = 154,  // 修改监听事件失败

};

}

#endif
#ifndef __NetCommon_H__
#define __NetCommon_H__

#include "./base/CCommon.h"
#include "./base/CLog.h"
#include "NetError.h"
#include "NetDefine.h"

class CLog;

#define NET_LOGV(format, args...) CBase::CLog::V("net", format, args)

#define NET_LOGD(format, args...) CBase::CLog::D("net", format, args)

#define NET_LOGI(format, args...) CBase::CLog::I("net", format, args)

#define NET_LOGW(format, args...) CBase::CLog::W("net", format, args)

#define NET_LOGE(format, args...) CBase::CLog::E("net", format, args)



#ifdef DEBUG
#define __AUTO_NET_LOG__ NET_LOGD("%s:%s enter()",__FILE__, __func__);
#else
#define __AUTO_NET_LOG__
#endif

#endif
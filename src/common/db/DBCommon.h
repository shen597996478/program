#ifndef __DBCommon_H__
#define __DBCommon_H__


#include "base/CCommon.h"
#include "DBErrorCode.h"


class CLog;

#define DB_LOGV(format, args...) CBase::CLog::V("db", format, args)

#define DB_LOGD(format, args...) CBase::CLog::D("db", format, args)

#define DB_LOGI(format, args...) CBase::CLog::I("db", format, args)

#define DB_LOGW(format, args...) CBase::CLog::W("db", format, args)

#define DB_LOGE(format, args...) CBase::CLog::E("db", format, args)


#endif

#ifndef __CCommon_H__
#define __CCommon_H__

#include <iostream>
#include <sstream>
#include "CLog.h"
//#define DEBUG

#define INVALID_INT (0xffffffff)

#define	EQUAL	(0)
#define	GREATER	(1)
#define	LESS	(-1)

#ifndef LOGV
#define LOGV CBase::CLog::V
#endif

#ifndef LOGD
#define LOGD CBase::CLog::D
#endif

#ifndef LOGI
#define LOGI CBase::CLog::I
#endif

#ifndef LOGW
#define LOGW CBase::CLog::W
#endif

#ifndef LOGE
#define LOGE CBase::CLog::E
#endif

#ifndef LOG_TAG
#define LOG_TAG "CCommon"
#endif

#ifndef AUTO_LOG_TAG
#define AUTO_LOG_TAG "auto"
#endif

#ifdef DEBUG
#define __AUTO_LOG__ LOGD(AUTO_LOG_TAG, "%s:%s enter()",__FILE__, __func__);
#else
#define __AUTO_LOG__
#endif

#endif
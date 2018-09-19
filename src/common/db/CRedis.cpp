#include <sys/time.h>
#include <math.h>
#include <queue>
#include <unistd.h>
#include "CRedis.h"
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "DBCommon.h"

using namespace CBase;

#define REDIS_NORMAL_JUDGE(reply) 					\
if (NULL == reply)									\
{													\
	return ERedis::NetError;						\
}													\
if (REDIS_REPLY_ERROR == reply->type)				\
{													\
	freeReplyObject(reply);							\
	return ERedis::NormalError;						\
}													\


#ifdef USE_EXTERN_HIREDIS_FUNC

#ifdef __cplusplus
extern "C" {
#endif
//It already used in hiredis source code.
int __redisAppendCommand(redisContext *c, const char *cmd, size_t len);
#ifdef __cplusplus
}
#endif

/* 
	It already used in hiredis source code.
	Calculate the number of bytes needed to represent an integer as string.
*/
static int intlen(int i) {
	int len = 0;
	if (i < 0) {
		len++;
		i = -i;
	}
	do {
		len++;
		i /= 10;
	} while (i);
	return len;
}

/* 
	It already used in hiredis source code.
	Helper that calculates the bulk length given a certain string length.
*/
static size_t bulklen(size_t len) {
	return 1 + intlen(len) + 2 + len + 2;
}

#endif
/*
	we will use a binary safe string(%b) in format.
	eg: reply = redisCommand(context, "SET foo %b", value, (size_t) valuelen);
*/
namespace CDB
{
CRedis::CRedis()
{
	m_nPort = 0;
	m_nTimeout = 0;
	m_predisContext = NULL;
}

CRedis::~CRedis()
{

}

/*************************************************
Function:  connectSvr
Description: connect redis server
Input: 
	eg:
	ip："192.168.1.2"
	port: 6379
	timeout: 60
Output: no
Return: true: sucess
Others: no
*************************************************/
bool CRedis::connectSvr(const char *ip, int port, unsigned int timeout)
{
	m_strIP = ip;
	m_nPort = port;
	m_nTimeout = timeout;

	struct timeval tvTimeout;
	tvTimeout.tv_sec = timeout / 1000;
	tvTimeout.tv_usec = timeout % 1000 * 1000;

	m_predisContext = redisConnectWithTimeout(ip, port, tvTimeout);
	if (m_predisContext == NULL || m_predisContext->err)
	{
		if (m_predisContext) {
			DB_LOGE("redis Connection ip: %s, port: %d error: %s", ip, port, m_predisContext->errstr);
			disconnectSvr();
		} else {
			DB_LOGE("redis Connection error: can't allocate redis context, ip: %s, port: %d", ip, port);
		}
		return false;
	}

	return true;
}

/*************************************************
Function:  disconnectSvr
Description: disconnect this redis connection
Input: no
Output: no
Return: no
Others: no
*************************************************/
void CRedis::disconnectSvr()
{
	redisFree(m_predisContext);
	m_predisContext = NULL;
}

/*************************************************
Function:  asynSave
Description: fork a child process, synchronize data to disk, return immediately
Input:no
Output: no
Return: 0: success
Others: need use command 'LASTSAVE' to check result
*************************************************/
int CRedis::asynSave()
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("BGSAVE");
	REDIS_NORMAL_JUDGE(reply);

	freeReplyObject(reply);
	return 0;
}

/*************************************************
Function:  save
Description: synchronous save to disk
Input: no
Output: no
Return: 0: success
Others: when data is too much, will slowly
*************************************************/
int CRedis::save()
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("SAVE");
	REDIS_NORMAL_JUDGE(reply);

	if (0 == strncasecmp("ok", reply->str, 3)) {
		freeReplyObject(reply);
		return 0;
	}

	freeReplyObject(reply);
	
	return ERedis::NormalError;
}

/*************************************************
Function:  command
Description: redis universal command interface
Input: format args
Output: no
Return: redisReply*
Others: no
*************************************************/
void* CRedis::command(const char *format, ...)
{
	va_list ap;
	void *reply = NULL;

	if (m_predisContext) {
		va_start(ap, format);
		reply = redisvCommand(m_predisContext, format, ap);
		va_end(ap);
	}

	if (NULL == reply) {
		if (reConnectSvr()) {
			va_start(ap, format);
			reply = redisvCommand(m_predisContext, format, ap);
			va_end(ap);
		}
	}

	return reply;
}

#ifdef USE_EXTERN_HIREDIS_FUNC
void* CRedis::commandArgv(const std::vector<std::string> &vstrOper, const std::vector<std::string> &vstrParam)
{
	char *cmd = NULL;
	int len = 0;
	void *reply = NULL;
	int pos;
	int totlen = 0;

	// format command, this mothod is already used in hiredis.c.
	/* Calculate number of bytes needed for the command */
	totlen = 1 + intlen(vstrOper.size() + vstrParam.size()) + 2;
	for (unsigned int j = 0; j < vstrOper.size(); j++) {
		totlen += bulklen(vstrOper[j].length());
	}
	for (unsigned int j = 0; j < vstrParam.size(); j++) {
		totlen += bulklen(vstrParam[j].length());
	}

	/* Build the command at protocol level */
	cmd = (char *)malloc(totlen + 1);
	if (cmd == NULL)
		return NULL;

	pos = sprintf(cmd, "*%zu\r\n", vstrOper.size() + vstrParam.size());
	//push cmd
	for (unsigned int j = 0; j < vstrOper.size(); j++) {
		pos += sprintf(cmd + pos, "$%zu\r\n", vstrOper[j].length());
		memcpy(cmd + pos, vstrOper[j].c_str(), vstrOper[j].length());
		pos += vstrOper[j].length();
		cmd[pos++] = '\r';
		cmd[pos++] = '\n';
	}

	//push param
	for (unsigned int j = 0; j < vstrParam.size(); j++) {
		pos += sprintf(cmd + pos, "$%zu\r\n", vstrParam[j].length());
		memcpy(cmd + pos, vstrParam[j].c_str(), vstrParam[j].length());
		pos += vstrParam[j].length();
		cmd[pos++] = '\r';
		cmd[pos++] = '\n';
	}
	assert(pos == totlen);
	cmd[pos] = '\0';

	len = totlen;
	//exec command
	reply = execCommand(cmd, len);
	if (NULL == reply) {
		if (reConnectSvr()) {
			reply = execCommand(cmd, len);
		}
	}

	free(cmd);
	return reply;
}

void* CRedis::execCommand(const char *cmd, int len)
{
	void *reply = NULL;
	if (NULL == m_predisContext) {
		return NULL;
	}

	// exec command
	if (__redisAppendCommand(m_predisContext, cmd, len) != REDIS_OK) {
		return NULL;
	}

	// get exec result
	if (m_predisContext->flags & REDIS_BLOCK) {
		if (redisGetReply(m_predisContext, &reply) != REDIS_OK)
		{
			return NULL;
		}
	}

	return reply;
}
#else

void* CRedis::commandArgv(const std::vector<std::string> &vstrOper, const std::vector<std::string> &vstrParam)
{
	char *cmd = NULL;
	void *reply = NULL;
	int pos;
	int totlen = 0;


	totlen = vstrOper.size() + vstrParam.size() - 1;
	for (unsigned int j = 0; j < vstrOper.size(); j++) {
		totlen += vstrOper[j].length();
	}
	for (unsigned int j = 0; j < vstrParam.size(); j++) {
		totlen += vstrParam[j].length();
	}

	/* Build the command at protocol level */
	cmd = (char *)malloc(totlen + 1);
	if (cmd == NULL)
		return NULL;

	pos = 0;
	//push cmd
	for (unsigned int j = 0; j < vstrOper.size(); j++) {
		if(j == 0) {
			pos += sprintf(cmd + pos, "%s", vstrOper[j].c_str());
		} else {
			pos += sprintf(cmd + pos, "%s ", vstrOper[j].c_str());
		}
	}

	//push param
	for (unsigned int j = 0; j < vstrParam.size(); j++) {
		pos += sprintf(cmd + pos, " %s", vstrParam[j].c_str());
	}

	assert(pos == totlen);
	cmd[pos] = '\0';
	//DB_LOGD("cmd:[%s] pos[%d], totlen[%d]", cmd, pos, totlen);
	reply = command(cmd);

	free(cmd);
	return reply;

}
#endif

int CRedis::setKey(const char *key, int keyLen, const char *value, int valueLen, unsigned int lifeTime)
{
	redisReply *reply = NULL;
	if (0 == lifeTime) {
		reply = (redisReply *)command("SET %b %b", key, (size_t)keyLen, value, (size_t)valueLen);
	} else {
		reply = (redisReply *)command("SET %b %b EX %u", key, (size_t)keyLen, value, (size_t)valueLen, lifeTime);
	}
	REDIS_NORMAL_JUDGE(reply);

	freeReplyObject(reply);
	return 0;
}

int CRedis::setKeyRange(const char *key, int keyLen, int offset, const char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("SETRANGE %b %u %b", key, (size_t)keyLen, offset, value, (size_t)valueLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->integer;
	freeReplyObject(reply);

	return len;
}

int CRedis::append(const char *key, int keyLen, const char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("APPEND %b %b", key, (size_t)keyLen, value, (size_t)valueLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->integer;
	freeReplyObject(reply);

	return len;
}

int CRedis::setKeyLifeTime(const char *key, int keyLen, unsigned int time)
{
	redisReply *reply = NULL;
	if (0 == time) {
		reply = (redisReply *)command("PERSIST %b", key, (size_t)keyLen);
	} else {
		reply = (redisReply *)command("EXPIRE %b %u", key, keyLen, time);
	}
	REDIS_NORMAL_JUDGE(reply);

	int ret = 0;
	freeReplyObject(reply);

	return ret;
}

int CRedis::getKey(const char *key, int keyLen, char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("GET %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->len;
	if (len > valueLen) {
		len = ERedis::SpaceNotEnough;
	} else {
		memcpy(value, reply->str, len);
	}
	freeReplyObject(reply);

	return len;
}

int CRedis::getLen(const char *key, int keyLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("STRLEN %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->integer;
	freeReplyObject(reply);

	return len;
}

int CRedis::getKeyByRange(const char *key, int keyLen, int start, int end, char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("GETRANGE %b %d %d", key, (size_t)keyLen, start, end);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->len;

	if (len > valueLen)
	{
		len = ERedis::SpaceNotEnough;
	}
	else
	{
		memcpy(value, reply->str, len);
	}
	freeReplyObject(reply);

	return len;
}

int CRedis::getKeyRemainLifeTime(const char *key, int keyLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("TTL %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->integer;
	freeReplyObject(reply);

	return len;
}

int CRedis::getKeyType(const char *key, int keyLen, char *valueType, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("TYPE %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = reply->len;
	if (reply->len + 1 > (unsigned int)valueLen) {
		len = ERedis::SpaceNotEnough;
	} else {
		memcpy(valueType, reply->str, reply->len + 1);
	}
	freeReplyObject(reply);

	return len;
}

int CRedis::delKey(const char *key, int keyLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("DEL %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

int CRedis::hasKey(const char *key, int keyLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("EXISTS %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

int CRedis::incrByFloat(const char *key, int keyLen, double addValue, double &retValue)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("INCRBYFLOAT %b %f", key, (size_t)keyLen, addValue);
	REDIS_NORMAL_JUDGE(reply);

	retValue = atof(reply->str);
	freeReplyObject(reply);

	return 0;
}

int  CRedis::setMultiKey(const std::vector<std::string> &vstrKeyValue)
{
	redisReply *reply = NULL;
	std::vector<std::string> vstrOper;
	vstrOper.push_back(std::string("MSET"));
	reply = (redisReply *)commandArgv(vstrOper, vstrKeyValue);
	REDIS_NORMAL_JUDGE(reply);

	freeReplyObject(reply);
	return 0;
}

int CRedis::getMultiKey(const std::vector<std::string> &vstrKey, std::vector<std::string> &vstrValue)
{
	redisReply *reply = NULL;
	std::vector<std::string> vstrOper;
	vstrOper.push_back(std::string("MGET"));
	reply = (redisReply *)commandArgv(vstrOper, vstrKey);
	REDIS_NORMAL_JUDGE(reply);

	vstrValue.clear();
	for (unsigned int i = 0; i < reply->elements; i++) {
		vstrValue.push_back(std::string(reply->element[i]->str, reply->element[i]->len));
	}

	freeReplyObject(reply);
	return 0;
}

int CRedis::delMultiKey(const std::vector<std::string> &vstrKey)
{
	redisReply *reply = NULL;
	std::vector<std::string> vstrOper;
	vstrOper.push_back(std::string("DEL"));
	reply = (redisReply *)commandArgv(vstrOper, vstrKey);
	REDIS_NORMAL_JUDGE(reply);

	int len = reply->integer;
	freeReplyObject(reply);
	return len;
}

int CRedis::setHField(const char *key, int keyLen, const char *field, int fieldLen, const char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("HSET %b %b %b", key, (size_t)keyLen, field, (size_t)fieldLen, value, (size_t)valueLen);
	REDIS_NORMAL_JUDGE(reply);

	freeReplyObject(reply);

	return 0;
}

int CRedis::getHField(const char *key, int keyLen, const char *field, int fieldLen, char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("HGET %b %b", key, (size_t)keyLen, field, (size_t)fieldLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->len;
	if (len > valueLen) {
		len = ERedis::SpaceNotEnough;
	} else {
		memcpy(value, reply->str, len);
	}
	freeReplyObject(reply);

	return len;
}

int CRedis::delHField(const char *key, int keyLen, const char *field, int fieldLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("HDEL %b %b", key, (size_t)keyLen, field, (size_t)fieldLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}


int CRedis::hasHField(const char *key, int keyLen, const char *field, int fieldLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("HEXISTS %b %b", key, (size_t)keyLen, field, (size_t)fieldLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

int CRedis::incrHByFloat(const char *key, int keyLen, const char *field, int fieldLen, double addValue, double &retValue)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("HINCRBYFLOAT %b %b %f", key, (size_t)keyLen, field, (size_t)fieldLen, addValue);
	REDIS_NORMAL_JUDGE(reply);

	retValue = atof(reply->str);
	freeReplyObject(reply);

	return 0;
}

int CRedis::getHAll(const char *key, int keyLen, std::vector<std::string> &vstrFieldValue)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("HGETALL %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	vstrFieldValue.clear();
	for (unsigned int i = 0; i < reply->elements; i++) {
		vstrFieldValue.push_back(std::string(reply->element[i]->str, reply->element[i]->len));
	}

	freeReplyObject(reply);
	return 0;
}

int CRedis::getHFieldCount(const char *key, int keyLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("HLEN %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

int CRedis::setMultiHField(const char *key, int keyLen, const std::vector<std::string> &vstrFieldValue)
{
	redisReply *reply = NULL;
	std::vector<std::string> vstrOper;
	vstrOper.push_back(std::string("HMSET"));
	vstrOper.push_back(std::string(key, keyLen));
	reply = (redisReply *)commandArgv(vstrOper, vstrFieldValue);
	REDIS_NORMAL_JUDGE(reply);

	freeReplyObject(reply);
	return 0;
}

int CRedis::getMultiHField(const char *key, int keyLen, const std::vector<std::string> &vstrField, std::vector<std::string> &vstrValue)
{
	redisReply *reply = NULL;
	std::vector<std::string> vstrOper;
	vstrOper.push_back(std::string("HMGET"));
	vstrOper.push_back(std::string(key, keyLen));
	reply = (redisReply *)commandArgv(vstrOper, vstrField);
	REDIS_NORMAL_JUDGE(reply);

	vstrValue.clear();
	for (unsigned int i = 0; i < reply->elements; i++) {
		vstrValue.push_back(std::string(reply->element[i]->str, reply->element[i]->len));
	}

	freeReplyObject(reply);
	return 0;
}

int CRedis::delMultiHField(const char *key, int keyLen, const std::vector<std::string> &vstrField)
{
	redisReply *reply = NULL;
	std::vector<std::string> vstrOper;
	vstrOper.push_back(std::string("HDEL"));
	vstrOper.push_back(std::string(key, keyLen));
	reply = (redisReply *)commandArgv(vstrOper, vstrField);
	REDIS_NORMAL_JUDGE(reply);

	int len = reply->integer;
	freeReplyObject(reply);
	return len;
}


int CRedis::lpushList(const char *key, int keyLen, const char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("LPUSH %b %b", key, (size_t)keyLen, value, (size_t)valueLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

int CRedis::lpopList(const char *key, int keyLen, char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("LPOP %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->len;
	if (len > valueLen) {
		len = ERedis::SpaceNotEnough;
	} else {
		memcpy(value, reply->str, len);
	}
	freeReplyObject(reply);

	return len;
}

int CRedis::rpushList(const char *key, int keyLen, const char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("RPUSH %b %b", key, (size_t)keyLen, value, (size_t)valueLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

int CRedis::rpopList(const char *key, int keyLen, char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("RPOP %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->len;
	if (len > valueLen) {
		len = ERedis::SpaceNotEnough;
	} else {
		memcpy(value, reply->str, len);
	}
	freeReplyObject(reply);

	return len;
}

int CRedis::indexList(const char *key, int keyLen, int index, char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("LINDEX %b %d", key, (size_t)keyLen, index);
	REDIS_NORMAL_JUDGE(reply);

	int len = (int)reply->len;
	if (len > valueLen) {
		len = ERedis::SpaceNotEnough;
	} else {
		memcpy(value, reply->str, len);
	}
	freeReplyObject(reply);

	return len;
}

int CRedis::lenList(const char *key, int keyLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("LLEN %b", key, (size_t)keyLen);
	REDIS_NORMAL_JUDGE(reply);

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

int CRedis::rangeList(const char *key, int keyLen, int start, int end, std::vector<std::string> &vstrList)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("LRANGE %b %d %d", key, (size_t)keyLen, start, end);
	REDIS_NORMAL_JUDGE(reply);

	vstrList.clear();
	for (unsigned int i = 0; i < reply->elements; i++) {
		vstrList.push_back(std::string(reply->element[i]->str, reply->element[i]->len));
	}
	freeReplyObject(reply);

	return 0;
}

int CRedis::setList(const char *key, int keyLen, int index, const char *value, int valueLen)
{
	redisReply *reply = NULL;
	reply = (redisReply *)command("LSET %b %d %b", key, (size_t)keyLen, index, value, (size_t)valueLen);
	REDIS_NORMAL_JUDGE(reply);

	return 0;
}

bool CRedis::reConnectSvr()
{
	disconnectSvr();
	return connectSvr(m_strIP.c_str(), m_nPort, m_nTimeout);
}

}


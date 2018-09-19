#include "DBCommon.h"
#include "CRedis.h"
#include <string.h>

using namespace CDB;

long long getCurrentTime();
void printReply(redisReply *reply);
void freeReply(redisReply *reply);
void dealReply(redisReply *reply);

#define RECV_MAX (4096)

int main(int argc, char **args)
{
	CRedis *redisDb = new CRedis();
	bool connRet = redisDb->connectSvr("127.0.0.1", 6379);

	DB_LOGD("redis connect result : [%d]", connRet);

	redisReply *reply = NULL;
	reply = (redisReply *)redisDb->command("PING");
	dealReply(reply);

	char recv_buf[RECV_MAX];
	int replyRet = 0;
	{
		//
		char *key = "str1";
		int keyLen = strlen(key);
		char *value = "hello world.";
		int valueLen = strlen(value);
		replyRet = redisDb->setKey(key, keyLen, value, valueLen, 0);
		DB_LOGD("setKey ret[%d]", replyRet);

		DB_LOGD("key [%s]", key);
		
		int offset = 6;
		value = "redis";
		valueLen = strlen(value);
		replyRet = redisDb->setKeyRange(key, keyLen, offset, value, valueLen);
		DB_LOGD("setKeyRange ret[%d]", replyRet);

		value = ", hiredis";
		valueLen = strlen(value);
		replyRet = redisDb->append(key, keyLen, value, valueLen);
		DB_LOGD("append ret[%d]", replyRet);

		replyRet = redisDb->setKeyLifeTime(key, keyLen, 60);
		DB_LOGD("setKeyLifeTime ret[%d]", replyRet);

		value = recv_buf;
		valueLen = RECV_MAX;
		replyRet = redisDb->getLen( key, valueLen);
		DB_LOGD("getLen len[%d]", replyRet);

		replyRet = redisDb->getKey(key, keyLen, value, valueLen);
		DB_LOGD("getKey ret[%d]", replyRet);
		if(replyRet > 0) {
			value[replyRet] = '\0';
			DB_LOGD("getKey value[%s]", value);
		}

		int start = 1;
		int end = -2;
		replyRet = redisDb->getKeyByRange( key, keyLen, start, end, value, valueLen);
		DB_LOGD("getKeyByRange ret[%d]", replyRet);
		if(replyRet > 0) {
			value[replyRet] = '\0';
			DB_LOGD("getKeyByRange value[%s]", value);
		}

		replyRet = redisDb->getKeyRemainLifeTime( key, keyLen);
		DB_LOGD("getKeyRemainLifeTime ret[%d]", replyRet);

		char *valueType = recv_buf;
		valueLen = RECV_MAX;
		replyRet = redisDb->getKeyType( key, keyLen, valueType, valueLen);
		DB_LOGD("getKeyType typeLen[%d]", replyRet);
		if(replyRet > 0) {
			value[replyRet] = '\0';
			DB_LOGD("getKeyType type[%s]", valueType);
		}

		replyRet = redisDb->hasKey( key, keyLen);
		DB_LOGD("hasKey has[%d]", replyRet);

		replyRet = redisDb->delKey( key, keyLen);
		DB_LOGD("delKey delRet[%d]", replyRet);

		replyRet = redisDb->hasKey( key, keyLen);
		DB_LOGD("hasKey has[%d]", replyRet);


		key = "double";
		keyLen = strlen(key);

		value = "1.5";
		valueLen = strlen(value);

		double addValue = 1.2;
		double retValue = 0;
		replyRet = redisDb->incrByFloat( key, keyLen, addValue, retValue);
		DB_LOGD("incrByFloat key[%s] addValue[%lf] retValue[%lf]", key, addValue, retValue);

		std::vector<std::string> vstrKeyValue;
		vstrKeyValue.push_back(std::string("key1"));
		vstrKeyValue.push_back(std::string("value1"));
		vstrKeyValue.push_back(std::string("key2"));
		vstrKeyValue.push_back(std::string("value2"));

		std::vector<std::string> vstrKey;
		vstrKey.push_back(std::string("key1"));
		vstrKey.push_back(std::string("key2"));
		
		std::vector<std::string> vstrValue;

		replyRet = redisDb->setMultiKey( vstrKeyValue);
		DB_LOGD("setMultiKey ret[%d]", replyRet);

		replyRet = redisDb->getMultiKey( vstrKey, vstrValue);
		for(int i = 0; i < vstrValue.size(); ++i) {
			DB_LOGD("getMultiKey key[%s] value[%s]", vstrKey.at(i).c_str(), vstrValue.at(i).c_str());
		}
		
		replyRet = redisDb->delMultiKey( vstrKey);
		DB_LOGD("delMultiKey ret[%d]", replyRet);


		key = "hash1";
		keyLen = strlen(key);
		char *field = "field";
		int fieldLen = strlen(field);
		value = "1.5";
		valueLen = strlen(value);

		replyRet = redisDb->setHField( key, keyLen, field, fieldLen, value, valueLen);
		DB_LOGD("setHField ret[%d]", replyRet);
		
		value = recv_buf;
		valueLen = RECV_MAX;
		replyRet = redisDb->getHField( key, keyLen, field, fieldLen, value, valueLen);
		
		DB_LOGD("getHField ret[%d]", replyRet);
		if(replyRet > 0) {
			value[replyRet] = '\0';
			DB_LOGD("getHField value[%s]", value);
		}

		retValue = 0;
		replyRet = redisDb->incrHByFloat( key, keyLen, field, fieldLen, addValue, retValue);
		DB_LOGD("incrByFloat key[%s] field[%s] addValue[%lf] retValue[%lf]", key, field, addValue, retValue);

		std::vector<std::string> vstrFieldValue;
		replyRet = redisDb->getHAll( key, keyLen, vstrFieldValue);
		DB_LOGD("getHAll ret[%d]", replyRet);
		for(int i = 0; i < vstrFieldValue.size(); ++i) {
			if(i%2) {
				DB_LOGD("getHAll value[%s]", vstrFieldValue.at(i).c_str());
			} else {
				DB_LOGD("getHAll key[%s]", vstrFieldValue.at(i).c_str());
			}
		}

		replyRet = redisDb->getHFieldCount( key, keyLen);
		DB_LOGD("getHFieldCount fieldLen[%d]", replyRet);

		replyRet = redisDb->hasHField( key,  keyLen, field, fieldLen);
		DB_LOGD("hasHField hasField[%d]", replyRet);

		replyRet = redisDb->delHField( key, keyLen, field, fieldLen);
		DB_LOGD("delHField delRet[%d]", replyRet);

		replyRet = redisDb->hasHField( key,  keyLen, field, fieldLen);
		DB_LOGD("hasHField hasField[%d]", replyRet);


		key = "list1";
		keyLen = strlen(key);
		value = "1.1";
		valueLen = strlen(value);
		replyRet = redisDb->lpushList( key, keyLen, value, valueLen);
		DB_LOGD("lpushList listLen[%d]", replyRet);


		value = "1.1.1";
		valueLen = strlen(value);
		replyRet = redisDb->lpushList( key, keyLen, value, valueLen);
		DB_LOGD("lpushList listLen[%d]", replyRet);

		value = "2.2.2.2.2";
		valueLen = strlen(value);
		replyRet = redisDb->rpushList( key, keyLen, value, valueLen);
		DB_LOGD("rpushList listLen[%d]", replyRet);

		value = "2.2.2.2";
		valueLen = strlen(value);
		replyRet = redisDb->rpushList( key, keyLen, value, valueLen);
		DB_LOGD("rpushList listLen[%d]", replyRet);


		value = recv_buf;
		valueLen = RECV_MAX;
		replyRet = redisDb->lpopList( key, keyLen, value, valueLen);
		DB_LOGD("lpopList strLen[%d]", replyRet);

		replyRet = redisDb->rpopList( key, keyLen, value, valueLen);
		DB_LOGD("rpopList strLen[%d]", replyRet);

		int index = 1;
		value = recv_buf;
		valueLen = RECV_MAX;
		replyRet = redisDb->indexList( key, keyLen, index, value, valueLen);
		value[5] = '\0';
		DB_LOGD("indexList strLen[%d] str[%s]", replyRet, value);

		replyRet = redisDb->lenList( key, keyLen);
		DB_LOGD("lenList listLen[%d]", replyRet);

		start = 0;
		end = -1;
		std::vector<std::string> vstrList;
		replyRet = redisDb->rangeList( key, keyLen, start, end, vstrList);
		for(int i = 0; i < vstrList.size(); ++i) {
			DB_LOGD("rangeList index[%d] value[%s]", i, vstrList.at(i).c_str());
		}

		index = 0;
		value = "6.6.6";
		valueLen = strlen(value);
		replyRet = redisDb->setList( key, keyLen, index, value, valueLen);
		DB_LOGD("setList ret[%d]", replyRet);

	}

	redisDb->disconnectSvr();

	return 0;
}

long long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void printReply(redisReply *reply)
{
	if(reply != NULL) {
		DB_LOGD("[%ld]redis reply result : [%s]", getCurrentTime(), reply->str);
	} else {
		DB_LOGD("[%ld]redis reply result : [%s]", getCurrentTime());
	}
}

void freeReply(redisReply *reply)
{
	if(reply != NULL) {
		freeReplyObject(reply);
	}
}

void dealReply(redisReply *reply)
{
	printReply(reply);
	freeReply(reply);
}
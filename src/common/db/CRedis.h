#ifndef __CREDIS_H__
#define __CREDIS_H__

#include "extern/hiredis/hiredis.h"
#include <string>
#include <vector>
// use '__redisAppendCommand' or not
/*
#ifndef USE_EXTERN_HIREDIS_FUNC
#define USE_EXTERN_HIREDIS_FUNC
#endif
*/
/* redis 接口注意事项
 * 1、使用get接口时，当返回结果为0（长度）时，该key有可能不存在，也有可能key对应的值为空，若想确定是否为空，需调用对应的 has接口，
      除非你的业务能保证不存在非空值的value.
 * 2、使用has接口时，当返回结果 大于等于1 时，才说明该Key存在，切记不要这样if(has())
 */
namespace CDB
{

	class CRedis
	{
	public:
		CRedis();
		~CRedis();

		//connect server
		bool connectSvr(const char *ip, int port, unsigned int timeout = 1500); //timeout(ms)
		void disconnectSvr();

		int asynSave();//asynchronous save
		int save();//synchronous save

		//universal command interface
		void* command(const char *format, ...);
		void* commandArgv(const std::vector<std::string> &vstrOper, const std::vector<std::string> &vstrParam);

		//redis opt string
		int setKey(const char *key, int keyLen, const char *value, int valueLen, unsigned int lifeTime = 0);//lifeTime(s)
		int setKeyRange(const char *key, int keyLen, int offset, const char *value, int valueLen);//offset 0,1,2...
		int append(const char *key, int keyLen, const char *value, int valueLen);//append string on exist key
		int setKeyLifeTime(const char *key, int keyLen, unsigned int time = 0); //time(s)

		int getKey(const char *key, int keyLen, char *value, int valueLen);//result save in value, return len
		int getLen(const char *key, int keyLen);//return len
		int getKeyByRange(const char *key, int keyLen, int start, int end, char *value, int valueLen);//start,end: 0,1,2...-3,-2,-1
		int getKeyRemainLifeTime(const char *key, int keyLen);// return 2:no have key, return -1:forever
		int getKeyType(const char *key, int keyLen, char *valueType, int valueLen);//result save in valueType, return len

		int delKey(const char *key, int keyLen);//1:success, 0:fail
		int hasKey(const char *key, int keyLen);//1:exist, 0:not exist
		int incrByFloat(const char *key, int keyLen, double addValue, double &retValue);//0:success

		int setMultiKey(const std::vector<std::string> &vstrKeyValue);//0:success
		int getMultiKey(const std::vector<std::string> &vstrKey, std::vector<std::string> &vstrValue);//0:success,result save in vstrValue
		int delMultiKey(const std::vector<std::string> &vstrKey);//return count

		//hash表
		int setHField(const char *key, int keyLen, const char *field, int fieldLen, const char *value, int valueLen);//0:success
		int getHField(const char *key, int keyLen, const char *field, int fieldLen, char *value, int valueLen);//return len
		int delHField(const char *key, int keyLen, const char *field, int fieldLen);//1:success, 0:not exist

		int hasHField(const char *key, int keyLen, const char *field, int fieldLen);//key存在返回1，不存在返回0
		int incrHByFloat(const char *key, int keyLen, const char *field, int fieldLen, double addValue, double &retValue);//0:success
		int getHAll(const char *key, int keyLen, std::vector<std::string> &vstrFieldValue);//0:success
		int getHFieldCount(const char *key, int keyLen);//return field count

		int setMultiHField(const char *key, int keyLen, const std::vector<std::string> &vstrFieldValue);//0:success
		int getMultiHField(const char *key, int keyLen, const std::vector<std::string> &vstrField, std::vector<std::string> &vstrValue);//0:success
		int delMultiHField(const char *key, int keyLen, const std::vector<std::string> &vstrField);//return delete success count
		
		//列表
		int lpushList(const char *key, int keyLen, const char *value, int valueLen);//return list len
		int lpopList(const char *key, int keyLen, char *value, int valueLen);//return value len
		int rpushList(const char *key, int keyLen, const char *value, int valueLen);//return list len
		int rpopList(const char *key, int keyLen, char *value, int valueLen);//return value len

		int indexList(const char *key, int keyLen, int index, char *value, int valueLen);//return value len
		int lenList(const char *key, int keyLen);//return len
		int rangeList(const char *key, int keyLen, int start, int end, std::vector<std::string> &vstrList);//0:success
		int setList(const char *key, int keyLen, int index, const char *value, int valueLen);//0:success

	private:
		CRedis(const CRedis&);
		CRedis& operator =(const CRedis&);
		bool reConnectSvr();
#ifdef USE_EXTERN_HIREDIS_FUNC
		void *execCommand(const char *cmd, int len);
#endif
	private:
		redisContext *m_predisContext;
	
		std::string m_strIP;
		int m_nPort;
		unsigned int m_nTimeout;
	};

}

#endif // CREDIS_H



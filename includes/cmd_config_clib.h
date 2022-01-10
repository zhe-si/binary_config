#ifndef CMD_CONFIG_CMD_CONFIG_CLIB_H
#define CMD_CONFIG_CMD_CONFIG_CLIB_H


#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;


/**
 * 库导入测试
 */
void hello();


/********************* 解包 *************************/

/**
 * 解析 payload 为对象（PayloadObjectMap）（需手动释放）
 */
void * loadPayload(uint8_t * payload, int size);

/**
 * 获得对象的名字。若objName为null，不写入数据
 */
int getObjName(void * obj, char * objName);

/**
 * 从对象（PayloadObjectMap）获得某参数
 */
uint16_t getShortField(void * obj, const char * fieldName);
uint32_t getLongField(void * obj, const char * fieldName);
// 返回的是字符串实际长度，存储长度还要加1（结尾的\0）。若outChars为null，不写入数据
int getCharsField(void * obj, const char * fieldName, char * outChars);
// 将可变长数据的byte原始数据写入data，并返回可变数据的长度；若data为null，不写入数据
int getVarDataField(void * obj, const char * fieldName, uint8_t * data);


/********************* 组包 *************************/

/**
 * 根据命令名生成空对象（PayloadObject）（需手动释放）
 */
void * createObject(const char * cmdName);

/**
 * 根据参数名向对象写入数据
 */
void setShortField(void * obj, const char * fieldName, uint16_t value);
void setLongField(void * obj, const char * fieldName, uint32_t value);
void setCharsField(void * obj, const char * fieldName, const char * value);
void setVarDataField(void * obj, const char * fieldName, const uint8_t * data, int dataSize);

/**
 * 根据对象生成 payload。若 data 不为null，payload写入data；若为null，不写入。
 * @return payload 的长度
 */
int getPayload(void * obj, uint8_t * data);


/**
 * 释放 obj
 */
void releaseObj(void * obj);


#ifdef __cplusplus
}
#endif

#endif //CMD_CONFIG_CMD_CONFIG_CLIB_H

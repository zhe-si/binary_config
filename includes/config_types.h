//
// Created by lq on 2021/12/27.
//

#ifndef CMD_CONFIG_CONFIG_TYPES_H
#define CMD_CONFIG_CONFIG_TYPES_H


typedef struct {
    int size;  // 负数表示自由长度

} FieldType;


extern const FieldType FT_BYTE;
extern const FieldType FT_SHORT;
extern const FieldType FT_LONG;
extern const FieldType FT_CHARS_4;
extern const FieldType FT_CHARS_1;
// 帧尾自由长度数据类型，长度 -1
extern const FieldType FT_VAR_DATA;


typedef struct {
    const char * name;
    int start_pos;  // 若为-1，默认连续，根据前面的字段计算；若指定，需要不重叠且从小到大，不然报错，中间空缺自动补零。（实际会在初始化时计算出每一个字段的起始偏移量）
    const FieldType *type;
    const char * primary_value;  // 主键值，根据载荷是否满足某配置下所有该字段的描述确定是否为某配置，若范围重叠优先前面的配置；若无限制，设为NULL/0

} CmdField;


// 指令最大字段数
#define CMD_MAX_FIELDS_NUM 10


typedef struct {
    const char * name;
    int fields_num;
    CmdField fields[CMD_MAX_FIELDS_NUM];

} Cmd;


// 最大指令数
#define CMDS_NUM 20


typedef struct {
    int cmd_num;
    Cmd cmds[CMDS_NUM];

} CmdManager;


// 配置项（需要自定义该变量作为配置项）
extern const CmdManager cmd_manager;


#endif //CMD_CONFIG_CONFIG_TYPES_H

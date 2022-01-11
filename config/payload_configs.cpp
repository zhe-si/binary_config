//
// Created by lq on 2021/12/27.
//

#include "../includes/config_types.h"


/**
 * 载荷配置项
 *
 * 在此描述不同载荷的具体格式
 * 需标明总指令数和每个指令的字段数
 */
const CmdManager cmd_manager = { 2, {
        {"TCRQ", 3, {
            {"TE_SEQ_NO", -1, &FT_SHORT, 0},
            {"CMD", -1, &FT_CHARS_4, "TCRQ"},
            {"REPEAT_COUNT", -1, &FT_SHORT, 0}}},

        {"EIPC", 6, {
            {"TE_SEQ_NO", -1, &FT_SHORT, 0},
            {"CMD", -1, &FT_CHARS_4, "EIPC"},
            {"AFDX_COM_PORT", -1, &FT_SHORT, 0},
            {"SEND_OR_HOLD", -1, &FT_CHARS_4, 0},
            {"S_U", -1, &FT_CHARS_1, 0},
            {"MESSAGE", -1, &FT_VAR_DATA, 0}}},
}};

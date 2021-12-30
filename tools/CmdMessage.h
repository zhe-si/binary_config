//
// Created by lq on 2021/12/29.
//

#ifndef CMD_CONFIG_CMDMESSAGE_H
#define CMD_CONFIG_CMDMESSAGE_H


#include <vector>
#include <cstdint>

#include "../includes/config_types.h"


// 对 Cmd 的进一步封装
class CmdMessage {
public:
    explicit CmdMessage(const Cmd &_cmd);

    const Cmd& getCmd() const;
    int getCmdPayloadMinSize() const;
    bool isHasUnknownFieldSize() const;
    bool checkRequires(const std::vector<uint8_t>& payload) const;

private:
    Cmd cmd{};
    int cmdPayloadMinSize = -1;
    bool hasUnknownFieldSize = false;
    // 标记存在要求的字段
    std::vector<int> requires;
};


#endif //CMD_CONFIG_CMDMESSAGE_H

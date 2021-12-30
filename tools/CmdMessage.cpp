//
// Created by lq on 2021/12/29.
//

#include <stdexcept>
#include "CmdMessage.h"


CmdMessage::CmdMessage(const Cmd &_cmd) {
    cmd = _cmd;
    int nowPos = 0;
    for (int j = 0; j < cmd.fields_num; j++) {
        CmdField &field = cmd.fields[j];
        if (field.start_pos == -1) {
            field.start_pos = nowPos;
        } else if (field.start_pos >= nowPos) {
            nowPos = field.start_pos;
        } else {
            throw std::runtime_error(std::string("payload configs error: cmd->") + cmd.name + "  field->" + field.name);
        }
        int typeSize = field.type->size;
        if (typeSize > 0) {
            nowPos += typeSize;
        } else if (field.type == &VAR_DATA) {
            if (j == (cmd.fields_num - 1)) {
                // 队尾可变长类型，不计入长度
                hasUnknownFieldSize = true;
            } else {
                throw std::runtime_error("this type size is -1, but not at payload's tail");
            }
        } else {
            throw std::runtime_error("this type size is known, if is a new type, please update CmdMessage's code");
        }
        if (nullptr != field.primary_value) requires.push_back(j);
    }
    cmdPayloadMinSize = nowPos;
}

const Cmd& CmdMessage::getCmd() const {
    return cmd;
}

int CmdMessage::getCmdPayloadMinSize() const {
    return cmdPayloadMinSize;
}

bool CmdMessage::checkRequires(const std::vector<uint8_t>& payload) const {
    if (hasUnknownFieldSize && payload.size() < cmdPayloadMinSize) return false;
    if (!hasUnknownFieldSize && payload.size() != cmdPayloadMinSize) return false;
    for (int r : requires) {
        CmdField field = cmd.fields[r];
        std::string require(field.primary_value);
        std::vector<uint8_t> fieldPayload;
        if (field.type == &VAR_DATA) {
            fieldPayload = std::vector<uint8_t>(payload.begin() + field.start_pos,
                                                payload.end());
        } else {
            fieldPayload = std::vector<uint8_t>(payload.begin() + field.start_pos,
                                                payload.begin() + field.start_pos + field.type->size);
        }
        // 因为只需要比较CMD的字符串，当前默认直接比较载荷。若有新的需要，拓展此处。
        if (require.size() != fieldPayload.size()) return false;
        for (int i = 0; i < require.size(); i++) {
            if (require[i] != fieldPayload[i]) return false;
        }
    }
    return true;
}

bool CmdMessage::isHasUnknownFieldSize() const {
    return hasUnknownFieldSize;
}

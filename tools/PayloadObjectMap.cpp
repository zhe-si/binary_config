//
// Created by lq on 2021/12/27.
//

#include "../includes/tools/PayloadObjectMap.h"

#include <utility>


PayloadObjectMap::~PayloadObjectMap() {
    delete payload;
    delete cmdDescription;
}

PayloadObjectMap::PayloadObjectMap(const CmdMessage& cmdMessage) {
    cmdDescription = new std::map<std::string, CmdField>();

    const Cmd &cmd = cmdMessage.getCmd();
    cmdName = cmd.name;
    for (int i = 0; i < cmd.fields_num; i++) {
        const CmdField &cmdField = cmd.fields[i];
        // map执行拷贝构造
        (*cmdDescription)[std::string(cmdField.name)] = cmdField;
    }

    payloadMinSize = cmdMessage.getCmdPayloadMinSize();
    hasUnknownFieldSize = cmdMessage.isHasUnknownFieldSize();

    payload = new std::vector<uint8_t>(payloadMinSize, 0);
}

bool PayloadObjectMap::loadPayload(const std::vector<uint8_t>& _payload) {
    if (_payload.size() >= getMinSize()) {
        payload->clear();
        for (auto &d : _payload) payload->push_back(d);
        return true;
    }
    return false;
}

void PayloadObjectMap::setField(const std::string& field_name, const std::vector<uint8_t>& field_data) {
    CmdField cmdField = (*cmdDescription).at(field_name);
    if (cmdField.type == &VAR_DATA) {
        // set末尾可变长类型
        clearVarData();
        for (auto d : field_data) payload->push_back(d);
    } else {
        for (int i = 0; i < cmdField.type->size; i++) {
            (*payload)[i + cmdField.start_pos] = field_data[i];
        }
    }
}

void PayloadObjectMap::setField(const std::string &field_name, Field *field, bool autoReleaseField) {
    setField(field_name, field->getPayload());
    if (autoReleaseField) delete field;
}

Field * PayloadObjectMap::getField(const std::string& field_name) {
    CmdField cmdField = (*cmdDescription).at(field_name);
    std::vector<uint8_t> field_data;
    if (cmdField.type == &VAR_DATA) {
        for (auto p = payload->begin() + getMinSize(); p != payload->end(); p++) {
            field_data.push_back((*p));
        }
    } else {
        field_data.reserve(cmdField.type->size);
        for (int i = 0; i < cmdField.type->size; i++) {
            field_data.push_back((*payload)[cmdField.start_pos + i]);
        }
    }
    return Field::createFieldFromPayload(cmdField.type, field_data);
}

int PayloadObjectMap::getMinSize() const {
    return payloadMinSize;
}

int PayloadObjectMap::getNowSize() const {
    return payload->size();
}

void PayloadObjectMap::clearVarData() {
    payload->erase(payload->begin() + this->getMinSize(), payload->end());
}

bool PayloadObjectMap::isHasUnknownFieldSize() const {
    return hasUnknownFieldSize;
}

const std::vector<uint8_t> &PayloadObjectMap::getPayload() const {
    return (*payload);
}

std::string PayloadObjectMap::getCmdName() {
    return cmdName;
}


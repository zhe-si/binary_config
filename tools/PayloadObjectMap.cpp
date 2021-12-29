//
// Created by lq on 2021/12/27.
//

#include "PayloadObjectMap.h"

#include <utility>

PayloadObjectMap::~PayloadObjectMap() {
    delete payload;
    delete description;
}

PayloadObjectMap::PayloadObjectMap(Cmd cmd) {
    description = new std::map<std::string, CmdField>();

    int nowPos = 0;
    for (int i = 0; i < cmd.fields_num; i++) {
        CmdField cmdField = cmd.fields[i];
        cmdField.start_pos = nowPos;
        nowPos += cmdField.type->size;
        std::string nameString(cmdField.name);
        (*description)[nameString] = cmdField;
    }
    totalSize = nowPos;
    payload = new std::vector<uint8_t>(totalSize, 0);
}

void PayloadObjectMap::loadPayload(const std::vector<uint8_t>& _payload) {
    payload->clear();
    for (auto &d : _payload) payload->push_back(d);
}

void PayloadObjectMap::setField(const std::string& field_name, const std::vector<uint8_t>& field_data) {
    CmdField cmdField = (*description)[field_name];
    for (int i = 0; i < cmdField.type->size; i++) {
        (*payload)[i + cmdField.start_pos] = field_data[i];
    }
}

void PayloadObjectMap::setField(const std::string &field_name, Field *field, bool autoReleaseField) {
    setField(field_name, field->getPayload());
    if (autoReleaseField) delete field;
}

Field * PayloadObjectMap::getField(const std::string& field_name) {
    CmdField cmdField = (*description)[field_name];
    std::vector<uint8_t> field_data;
    field_data.reserve(cmdField.type->size);
    for (int i = 0; i < cmdField.type->size; i++) {
        field_data.push_back((*payload)[cmdField.start_pos + i]);
    }
    return Field::createFieldFromPayload(cmdField.type, field_data);
}

int PayloadObjectMap::getTotalSize() const {
    return totalSize;
}

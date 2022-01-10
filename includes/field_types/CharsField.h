//
// Created by lq on 2021/12/28.
//

#ifndef CMD_CONFIG_CHARSFIELD_H
#define CMD_CONFIG_CHARSFIELD_H


#include "Field.h"

/**
 * 将payload解析为std::string
 */
class CharsField: public Field {
public:
    CharsField(const FieldType *fieldType, const std::vector<uint8_t> &data);
    CharsField(const std::string &value, const FieldType *fieldType);

    std::any getValue() override;
    std::vector<uint8_t> getPayload() override;
};


#endif //CMD_CONFIG_CHARSFIELD_H

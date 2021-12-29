//
// Created by lq on 2021/12/28.
//

#ifndef CMD_CONFIG_INTFIELD_H
#define CMD_CONFIG_INTFIELD_H


#include "Field.h"


template<typename FT>
class IntField: public Field {
public:
    IntField(const FieldType *fieldType, const std::vector<uint8_t> &data) : Field(fieldType, data) {}
    IntField(const std::any &value, const FieldType *fieldType) : Field(value, fieldType) {}

    std::any getValue() override {
        return getIntValue();
    }

    FT getIntValue() {
        if (!value.has_value()) value = payloadToUnsignedInt(data, fieldType->size, static_cast<FT>(-1));
        return std::any_cast<FT>(value);
    }

    std::vector<uint8_t> getPayload() override {
        if (data.empty()) data = intToPayload(std::any_cast<FT>(value), fieldType->size);
        return data;
    }
};


#endif //CMD_CONFIG_INTFIELD_H

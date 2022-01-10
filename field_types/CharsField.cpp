//
// Created by lq on 2021/12/28.
//

#include "../includes/field_types/CharsField.h"


std::any CharsField::getValue() {
    if (!value.has_value()) value = payloadToChars(data, fieldType->size);
    return value;
}

std::vector<uint8_t> CharsField::getPayload() {
    if (data.empty()) data = stringToPayload(std::any_cast<std::string>(value), fieldType->size);
    return data;
}

CharsField::CharsField(const std::string &value, const FieldType *fieldType) : Field(value, fieldType) {}

CharsField::CharsField(const FieldType *fieldType, const std::vector<uint8_t> &data) : Field(fieldType, data) {}

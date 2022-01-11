//
// Created by lq on 2021/12/28.
//

#include "../includes/field_types/Field.h"
#include "../includes/field_types/IntField.h"
#include "../includes/field_types/CharsField.h"

#include <utility>


Field::Field(const FieldType *fieldType, const std::vector<uint8_t> &data) {
    this->fieldType = fieldType;
    this->data = data;
}

Field::Field(std::any value, const FieldType *fieldType) {
    this->value = std::move(value);
    this->fieldType = fieldType;
}

std::any Field::getValue() {
    if (!this->value.has_value()) this->value = this->data;
    return this->value;
}

std::vector<uint8_t> Field::getPayload() {
    return this->data;
}

Field * Field::createFieldFromPayload(const FieldType *fieldType, const std::vector<uint8_t> &data) {
    if (fieldType == &FT_BYTE) {
        return new IntField<uint8_t>(fieldType, data);
    } else if (fieldType == &FT_SHORT) {
        return new IntField<uint16_t>(fieldType, data);
    } else if (fieldType == &FT_LONG) {
        return new IntField<uint32_t>(fieldType, data);
    } else if (fieldType == &FT_CHARS_4 || fieldType == &FT_CHARS_1) {
        return new CharsField(fieldType, data);
    } else if (fieldType == &FT_VAR_DATA) {
        return new Field(fieldType, data);
    } else {
        throw std::runtime_error("unknown this fieldType");
    }
}

Field * Field::createFieldFromValue(const FieldType *fieldType, const std::any& value) {
    if (fieldType == &FT_BYTE) {
        return new IntField<uint8_t>(value, fieldType);
    } else if (fieldType == &FT_SHORT) {
        return new IntField<uint16_t>(value, fieldType);
    } else if (fieldType == &FT_LONG) {
        return new IntField<uint32_t>(value, fieldType);
    } else if (fieldType == &FT_CHARS_4 || fieldType == &FT_CHARS_1) {
        std::string value_s;
        try {
            value_s = std::any_cast<std::string>(value);
        } catch (std::bad_any_cast &e) {
            value_s = std::any_cast<const char*>(value);
        }
        return new CharsField(value_s, fieldType);
    } else if (fieldType == &FT_VAR_DATA) {
        return new Field(value, fieldType);
    } else {
        throw std::runtime_error("unknown this fieldType");
    }
}

std::string payloadToChars(const std::vector<uint8_t> &data, int charsSize) {
    if (data.size() < charsSize) throw std::runtime_error("data size is less than charsSize");
    return std::string{data.begin(), data.begin() + charsSize};
}

std::vector<uint8_t> stringToPayload(std::string value, int fieldSize) {
    if (value.size() < fieldSize) throw std::runtime_error("string size is less than field size");
    return std::vector<uint8_t>{value.begin(), value.begin() + fieldSize};
}

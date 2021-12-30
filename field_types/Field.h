//
// Created by lq on 2021/12/28.
//

#ifndef CMD_CONFIG_FIELD_H
#define CMD_CONFIG_FIELD_H

#include <any>
#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <map>
#include "../includes/config_types.h"


class Field {
public:
    [[nodiscard]] static Field * createFieldFromPayload(const FieldType * fieldType, const std::vector<uint8_t>& data);
    [[nodiscard]] static Field * createFieldFromValue(const FieldType * fieldType, const std::any& value);

    Field(const FieldType * fieldType, const std::vector<uint8_t>& data);
    Field(std::any value, const FieldType * fieldType);
    virtual ~Field() = default;

    virtual std::any getValue();
    virtual std::vector<uint8_t> getPayload();

protected:
    const FieldType * fieldType = nullptr;
    // 载荷数据，大端，高位在前
    std::vector<uint8_t> data;
    // 实际值
    std::any value;
};


/**
 * 将data转换为无符号整形数字（无符号char，short，int，long，long long等）
 * @tparam T 目标类型
 * @param data 载荷数据 byte数组
 * @param fieldSize 字段长度
 * @param default_value 默认值
 * @return 根据data转换的无符号整形数据
 */
template<typename T>
T payloadToUnsignedInt(std::vector<uint8_t> data, int fieldSize, T default_value) {
    if (fieldSize > data.size()) return default_value;
    T value = 0;
    for (int i = 0; i < fieldSize; i++) {
        value |= (data[i] & 0xff) << ((fieldSize - 1 - i) << 3);
    }
    return value;
}


/**
 * 无符号整形转换为载荷 byte数组
 * @return 载荷 byte数组
 */
template<typename T>
std::vector<uint8_t> intToPayload(T value, int fieldSize) {
    std::vector<uint8_t> data(fieldSize, 0);
    for (int i = 0; i < fieldSize; i++) {
        data[i] = (value >> ((fieldSize - 1 - i) << 3)) & 0xff;
    }
    return data;
}


std::vector<uint8_t> stringToPayload(std::string value, int fieldSize);


std::string payloadToChars(const std::vector<uint8_t>& data, int charsSize);


#endif //CMD_CONFIG_FIELD_H

//
// Created by lq on 2021/12/27.
//

#ifndef CMD_CONFIG_PAYLOADOBJECTMAP_H
#define CMD_CONFIG_PAYLOADOBJECTMAP_H


#include <vector>
#include <map>
#include <any>

#include "../includes/config_types.h"
#include "../field_types/Field.h"
#include "CmdMessage.h"


/**
 * 载荷（byte数组）与对象的映射器
 */
class PayloadObjectMap {
public:
    explicit PayloadObjectMap(const CmdMessage& cmdMessage);

    ~PayloadObjectMap();

    int getMinSize() const;
    // 获得当前组包的实际大小，可能包括可变长参数，nowSize >= minSize
    int getNowSize() const;
    bool isHasUnknownFieldSize() const;

    // 若加载内容与该 PayloadObjectMap 对应的 Cmd 不匹配，取消加载，返回false
    bool loadPayload(const std::vector<uint8_t>& _payload);

    // 若 field_name 不存在，抛异常 std::out_of_range
    void setField(const std::string &field_name, const std::vector<uint8_t>& field_data);
    // 若 field_name 不存在，抛异常 std::out_of_range
    void setField(const std::string &field_name, Field *field, bool autoReleaseField = false);

    /**
     * 通过泛型将value转化为目标类型
     * @tparam T 目标类型
     * @param field_name 字段名
     * @param default_value 默认值，目标类型，若为string，不可以写成 “abc”，要写成 string("abc")
     * @return 目标类型的字段值
     * @throw 若 field_name 不存在，抛异常 std::out_of_range
     */
    template<typename T> T getField(const std::string& field_name, T default_value) {
        Field* field = getField(field_name);
        std::any value_any = field->getValue();
        T value;
        try {
            value = std::any_cast<T>(value_any);
        } catch (std::bad_any_cast &e) {
            throw std::runtime_error("value cast type error, make sure default value is really your target type value. \n\tIf you want to get a string but default value is char* , try to cast char* as string");
        }
        delete field;
        return value;
    }
    // Field 需要手动释放。若 field_name 不存在，抛异常 std::out_of_range。
    Field * getField(const std::string& field_name);

private:
    std::map<std::string, CmdField> * cmdDescription = nullptr;
    int payloadMinSize = -1;
    bool hasUnknownFieldSize = false;
    std::vector<uint8_t> * payload = nullptr;

    void clearVarData();
};


#endif //CMD_CONFIG_PAYLOADOBJECTMAP_H

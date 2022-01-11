//
// Created by lq on 2021/12/29.
//

#ifndef CMD_CONFIG_PAYLOADOBJECTMAPFACTORY_H
#define CMD_CONFIG_PAYLOADOBJECTMAPFACTORY_H


#include <map>
#include <vector>
#include <string>

#include "../config_types.h"
#include "PayloadObjectMap.h"
#include "CmdMessage.h"


/**
 * PayloadObjectMap简单工厂
 * 从前到后匹配符合规则的指令（Cmd）并基于匹配到的指令产生PayloadObjectMap
 */
class PayloadObjectMapFactory {
public:
    static PayloadObjectMapFactory * getInstance();

    // 为了测试，设置为public，不限制单例构造不可见，但正常调用请使用 getInstance
    explicit PayloadObjectMapFactory(const CmdManager &cmdManager);

    /**
     * 根据名字得到对应 Cmd 信息。
     * 因为工厂是全局单例，所以不会出现引用的对象先被释放的问题。
     * @param name cmd 的名字
     * @return cmd 信息对象
     */
    const CmdMessage& getCmdMessageByName(const std::string& name) const;

    /**
     * 获取所有 cmds 信息，用于遍历使用。
     * 因为工厂是全局单例，所以不会出现引用的对象先被释放的问题。
     * @return cmd 名字到 cmd 信息的映射 Map
     */
    const std::map<std::string, CmdMessage>& getCmdsMap() const;

    /**
     * 根据名字创建新 PayloadObjectMap 对象
     * @param name cmd 名字
     * @return 对应名字的 cmd 的 PayloadObjectMap 对象
     */
    [[nodiscard]] PayloadObjectMap * createPOMByName(const std::string& name) const;

    /**
     * 根据载荷解析创建 PayloadObjectMap 对象。
     * 返回nullptr表示无匹配项，创建失败。
     * @param payload 载荷（std::vector<uint8_t>）
     * @return 解析了载荷的 PayloadObjectMap 对象
     */
    [[nodiscard]] PayloadObjectMap * createPOMByPayload(const std::vector<uint8_t>& payload) const;
    /**
     * 根据载荷解析创建 PayloadObjectMap 对象。
     * 返回nullptr表示无匹配项，创建失败。
     * @param payload 载荷（uint8_t 数组）
     * @return 解析了载荷的 PayloadObjectMap 对象
     */
    [[nodiscard]] PayloadObjectMap * createPOMByPayload(const uint8_t * payload, int size) const;

private:
    // 根据cmd_manager配置生成
    std::map<std::string, CmdMessage> cmdsMap;
};


#endif //CMD_CONFIG_PAYLOADOBJECTMAPFACTORY_H

//
// Created by lq on 2021/12/29.
//

#ifndef CMD_CONFIG_PAYLOADOBJECTMAPFACTORY_H
#define CMD_CONFIG_PAYLOADOBJECTMAPFACTORY_H


#include <map>
#include <vector>
#include <string>
#include "../includes/config_types.h"
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

    // PayloadObjectMap* 需要手动释放。返回nullptr表示无匹配项，创建失败。
    PayloadObjectMap * createPayloadObjectMap(const std::vector<uint8_t>& payload) const;
    PayloadObjectMap * createPayloadObjectMap(const uint8_t * payload, int size) const;

private:
    // 根据cmd_manager配置生成
    std::map<std::string, CmdMessage> cmdsMap;
};


#endif //CMD_CONFIG_PAYLOADOBJECTMAPFACTORY_H

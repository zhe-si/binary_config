//
// Created by lq on 2021/12/29.
//

#include "../includes/tools/PayloadObjectMapFactory.h"


PayloadObjectMapFactory::PayloadObjectMapFactory(const CmdManager &cmdManager) {
    for (int i = 0; i < cmdManager.cmd_num; i++) {
        const Cmd &cmd = cmdManager.cmds[i];
        cmdsMap.insert(std::make_pair(std::string(cmd.name), CmdMessage(cmd)));
    }
}

PayloadObjectMapFactory * PayloadObjectMapFactory::getInstance() {
    static PayloadObjectMapFactory factory(cmd_manager);
    return &factory;
}

PayloadObjectMap * PayloadObjectMapFactory::createPOMByPayload(const std::vector<uint8_t>& payload) const {
    for (const auto& cmd : cmdsMap) {
        if (cmd.second.checkRequires(payload)) {
            auto * payloadObjectMap = new PayloadObjectMap(cmd.second);
            if (payloadObjectMap->loadPayload(payload)) {
                return payloadObjectMap;
            } else {
                // 兜底逻辑。按照正常逻辑，经过了CmdMessage检查，不应走到这里
                delete payloadObjectMap;
                return nullptr;
            }
        }
    }
    return nullptr;
}

PayloadObjectMap * PayloadObjectMapFactory::createPOMByPayload(const uint8_t *payload, int size) const {
    std::vector<uint8_t> payloadVector;
    payloadVector.reserve(size);
    for (int i = 0; i < size; i++) payloadVector.push_back(payload[i]);
    return PayloadObjectMapFactory::createPOMByPayload(payloadVector);
}

const CmdMessage & PayloadObjectMapFactory::getCmdMessageByName(const std::string& name) const {
    return cmdsMap.at(name);
}

PayloadObjectMap * PayloadObjectMapFactory::createPOMByName(const std::string &name) const {
    const CmdMessage &cmdMsg = getCmdMessageByName(name);
    return new PayloadObjectMap(cmdMsg);
}

const std::map<std::string, CmdMessage> & PayloadObjectMapFactory::getCmdsMap() const {
    return cmdsMap;
}

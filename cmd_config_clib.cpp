#include "includes/cmd_config_clib.h"

#include <iostream>
#include <vector>
#include <cstring>

#include "tools/PayloadObjectMapFactory.h"


void hello() {
    std::cout << "hello, use lib successful!" << std::endl;
}


void * loadPayload(uint8_t * payload, int size) {
    auto factory = PayloadObjectMapFactory::getInstance();
    auto obj = factory->createPayloadObjectMap(payload, size);
    return obj;
}

uint16_t getShortField(void * obj, char * fieldName) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    return poMap->getField(std::string(fieldName), uint16_t(-1));
}

uint32_t getLongField(void * obj, char * fieldName) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    return poMap->getField(std::string(fieldName), uint32_t(-1));
}

int getCharsField(void * obj, char * fieldName, char * outChars) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    std::string field = poMap->getField(std::string(fieldName), std::string());
    if (nullptr != outChars) {
        strcpy_s(outChars, field.size() + 1, field.data());
    }
    return static_cast<int>(field.size());
}

int getVarDataField(void * obj, char * fieldName, uint8_t * data) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    std::vector<uint8_t> field = poMap->getField(std::string(fieldName), std::vector<uint8_t>());
    if (nullptr != data) {
        for (int i = 0; i < field.size(); i++) data[i] = field[i];
    }
    return static_cast<int>(field.size());
}

void * createObject(char *cmdName) {
    auto factory = PayloadObjectMapFactory::getInstance();
    const CmdMessage &cmdMsg = factory->getCmdMessageByName(std::string(cmdName));
    return new PayloadObjectMap(cmdMsg);
}

void setShortField(void *obj, char *fieldName, uint16_t value) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    poMap->setField(std::string(fieldName), Field::createFieldFromValue(&SHORT, value), true);
}

void setLongField(void *obj, char *fieldName, uint32_t value) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    poMap->setField(std::string(fieldName), Field::createFieldFromValue(&LONG, value), true);
}

void setCharsField(void * obj, char * fieldName, char * value) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    const FieldType * type;
    size_t charsLength = strlen(value);
    if (1 == charsLength) {
        type = &CHARS_1;
    } else if (4 == charsLength) {
        type = &CHARS_4;
    } else {
        throw std::runtime_error("chars length is unknown, now has CHARS_1 and CHARS_4");
    }
    poMap->setField(std::string(fieldName), Field::createFieldFromValue(type, value), true);
}

void setVarDataField(void *obj, char *fieldName, uint8_t *data, int dataSize) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    poMap->setField(std::string(fieldName), std::vector<uint8_t>(data, data + dataSize));
}

int getPayload(void *obj, uint8_t *data) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    const std::vector<uint8_t> &payload = poMap->getPayload();
    if (nullptr != data) {
        for (int i = 0; i < payload.size(); i++) data[i] = payload[i];
    }
    return static_cast<int>(payload.size());
}

void releaseObj(void *obj) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    delete poMap;
}

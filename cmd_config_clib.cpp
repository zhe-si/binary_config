#include "includes/cmd_config_clib.h"

#include <iostream>
#include <vector>
#include <cstring>

#include "includes/tools/PayloadObjectMapFactory.h"


void hello() {
    std::cout << "hello, use lib successful!" << std::endl;
}


void * loadPayload(uint8_t * payload, int size) {
    auto factory = PayloadObjectMapFactory::getInstance();
    auto obj = factory->createPOMByPayload(payload, size);
    return obj;
}

uint8_t getByteField(void *obj, const char *fieldName) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    return poMap->getField(std::string(fieldName), uint8_t(-1));
}

uint16_t getShortField(void * obj, const char * fieldName) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    return poMap->getField(std::string(fieldName), uint16_t(-1));
}

uint32_t getLongField(void * obj, const char * fieldName) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    return poMap->getField(std::string(fieldName), uint32_t(-1));
}

int getCharsField(void * obj, const char * fieldName, char * outChars) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    std::string field = poMap->getField(std::string(fieldName), std::string());
    if (nullptr != outChars) {
        strcpy_s(outChars, field.size() + 1, field.data());
    }
    return static_cast<int>(field.size());
}

int getVarDataField(void * obj, const char * fieldName, uint8_t * data) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    std::vector<uint8_t> field = poMap->getField(std::string(fieldName), std::vector<uint8_t>());
    if (nullptr != data) {
        for (int i = 0; i < field.size(); i++) data[i] = field[i];
    }
    return static_cast<int>(field.size());
}

void * createObject(const char *cmdName) {
    auto factory = PayloadObjectMapFactory::getInstance();
    return factory->createPOMByName(std::string(cmdName));
}

void setByteField(void *obj, const char *fieldName, uint8_t value) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    poMap->setField(std::string(fieldName), Field::createFieldFromValue(&FT_BYTE, value), true);
}

void setShortField(void *obj, const char *fieldName, uint16_t value) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    poMap->setField(std::string(fieldName), Field::createFieldFromValue(&FT_SHORT, value), true);
}

void setLongField(void *obj, const char *fieldName, uint32_t value) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    poMap->setField(std::string(fieldName), Field::createFieldFromValue(&FT_LONG, value), true);
}

void setCharsField(void * obj, const char * fieldName, const char * value) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    const FieldType * type;
    size_t charsLength = strlen(value);
    if (1 == charsLength) {
        type = &FT_CHARS_1;
    } else if (4 == charsLength) {
        type = &FT_CHARS_4;
    } else {
        throw std::runtime_error("chars length is unknown, now has FT_CHARS_1 and FT_CHARS_4");
    }
    poMap->setField(std::string(fieldName), Field::createFieldFromValue(type, value), true);
}

void setVarDataField(void *obj, const char *fieldName, const uint8_t *data, int dataSize) {
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

int getObjName(void * obj, char *objName) {
    auto * poMap = static_cast<PayloadObjectMap *>(obj);
    std::string cmdName = poMap->getCmdName();
    if (nullptr != objName) {
        strcpy_s(objName, cmdName.size() + 1, cmdName.data());
    }
    return static_cast<int>(cmdName.size());
}

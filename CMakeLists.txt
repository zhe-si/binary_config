cmake_minimum_required(VERSION 3.20)
project(cmd_config)

set(CMAKE_CXX_STANDARD 17)

# 命令解析工具库
add_library(cmd_config
        library.cpp

        field_types/Field.h
        field_types/Field.cpp
        field_types/IntField.h
        field_types/CharsField.h
        field_types/CharsField.cpp

        # 公共配置类型
        includes/config_types.h

        tools/config_types.cpp
        config/payload_configs.cpp

        tools/PayloadObjectMap.h
        tools/PayloadObjectMap.cpp
        tools/PayloadObjectMapFactory.h
        tools/PayloadObjectMapFactory.cpp
        tools/CmdMessage.h
        tools/CmdMessage.cpp
        )

# 测试
add_executable(test
        tests/test_main.cpp

        field_types/Field.h
        field_types/Field.cpp
        field_types/IntField.h
        field_types/CharsField.h
        field_types/CharsField.cpp

        includes/config_types.h
        tools/config_types.cpp
        config/payload_configs.cpp

        tools/PayloadObjectMap.h
        tools/PayloadObjectMap.cpp
        tools/PayloadObjectMapFactory.h
        tools/PayloadObjectMapFactory.cpp
        tools/CmdMessage.h
        tools/CmdMessage.cpp
        )
cmake_minimum_required(VERSION 3.20)
project(cmd_config)

set(CMAKE_CXX_STANDARD 17)

# 命令解析工具库
set(LIB_NAME cmd_config)
add_library(${LIB_NAME}
        cmd_config_clib.cpp
        cmd_config_cxxlib.cpp

        includes/field_types/Field.h
        field_types/Field.cpp
        includes/field_types/IntField.h
        includes/field_types/CharsField.h
        field_types/CharsField.cpp

        # 公共配置类型
        includes/config_types.h

        tools/config_types.cpp
        config/payload_configs.cpp

        includes/tools/PayloadObjectMap.h
        tools/PayloadObjectMap.cpp
        includes/tools/PayloadObjectMapFactory.h
        tools/PayloadObjectMapFactory.cpp
        includes/tools/CmdMessage.h
        tools/CmdMessage.cpp
        )
set(LIB_DIR ${LIB_NAME}_lib)
add_custom_command(TARGET ${LIB_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo create ${LIB_NAME} lib dir...
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${LIB_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy lib${LIB_NAME}.a ${LIB_DIR}/libs/${LIB_NAME}.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/includes ${LIB_DIR}/includes
        COMMAND ${CMAKE_COMMAND} -E echo create ${LIB_NAME} lib dir successful
        )


# 测试
add_executable(test
        tests/test_main.cpp

        includes/field_types/Field.h
        field_types/Field.cpp
        includes/field_types/IntField.h
        includes/field_types/CharsField.h
        field_types/CharsField.cpp

        includes/config_types.h
        tools/config_types.cpp
        config/payload_configs.cpp

        includes/tools/PayloadObjectMap.h
        tools/PayloadObjectMap.cpp
        includes/tools/PayloadObjectMapFactory.h
        tools/PayloadObjectMapFactory.cpp
        includes/tools/CmdMessage.h
        tools/CmdMessage.cpp
        )
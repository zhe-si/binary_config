cmake_minimum_required(VERSION 3.20)
project(cmd_config)

set(CMAKE_CXX_STANDARD 17)

# 命令解析工具库
add_library(cmd_config
        cmd_config_clib.cpp
        cmd_config_cxxlib.cpp

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
set(LIB_DIR cmd_config_lib)
add_custom_command(TARGET cmd_config
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo_append "create cmd_config lib dir ... "
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${LIB_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy libcmd_config.a ${LIB_DIR}/libs/cmd_config.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/includes ${LIB_DIR}/includes
        COMMAND ${CMAKE_COMMAND} -E echo successful
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
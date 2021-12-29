cmake_minimum_required(VERSION 3.20)
project(cmd_config)

set(CMAKE_CXX_STANDARD 17)

add_library(cmd_config library.cpp tools/PayloadObjectMap.cpp tools/PayloadObjectMap.h config/payload_configs.cpp includes/config_types.h tools/config_types.cpp field_types/Field.cpp field_types/Field.h field_types/IntField.h field_types/CharsField.cpp field_types/CharsField.h)

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
        )
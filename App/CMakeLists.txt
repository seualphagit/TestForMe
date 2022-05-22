cmake_minimum_required(VERSION 3.0)
project(xsd_core)

# 如果代码需要支持C++11，就直接加上这句
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
add_definitions("-Wall -g")

# 设置变量，下面的代码都可以用到
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INC_DIR ${PROJECT_SOURCE_DIR}/inc)

aux_source_directory(${SRC_DIR} SRC_FILES)



# 规定.h头文件路径
include_directories(${INC_DIR}

)

set(SOURCES_CORE ${SRC_DIR}/*.cpp)

# # 生成library
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# 希望 "hello_static" 在输出时，不是"hello_static"，而是以"hello"的名字显示，故设置如下
#SET_TARGET_PROPERTIES (${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# 生成可执行文件
# add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES} )

# 链接操作
# target_link_libraries(${PROJECT_NAME}  event pthread)


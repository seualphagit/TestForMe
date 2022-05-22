cmake_minimum_required(VERSION 3.0)
project(xsd)

# 如果代码需要支持C++11，就直接加上这句
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
add_definitions("-Wall -g")

# 设置变量，下面的代码都可以用到
set(COM_CLIENT_DIR ${PROJECT_SOURCE_DIR}/ComClient)
set(COM_SERVER_DIR ${PROJECT_SOURCE_DIR}/ComServer)
set(CORE_DIR ${PROJECT_SOURCE_DIR}/Core)



aux_source_directory(${COM_CLIENT_DIR}/src COM_CLIENT_SRC_FILES)
aux_source_directory(${COM_SERVER_DIR}/src COM_SERVER_SRC_FILES)
aux_source_directory(${CORE_DIR}/src CORE_SRC_FILES)

# 编译子文件夹的CMakeLists.txt

# 规定.h头文件路径
include_directories(
    ${COM_CLIENT_DIR}/inc
    ${COM_SERVER_DIR}/inc
    ${CORE_DIR}/inc
)

# set(SOURCES_CLIENT src/*.cpp)

# # 生成library
add_library(${PROJECT_NAME} SHARED ${COM_CLIENT_SRC_FILES} ${COM_SERVER_SRC_FILES} ${CORE_SRC_FILES})


# 生成可执行文件
# add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES} )

# 链接操作
# target_link_libraries(${PROJECT_NAME}  event pthread xsd_com_client xsd_com_server xsd_core)


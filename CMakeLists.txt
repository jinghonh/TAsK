cmake_minimum_required(VERSION 3.10)
project(TAsK)

set(CMAKE_CXX_STANDARD 11)

# 添加Boost库
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# 添加源文件目录
include_directories(src)

# 收集所有源文件
file(GLOB SOURCES "src/*.cpp")

# 创建可执行文件
add_executable(ta ${SOURCES})
target_link_libraries(ta ${Boost_LIBRARIES})

# 使用长双精度 (可选)
option(USE_EXTENDED_PRECISION "Use extended precision" ON)
if(USE_EXTENDED_PRECISION)
    add_definitions(-DUSE_EXTENDED_PRECISION)
endif()
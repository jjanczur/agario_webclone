cmake_minimum_required(VERSION 3.8)
project(agario_webclone)

set(CMAKE_CXX_STANDARD 17)

include_directories(Server/beast/include)
include_directories(Server/json/include)

set(SOURCE_FILES Server/main.cpp)
add_executable(agario_webclone ${SOURCE_FILES})

target_link_libraries(agario_webclone boost_system)
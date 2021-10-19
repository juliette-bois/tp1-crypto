cmake_minimum_required(VERSION 3.20)
project(TP_1 C)

set(CMAKE_C_STANDARD 99)

set(I /usr/local/opt/openssl@1.1/include)
set(L /usr/local/opt/openssl@1.1/lib)

add_executable(TP_1 main.c)

target_link_libraries(TP_1 ssl crypto)
cmake_minimum_required(VERSION 3.23)
project(chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(chat
        main.cpp
        chat_handler.cpp
        chat_packet.cpp
        packet_helper.cpp
        server_helper.cpp
        client_helper.cpp
        )

target_link_libraries(chat ws2_32)

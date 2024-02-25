cmake_minimum_required(VERSION 3.5)

project(NetworkSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NetworkSimulator
    main.cpp
    settings.h
    LTE.h
    ue.h
    ue.cpp
)

install(TARGETS NetworkSimulator
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

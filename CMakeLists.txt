cmake_minimum_required(VERSION 3.26)
project(wpk_switcher)

set(CMAKE_CXX_STANDARD 23)

add_executable(wpk_switcher
        "wpk/Windows Product Key Switcher.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
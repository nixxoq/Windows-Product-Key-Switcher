cmake_minimum_required(VERSION 3.26)
project(WPK)

set(CMAKE_CXX_STANDARD 17)

add_executable(WPK "Windows Product Key Switcher.cpp")

add_subdirectory(fmt)
find_package(fmt)
target_link_libraries(WPK fmt::fmt)

set(CMAKE_BUILD_TYPE Release)
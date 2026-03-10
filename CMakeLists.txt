cmake_minimum_required(VERSION 3.28)

project(runic VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

add_subdirectory(src)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE runic)

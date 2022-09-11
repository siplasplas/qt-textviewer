cmake_minimum_required(VERSION 3.13)
project(gtest_viewer)

add_definitions("-Werror=return-type")

add_executable(${PROJECT_NAME}
        logic/ViewLogic.cpp logic/ViewLogic.h
        logic/Lines.cpp logic/Lines.h logic/Util.cpp logic/Util.h
        logic/test1.cpp logic/test2.cpp logic/test3.cpp logic/test4.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
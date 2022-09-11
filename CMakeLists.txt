cmake_minimum_required(VERSION 3.13)
project(gtest_viewer)

add_definitions("-Werror=return-type")

add_executable(${PROJECT_NAME}
        ViewLogic.cpp ViewLogic.h
        Lines.cpp Lines.h Util.cpp Util.h
        test1.cpp test2.cpp test3.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
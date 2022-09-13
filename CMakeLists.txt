cmake_minimum_required(VERSION 3.13)

project(textViewer)

add_definitions("-Werror=return-type")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(LOGIC_SOURCES logic/ViewLogic.cpp logic/ViewLogic.h
        logic/Lines.cpp logic/Lines.h logic/Util.cpp logic/Util.h)

set(PROJECT_SOURCES
        main.cpp
        dialog.cpp dialog.h
        wid/TextViewer.cpp wid/TextViewer.h wid/PaintArea.cpp wid/PaintArea.h
        FileMap.h FileMap.cpp
        ${LOGIC_SOURCES} )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
            res/icons.qrc
        ${PROJECT_SOURCES}
    )
else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
        )

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()


add_executable(gtest_viewer ${LOGIC_SOURCES}
        logic/test1.cpp logic/test2.cpp logic/test3.cpp logic/test4.cpp)

target_link_libraries(gtest_viewer gtest gtest_main)

add_executable(test test.cpp)
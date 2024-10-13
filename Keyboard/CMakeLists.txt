cmake_minimum_required(VERSION 3.5)

project(Keyboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# include 디렉토리 설정
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

# 프로젝트 소스 파일 정의
set(SOURCES
    src/main.cpp
    src/robot_arm.cpp
)

# 헤더 파일 정의
set(HEADERS
    include/robot_arm.h
)

# UI 파일 정의
set(UI_FILES
    ui/robot_arm.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Keyboard
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Keyboard SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Keyboard
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# X11 라이브러리를 타겟에 링크
target_link_libraries(Keyboard PRIVATE Qt${QT_VERSION_MAJOR}::Widgets X11)

set_target_properties(Keyboard PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Keyboard)
endif()

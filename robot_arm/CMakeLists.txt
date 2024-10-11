cmake_minimum_required(VERSION 3.5)

# 프로젝트 이름과 버전 설정
project(robot_arm VERSION 0.1 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 패키지 찾기 (Qt6 또는 Qt5 지원)
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Found Qt6: ${Qt6_VERSION}")
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_VERSION_MAJOR 5)
    message(STATUS "Found Qt5: ${Qt5_VERSION}")
endif()

if(NOT Qt5_FOUND AND NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt5 or Qt6 not found. Please install Qt.")
endif()

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

# MOC 파일 생성
if(QT_VERSION_MAJOR EQUAL 6)
    qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
    qt6_wrap_ui(UIC_SOURCES ${UI_FILES})
else()
    qt5_wrap_cpp(MOC_SOURCES ${HEADERS})
    qt5_wrap_ui(UIC_SOURCES ${UI_FILES})
endif()

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES} ${UIC_SOURCES})

# 적절한 Qt Widgets 라이브러리 링크
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
else()
    target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
endif()

# 플랫폼별 설정 (필요시 추가)
if(WIN32)
    # Windows-specific settings
elseif(UNIX)
    # Linux-specific settings
endif()

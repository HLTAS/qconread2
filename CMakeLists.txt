project(qconread2)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

set(COMMON_GCC_FLAGS "-pedantic -O3 -march=native -mtune=native")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_GCC_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded -Wno-switch-enum")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_GCC_FLAGS} -Wextra")
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
set(QT_LIBRARIES Qt5::Core Qt5::Widgets)

add_subdirectory(taslogger)

include_directories(
	src
	taslogger
	${RapidJSON_INCLUDE_DIR}
)

add_executable(qconread2
	src/logtablemodel.cpp
	src/main.cpp
	src/mainwindow.cpp
)

target_link_libraries(qconread2 taslogger ${QT_LIBRARIES})

cmake_minimum_required(VERSION 3.10)

project(main)

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/app)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES ${SOURCE_DIR}/task.c)

option(BUILD_SHARED_LIBS "Build with shared lib" OFF)

add_library(libtask ${SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(main ${APP_DIR}/main.c ${SOURCES})

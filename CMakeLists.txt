cmake_minimum_required(VERSION 3.5.0)

project(GetOptPlusPlusTests)
set(PROJECT_VERSION 1.0.0)
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

## Set Doctest options
set(DOCTEST_VERSION 1.2.5)
set(DOCTEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doctest)

file(MAKE_DIRECTORY ${DOCTEST_OUTPUT_DIR})
include(ExternalProject)

## Download Doctest
ExternalProject_Add(Doctest
 PREFIX            ${DOCTEST_OUTPUT_DIR}
 #SRC_DIR           ${DOCTEST_OUTPUT_DIR}/src
 #TMP_DIR           ${DOCTEST_OUTPUT_DIR}/temp
 #STAMP_DIR         ${DOCTEST_OUTPUT_DIR}/stamp
 #--Download step--------------
 GIT_REPOSITORY https://github.com/onqtam/doctest.git  
 GIT_TAG ${DOCTEST_VERSION}
 CONFIGURE_COMMAND ""
 BUILD_COMMAND ""
 INSTALL_COMMAND ""
)

enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${DOCTEST_OUTPUT_DIR}/src/Doctest/doctest/)
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/test/unit_doctest.cpp )

add_test(${PROJECT_NAME} ${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/getopt++.h)

add_dependencies(${PROJECT_NAME} Doctest)
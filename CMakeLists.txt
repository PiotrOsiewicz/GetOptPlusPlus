cmake_minimum_required(VERSION 3.5.0)

project(GetOptPlusPlusTests)
set(PROJECT_VERSION 1.0.0)

set(DOCTEST_VERSION 1.2.5)
set(DOCTEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doctest)

file(MAKE_DIRECTORY ${DOCTEST_OUTPUT_DIR})
include(ExternalProject)

## Download DocTest
ExternalProject_Add(Doctest
 PREFIX            ${DOCTEST_OUTPUT_DIR}
 #SRC_DIR           ${DOCTEST_OUTPUT_DIR}/src
 #TMP_DIR           ${DOCTEST_OUTPUT_DIR}/temp
 #STAMP_DIR         ${DOCTEST_OUTPUT_DIR}/stamp
 #--Download step--------------
 GIT_REPOSITORY https://github.com/onqtam/doctest.git  
 GIT_TAG ${DOCTEST_VERSION}
 CONFIGURE_COMMAND ""
)

include_directories(${DOCTEST_OUTPUT_DIR}/src/Doctest/doctest/)
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/test/unit_doctest.cpp )

target_sources(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/getopt++.h)

add_dependencies(${PROJECT_NAME} Doctest)
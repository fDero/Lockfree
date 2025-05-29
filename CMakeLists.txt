cmake_minimum_required(VERSION 3.22)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                      PROJECT FILE-STRUCTURE SETTINGS                                      #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

project(
  Lockfree
  DESCRIPTION "A collection of lock-free data structures in C++20"
  HOMEPAGE_URL https://github.com/fDero/Lockfree
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
  set(BUILD_SHARED_LIBS OFF) 
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)

set(TEST_BINARY lockfree_test)
file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                             DEPENDENCIES                                                 #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/dependencies/build/Release/generators ${CMAKE_PREFIX_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/dependencies/build/Release/generators ${CMAKE_MODULE_PATH})

find_package(Celero REQUIRED)
find_package(GTest REQUIRED)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                                LIBRARY                                                   #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

add_library(lockfree INTERFACE)
target_include_directories(lockfree INTERFACE include)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                                TESTING                                                   #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

add_executable(
  ${TEST_BINARY}
  ${TEST_FILES}
)

target_compile_options(
  ${TEST_BINARY}
  PRIVATE
  -O0
  -Wunused
  -Wall
  -Wpedantic
  -Wswitch
  -Wfloat-equal
)

target_link_libraries(
  ${TEST_BINARY}
  PRIVATE
  GTest::GTest
  GTest::Main
)

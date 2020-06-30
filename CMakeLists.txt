if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Create a separate build directory")
endif()

cmake_minimum_required (VERSION 3.12)

project(mcpat DESCRIPTION "Power Timing Area Calculator"
              LANGUAGES CXX)

find_package(Boost 1.56 REQUIRED COMPONENTS
             program_options
             serialization)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-value -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_PROFILE "-O3 -pg -g")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_definitions(-DNTHREADS=1)

add_executable(vs_classifier
               main.cc
               perceptron.h
               perceptron.cc
               array.h
               array.cc
               layer.h
               layer.cc
               func.h
               func.cc
               dnn.h
               dnn.cc
               utility.h
               utility.cc
               options.h
               options.cc)

target_link_libraries(vs_classifier
                      LINK_PUBLIC
                      Threads::Threads
                      Boost::program_options
                      Boost::serialization)

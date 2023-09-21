cmake_minimum_required(VERSION 3.5.1)
set(CONAN_DISABLE_CHECK_COMPILER "true")
set(CMAKE_CXX_STANDARD 14)
project(jmsg_generator)

file(GLOB_RECURSE JMSG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c**)
file(GLOB_RECURSE GENERATOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/generator/*.c**)

add_executable(jmsg_generator ${JMSG_SOURCES}
${GENERATOR_SOURCES})

target_include_directories(jmsg_generator PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include
${CMAKE_CURRENT_SOURCE_DIR}/src)
cmake_minimum_required(VERSION 3.18)
project("${SKBUILD_PROJECT_NAME}" LANGUAGES CXX VERSION ${SKBUILD_PROJECT_VERSION_FULL})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Python 3.8 COMPONENTS Interpreter Development.Module ${SKBUILD_SABI_COMPONENT} REQUIRED)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/nanobind")

nanobind_add_module(nbexample_ext
  STABLE_ABI
  NB_STATIC
  src/nbexample_ext.cpp
)

install(TARGETS nbexample_ext LIBRARY DESTINATION nbexample)

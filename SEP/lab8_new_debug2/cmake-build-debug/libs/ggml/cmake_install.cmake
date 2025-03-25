# Install script for directory: D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/SmartServing")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/96317/CLion 2023.3.4/bin/mingw/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/cmake-build-debug/libs/ggml/src/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/cmake-build-debug/libs/ggml/src/libggml.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-alloc.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-backend.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-blas.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-cann.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-cuda.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-kompute.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-metal.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-rpc.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-sycl.h"
    "D:/Haoyu_Liu/homeworks/SEP/lab8_new_debug2/libs/ggml/include/ggml-vulkan.h"
    )
endif()


# gaia [![Build and test all platforms](https://github.com/aretecarpe/gaia/actions/workflows/main.yml/badge.svg)](https://github.com/aretecarpe/gaia/actions/workflows/main.yml)

A Portable, C++ 17 header-only library for abstracting system-level calls behind a POSIX compliant API

## Introduction

Gaia provides a POSIX compliant API for named and unnamed semaphores, shared_libraries, signal_handlers, etc.

## Examples

The [examples](./examples/) folder contains plenty of examples showcasing features of gaia, such as [semaphore](./examples/semaphore/semaphore.cpp), [shared_library](./examples/shared_library/shared_library.cpp), [signal_watcher](./examples/signal_watcher/signal_watcher.cpp).

## Using gaia in your project

### cmake

If your project uses CMake, then after cloning 'gaia' and generating the installed artifacts, simply add the following to your 'CMakeLists.txt': 

```
find_package(gaia REQUIRED)

target_link_libraries(my_project INTERFACE gaia::gaia)
```

This will both find the package as well as link the header-only library.


### CMake Package Manager (CPM)

TO further simplify obtaining and including 'gaia' in your CMake project, it is recommneded to use the [CMake Pacakge Manager (CPM)](https://github.com/cpm-cmake/CPM.cmake) to fetch and configure 'gaia'.

Complete example:

```
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(genesisExample)

# Get CPM
# For more information on how to add CPM to your project, see: https://github.com/cpm-cmake/CPM.cmake#adding-cpm
include(CPM.cmake)

CPMAddPackage(
  NAME gaia
  GITHUB_REPOSITORY aretecarpe/gaia
  GIT_TAG master # This will always pull the latest code from the `master` branch. You may also use a specific release version or tag
  OPTIONS
	"BUILD_TESTING OFF"
	"BUILD_EXAMPLES OFF"
)

add_executable(main example.cpp)

target_link_libraries(main gaia::gaia)
```

## Building gaia

Below is the minimal instructions needed to build and install gaia.

To build gaia (examples and tests are enabled by default)

```shell
$ cmake -B build -S .
$ cmake --build build
```

To build gaia without examples or tests

```shell
$ cmake -B build -S . -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF
$ cmake --build build
```

To install gaia

```shell
$ cmake --install build
```

This will install gaia in a folder called 'installed_artifacts' and should be ready for ingestion into your project as a cmake package.
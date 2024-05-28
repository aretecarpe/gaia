file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.39.0/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=66639bcac9dd2907b2918de466783554c1334446b9874e90d38e3778d404c2ef
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage(
  NAME genesis
  GITHUB_REPOSITORY aretecarpe/genesis
  GIT_TAG fix-errno-ms # This will always pull the latest code from the `master` branch. You may also use a specific release version or tag
  OPTIONS
	"BUILD_TESTING OFF"
	"BUILD_EXAMPLES OFF"
)
CPMAddPackage("gh:catchorg/Catch2@3.6.0")
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

cmake_minimum_required(VERSION 3.21)

set(CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake") # we are going to assume the cmake is here.

include("${CMAKE_PATH}/project-template.cmake")

project(${PROJECT_NAME}
	VERSION ${PROJECT_VERSION}
	DESCRIPTION ${PROJECT_DESCRIPTION}
	LANGUAGES CXX
)

foreach(product_path ${PRODUCT_LIST})
	set(PRODUCT_SUBDIRECTORY ${product_path})
	add_subdirectory(${product_path})
endforeach()
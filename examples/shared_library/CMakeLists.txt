if (BUILD_EXAMPLES)
	add_library(dummy_lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/dummy_lib.cpp)
	set_target_properties(dummy_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
	target_link_libraries(dummy_lib PUBLIC gaia::gaia genesis::genesis)

	include("${CMAKE_PATH}/product-template.cmake")

	find_package(genesis 0.0.1 EXACT REQUIRED)

	target_link_libraries(${PRODUCT_NAME} PUBLIC gaia::gaia genesis::genesis)
endif()
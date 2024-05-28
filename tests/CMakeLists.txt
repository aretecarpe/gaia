if (BUILD_TESTING)
	CPMAddPackage("gh:catchorg/Catch2@3.6.0")
	list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
	include(CTest)
	include(Catch)

	add_library(gaia_shared_lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/common/shared_lib.cpp)
	set_target_properties(gaia_shared_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
	target_link_libraries(gaia_shared_lib PUBLIC gaia::gaia genesis::genesis)

	enable_testing()
	include("${CMAKE_PATH}/product-template.cmake")

	find_package(genesis 0.0.1 REQUIRED)

	target_link_libraries(${PRODUCT_NAME} PUBLIC gaia::gaia genesis::genesis Catch2::Catch2WithMain)

	add_test(NAME ${PRODUCT_NAME} COMMAND ${PRODUCT_NAME})
	enable_testing()
endif()
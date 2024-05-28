include("${CMAKE_PATH}/product-template.cmake")

find_package(genesis 0.0.1 EXACT REQUIRED)

target_link_libraries(${PRODUCT_NAME} PUBLIC gaia genesis::genesis)
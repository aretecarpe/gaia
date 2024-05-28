#include "gaia/shared_library.hpp"
#include "gaia/filesystem_helpers.hpp"

#include <catch2/catch_all.hpp>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

TEST_CASE("shared_library constructor", "[shared_library][constructor]") {
	#if GENESIS_MICROSOFT
	auto path = gaia::find_file_recursively("gaia_shared_lib.dll");
	#else
	auto path = gaia::find_file_recursively("libgaia_shared_lib.so");
	#endif
	REQUIRE(path != std::nullopt);
	REQUIRE_NOTHROW(gaia::shared_library{*path});
	REQUIRE_THROWS(gaia::shared_library{"foobar"});
}
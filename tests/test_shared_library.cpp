#include "gaia/shared_library.hpp"
#include "gaia/filesystem_helpers.hpp"

#include <catch2/catch_all.hpp>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

inline std::optional<fs::path> slib() {
	#if GENESIS_MICROSOFT
	return gaia::find_file_recursively("gaia_shared_lib.dll");
	#else
	return gaia::find_file_recursively("libgaia_shared_lib.so");
	#endif
}

TEST_CASE("shared_library constructor", "[shared_library][constructor]") {
	REQUIRE_NOTHROW(gaia::shared_library{*slib()});
	REQUIRE_THROWS(gaia::shared_library{"foobar"});
}

TEST_CASE("shared_library get symbol and call", "[shared_library][symbol]") {
	gaia::shared_library foo_lib{*slib()};
	using function_ptr = int(*)();
	auto f_ptr = foo_lib.get<function_ptr>("foobar");
	REQUIRE(f_ptr != nullptr);
	REQUIRE(f_ptr() == 42);
}
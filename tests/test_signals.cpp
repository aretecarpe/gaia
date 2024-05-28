#include "gaia/signal_watcher.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("signal_watcher simple sigint", "[signal_watcher][sigint]") {
	REQUIRE(gaia::termination_requested() == false);
	std::raise(static_cast<int>(gaia::posix_signal::sigint));
	REQUIRE(gaia::termination_requested() == true);
}

// This won't work, need to have signal registration in a guard class... 
// It will think termination has already been requested.
TEST_CASE("signal_watcher simple sigterm", "[signal_watcher][sigterm]") {
	// REQUIRE(gaia::termination_requested() == false);
	// std::raise(static_cast<int>(gaia::posix_signal::sigint));
	// REQUIRE(gaia::termination_requested() == true);
}
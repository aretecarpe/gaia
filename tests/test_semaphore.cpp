// #include "gaia/semaphore.hpp"

// #include <catch2/catch_all.hpp>
// #include <catch2/matchers/catch_matchers_floating_point.hpp>

// #include <chrono>
// #include <thread>
// #include <string_view>

// namespace details {

// constexpr std::string_view sem_name{"semmy"};

// } // end namespace details

// TEST_CASE("basic semaphore construction", "[semaphore][basic]") {
// 	REQUIRE_NOTHROW(gaia::semaphore{});
// }

// TEST_CASE("basic_semaphore acquire nothrow", "[semaphore][basic]") {
// 	gaia::basic_semaphore<1> sem{};
// 	std::thread task {
// 		[&sem] {
// 			REQUIRE_NOTHROW(sem.acquire());
// 		}
// 	};
// 	std::this_thread::sleep_for(std::chrono::milliseconds{10});
// 	sem.release();
// 	task.join();
// }

// TEST_CASE("basic_semaphore release nothrow", "[semaphore][basic]") {
// 	gaia::basic_semaphore<1> sem{};
// 	std::thread task {
// 		[&sem] {
// 			sem.acquire();
// 		}
// 	};
// 	std::this_thread::sleep_for(std::chrono::milliseconds{10});
// 	REQUIRE_NOTHROW(sem.release());
// 	task.join();
// }

// TEST_CASE("basic_semaphore acquire with ec", "[semaphore][basic]") {
// 	std::error_code ec{};
// 	gaia::basic_semaphore<1> sem{};
// 	std::thread task {
// 		[&sem, &ec] {
// 			sem.acquire(ec);
// 		}
// 	};
// 	sem.release();
// 	task.join();
// 	REQUIRE(ec.value() == 0);
// }

// TEST_CASE("basic_semaphore release with ec", "[semaphore][basic]") {
// 	std::error_code ec{};
// 	gaia::basic_semaphore<1> sem{};
// 	std::thread task {
// 		[&sem] {
// 			sem.acquire();
// 		}
// 	};
// 	sem.release(ec);
// 	task.join();
// 	REQUIRE(ec.value() == 0);
// }

// TEST_CASE("basic_semaphore acquire and release", "[semaphore][basic]") {
// 	gaia::basic_semaphore<1> sem{};
// 	int32_t count{0};
// 	std::thread task{
// 		[&sem, &count] {
// 			sem.acquire();
// 			std::this_thread::sleep_for(std::chrono::milliseconds{100});
// 			count += 5;
// 			sem.release();
// 		}
// 	};
// 	std::this_thread::sleep_for(std::chrono::milliseconds{10});
// 	sem.release();
// 	std::this_thread::sleep_for(std::chrono::milliseconds{10});
// 	sem.acquire();
// 	sem.release();
// 	sem.acquire();
// 	count += 5;
// 	sem.release();
// 	task.join();
// 	REQUIRE(10 == count);
// }

// TEST_CASE("basic_semaphore try_acquire", "[semaphore][basic]") {
// 	gaia::basic_semaphore<1> sem{};
// 	constexpr auto default_duration = std::chrono::milliseconds{500};
// 	auto start_time = std::chrono::high_resolution_clock::now();
// 	sem.try_acquire_for(default_duration);
// 	auto stop_time = std::chrono::high_resolution_clock::now();
// 	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);
// 	REQUIRE(elapsed_time.count() >= (default_duration.count())); // prob a bad check on machines with a lot going on.
// }

// TEST_CASE("named_semaphore create_only construction", "[semapohre][named]") {
// 	REQUIRE_NOTHROW(gaia::named_semaphore{gaia::create_only, std::string{details::sem_name}, 0});
// 	gaia::named_semaphore::remove(std::string{details::sem_name});
// }

// TEST_CASE("named_semaphore open_only construction", "[semaphore][named]") {
// 	gaia::named_semaphore sem{gaia::create_only, std::string{details::sem_name}, 0};
// 	REQUIRE_NOTHROW(gaia::named_semaphore{gaia::open_only, std::string{details::sem_name}});
// 	sem.remove(sem.name());
// }

// TEST_CASE("named_semaphore open_or_create create construction", "[semaphore][named]") {
// 	REQUIRE_NOTHROW(gaia::named_semaphore{gaia::open_or_create, std::string{details::sem_name}, 0});
// 	gaia::named_semaphore::remove(std::string{details::sem_name});
// }

// TEST_CASE("named_semaphore open_or_create open construction", "[semapohre][named]") {
// 	gaia::named_semaphore sem{gaia::create_only, std::string{details::sem_name}, 0};
// 	REQUIRE_NOTHROW(gaia::named_semaphore{gaia::open_or_create, std::string{details::sem_name}, 0});
// 	sem.remove(sem.name());
// }
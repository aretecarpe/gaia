// #include "gaia/signal_watcher.hpp"

// #include <genesis/stop_token.hpp>

// #include <chrono>
// #include <thread>
// #include <iostream>
// #include <filesystem>

// namespace fs = std::filesystem;

// struct object {
// 	object() noexcept = default;

// 	~object() {
// 		std::cout << "Cleaned up resource" << std::endl;
// 	}
// };

int main(int argc, char** argv) {
	// object ob{};
	// genesis::stop_source source{};
	// std::thread task_one{
	// 	[&source] {
	// 		auto token = source.get_token();
	// 		while (!token.stop_requested()) {
	// 			std::printf("Hi from thread one\n");
	// 			std::this_thread::sleep_for(std::chrono::milliseconds{100});
	// 		}
	// 	}
	// };
	// std::thread task_two{
	// 	[&source] {
	// 		auto token = source.get_token();
	// 		while (!token.stop_requested()) {
	// 			std::printf("Hi from thread two\n");
	// 			std::this_thread::sleep_for(std::chrono::milliseconds{50});
	// 		}
	// 	}
	// };
	// while (!gaia::termination_requested()) { 
	// 	std::this_thread::sleep_for(std::chrono::milliseconds{1000});
	// }
	// source.request_stop();
	// task_one.join();
	// task_two.join();
}
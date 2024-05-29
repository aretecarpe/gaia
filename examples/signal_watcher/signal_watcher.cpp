#include <gaia/signal_watcher.hpp>

#include <iostream>

int main(int argc, char** argv) {
	std::thread t1{[] {
		while (!gaia::termination_requested()) {
			std::printf("Hi from thread one\n");
			std::this_thread::sleep_for(std::chrono::milliseconds{42});
		}
		std::printf("Bye from thread one\n"); 
	}};
	std::thread t2{[] {
		while (!gaia::termination_requested()) {
			std::printf("Hi from thread two\n");
			std::this_thread::sleep_for(std::chrono::milliseconds{42});
		}
		std::printf("Bye from thread two\n");
	}};

	t1.join();
	t2.join();
	std::printf("Done...\n");
	return 0;
}
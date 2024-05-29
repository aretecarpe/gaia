#include <gaia/semaphore.hpp>

#include <cstdint>
#include <chrono>
#include <thread>
#include <iostream>
#include <queue>
#include <mutex>

constexpr uint32_t buffer_size{5};
constexpr uint32_t num_producers{3};
constexpr uint32_t num_consumers{num_producers};
constexpr uint32_t items_per_producer{5};

gaia::semaphore empty_slots{buffer_size};
gaia::semaphore filled_slots{0};
std::mutex mutex{};
std::queue<uint32_t> queue{};

void producer(uint32_t id) {
	for (std::size_t i = 0; i < items_per_producer; ++i) {
		empty_slots.acquire();
		{
			std::lock_guard lock{mutex};
			queue.push(id * items_per_producer + i);
			std::cout << "Producer: "	 << id << " produced item " << id * items_per_producer + i << std::endl;
		}
		filled_slots.release();
		std::this_thread::sleep_for(std::chrono::milliseconds{100});
	}
}

void consumer(uint32_t id) {
	for (int i = 0; i < (num_producers * items_per_producer) / num_consumers; ++i) {
		filled_slots.acquire();
		
		int item;
		{
			std::lock_guard<std::mutex> lock(mutex);
			item = queue.front();
			queue.pop();
			std::cout << "Consumer " << id << " consumed item " << item << std::endl;
		}
		
		empty_slots.release();
		std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Simulate work
	}
}

int main() {
	std::vector<std::thread> producers, consumers;

	for (int i = 0; i < num_producers; ++i) {
		producers.emplace_back(producer, i);
	}

	for (int i = 0; i < num_consumers; ++i) {
		consumers.emplace_back(consumer, i);
	}

	for (auto& producer : producers) {
		producer.join();
	}

	for (auto& consumer : consumers) {
		consumer.join();
	}

	return 0;
}
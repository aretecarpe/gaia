#if !defined GENESIS_SIGNAL_WATCHER_HEADER_INCLUDED
#define GENESIS_SIGNAL_WATCHER_HEADER_INCLUDED
#pragma once

#include "gaia/signal_handler.hpp"
#include "gaia/semaphore.hpp"

#include <atomic>
#include <cstdint>
#include <optional>

namespace gaia {

void internal_signal_handler(int) noexcept;

class signal_watcher {
private:
	std::atomic_bool signal_occured_;
	mutable semaphore sem_;

protected:
	signal_watcher() noexcept :
		signal_occured_{false},
		sem_{}
	{
		register_signal(posix_signal::sigint, internal_signal_handler);
		register_signal(posix_signal::sigterm, internal_signal_handler);
	}

public:
	signal_watcher(const signal_watcher&) = delete;
	
	signal_watcher(signal_watcher&&) = delete;
	
	signal_watcher& operator=(const signal_watcher&) = delete;

	signal_watcher& operator=(signal_watcher&&) = delete;

	~signal_watcher() = default;

	static signal_watcher& instance() noexcept {
		static signal_watcher instance;
		return instance;
	}

	void wait_for_signal() const noexcept {
		if (signal_occured_.load()) {
			return; 
		}
		sem_.acquire();
	}

	bool was_signal_triggered() const noexcept {
		return signal_occured_.load();
	}

private:
	friend void internal_signal_handler(int) noexcept;
};

inline void internal_signal_handler(int) noexcept {
	auto& instance = signal_watcher::instance();
	instance.signal_occured_.store(true);
	instance.sem_.release();
}

inline void wait_for_termination_request() noexcept {
	signal_watcher::instance().wait_for_signal();
}

inline bool termination_requested() noexcept {
	return signal_watcher::instance().was_signal_triggered();
}

} // end namespace gaia

#endif
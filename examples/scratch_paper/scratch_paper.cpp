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

// #include "gaia/semaphore.hpp"
// #include <windows.h>

#include "gaia/tags.hpp"

#include <genesis/config.hpp>
#include <genesis/errno.hpp>

#include <system_error>
#include <chrono>
#include <cstdint>
#include <limits>
#include <string>
#include <thread>
#include <utility>

#if GENESIS_POSIX
#include <semaphore.h>
#endif

#if GENESIS_MICROSOFT
#undef max
#endif

namespace gaia {

class semaphore_base {
public:
#if GENESIS_POSIX
	using native_handle_type = sem_t*;
	static constexpr native_handle_type invalid_handle = nullptr;
#else
	using native_handle_type = intptr_t;
	static constexpr native_handle_type invalid_handle = 0;
#endif

	using duration_type = std::chrono::nanoseconds;

protected:
	static void release(native_handle_type);

	static void release(native_handle_type, std::error_code&) noexcept;

	static void release(native_handle_type, ptrdiff_t);

	static void release(native_handle_type, ptrdiff_t, std::error_code&) noexcept;

	static void acquire(native_handle_type);

	static void acquire(native_handle_type, std::error_code&) noexcept;

	static auto try_acquire(native_handle_type) -> bool;

	static auto try_acquire(native_handle_type, std::error_code&) noexcept -> bool;

	static auto try_acquire_for(native_handle_type, duration_type) -> bool;

	static auto try_acquire_for(native_handle_type, duration_type, std::error_code&) noexcept -> bool;
};

class unnamed_semaphore_base : public semaphore_base {
protected:
// #if GENESIS_POSIX
// 	mutable sem_t handle_;
// #else
	native_handle_type handle_;
// #endif

public:
	unnamed_semaphore_base(ptrdiff_t init_count);

	unnamed_semaphore_base(const unnamed_semaphore_base&) = delete;

	unnamed_semaphore_base(unnamed_semaphore_base&& other) = delete;

	unnamed_semaphore_base& operator=(const unnamed_semaphore_base&) = delete;

	unnamed_semaphore_base& operator=(unnamed_semaphore_base&&) = delete;

	~unnamed_semaphore_base();

	void release();

	void release(std::error_code& ec) noexcept;

	void release(ptrdiff_t count);

	void release(ptrdiff_t count, std::error_code& ec) noexcept;

	void acquire();

	void acquire(std::error_code& ec) noexcept;

	bool try_acquire();

	bool try_acquire(std::error_code& ec) noexcept;

	template <typename Rep, typename Period>
	bool try_acquire_for(const std::chrono::duration<Rep, Period>& duration) {
		return try_acquire_for_ns(std::chrono::duration_cast<duration_type>(duration));
	}

	template <typename Rep, typename Period>
	bool try_acquire_for(const std::chrono::duration<Rep, Period>& duration, std::error_code& ec) noexcept {
		return try_acquire_for_ns(std::chrono::duration_cast<duration_type>(duration), ec);
	}

	native_handle_type native_handle() const noexcept;

private:
	bool try_acquire_for_ns(duration_type duration);

	bool try_acquire_for_ns(duration_type duration, std::error_code& ec) noexcept;
};

class named_semaphore_base : public semaphore_base {
private:	
	std::string sem_name_;

protected:
	native_handle_type handle_;

	named_semaphore_base(gaia::create_t, const std::string& sem_name, ptrdiff_t init_count, ptrdiff_t init_max);

	named_semaphore_base(gaia::open_or_create_t, const std::string& sem_name, ptrdiff_t init_count, ptrdiff_t init_max);

	named_semaphore_base(gaia::open_t, const std::string& name);

	named_semaphore_base(const named_semaphore_base&) = delete;

	named_semaphore_base& operator=(const named_semaphore_base&) = delete;

	named_semaphore_base(named_semaphore_base&& other) noexcept :
		handle_{std::exchange(other.handle_, semaphore_base::invalid_handle)}	
	{ }

	named_semaphore_base& operator=(named_semaphore_base&& other) noexcept {
		if (this != &other) {
			handle_ = std::exchange(other.handle_, semaphore_base::invalid_handle);
		}
		return *this;
	}

	~named_semaphore_base();

public:
	const std::string& name() const noexcept { return sem_name_; }

	void release() { semaphore_base::release(handle_); }

	void release(ptrdiff_t count) { semaphore_base::release(handle_, count); }

	void acquire() { semaphore_base::acquire(handle_); }

	bool try_acquire() { return semaphore_base::try_acquire(handle_); }

	template <typename Rep, typename Period>
	bool try_acquire_for(const std::chrono::duration<Rep, Period>& duration){
		return semaphore_base::try_acquire_for(handle_, std::chrono::duration_cast<duration_type>(duration));
	}

	native_handle_type native_handle() const noexcept { return handle_; }

	static void remove(const std::string& name);
};

template <ptrdiff_t least_max_value>
class basic_semaphore : public unnamed_semaphore_base {
public:
	explicit basic_semaphore(ptrdiff_t init_count = 0) :
		unnamed_semaphore_base{init_count}
	{ }

	basic_semaphore(const basic_semaphore&) = delete;

	basic_semaphore& operator=(const basic_semaphore&) = delete;

	basic_semaphore(basic_semaphore&&) = default;

	basic_semaphore& operator=(basic_semaphore&&) = default;
};

using binary_semaphore = basic_semaphore<1>;
using semaphore = basic_semaphore<std::numeric_limits<uint16_t>::max()>;

template <ptrdiff_t least_max_value>
class basic_named_semaphore : public named_semaphore_base {
public:
	basic_named_semaphore(gaia::create_t, const std::string& name, ptrdiff_t init_count) :
		named_semaphore_base{gaia::create_only, name, init_count, least_max_value}
	{ }

	basic_named_semaphore(gaia::open_or_create_t, const std::string& name, ptrdiff_t init_count) :
		named_semaphore_base{gaia::open_or_create, name, init_count, least_max_value}
	{ }

	basic_named_semaphore(gaia::open_t, const std::string& name) :
		named_semaphore_base{gaia::open_only, name}
	{ }

	basic_named_semaphore(basic_named_semaphore&&) noexcept = default;

	basic_named_semaphore& operator=(basic_named_semaphore&& other) noexcept = default;
};

using named_binary_semaphore = basic_named_semaphore<1>;
using named_semaphore = basic_named_semaphore<std::numeric_limits<uint16_t>::max()>;

} // end namespace gaia

int main(int argc, char** argv) {
	// LONG previous;
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
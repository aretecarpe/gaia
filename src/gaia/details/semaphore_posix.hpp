#if !defined GAIA_SEMAPHORE_POSIX_HEADER_INCLUDED
#define GAIA_SEMAPHORE_POSIX_HEADER_INCLUDED
#pragma once

#include <genesis/config.hpp>

#if GENESIS_POSIX

#include "gaia/semaphore.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#if not defined __aarch64__
#include <sys/io.h>
#endif
#include <sys/types.h>

#include <system_error>

namespace gaia {	

inline void semaphore_base::release(semaphore_base::native_handle_type handle) {
	if (sem_post(handle) != 0) {
		genesis::throw_last_error();
	}
}

inline void semaphore_base::release(semaphore_base::native_handle_type handle, std::error_code& ec) noexcept {
	if (sem_post(handle) !=  0) {
		ec = genesis::get_last_error();
	}
}

inline void semaphore_base::release(semaphore_base::native_handle_type handle, ptrdiff_t count) {
	for (; count > 0; --count) {
		if (sem_post(handle) !=0) { genesis::throw_last_error(); }
	}
}

inline void semaphore_base::release(semaphore_base::native_handle_type handle, ptrdiff_t count, std::error_code& ec) noexcept {
	for (; count > 0; --count) {
		if (sem_post(handle) != 0) { ec = genesis::get_last_error(); }
	}
}

inline void semaphore_base::acquire(native_handle_type handle) {
	if (sem_wait(handle) != 0) { genesis::throw_last_error(); }
}

inline void semaphore_base::acquire(native_handle_type handle, std::error_code& ec) noexcept {
	if (sem_wait(handle) != 0) { ec = genesis::get_last_error(); }
}

inline auto semaphore_base::try_acquire(native_handle_type handle) -> bool {
	if (sem_trywait(handle) == 0) { return true; }
	if (errno == EAGAIN) { return false; }
	genesis::throw_last_error();
}

inline auto semaphore_base::try_acquire(native_handle_type handle, std::error_code& ec) noexcept -> bool {
	if (sem_trywait(handle) == 0) { return true; }
	if (errno == EAGAIN) { return false; }
	ec = genesis::get_last_error();
	return false;
}

inline auto semaphore_base::try_acquire_for(native_handle_type handle, duration_type duration) -> bool {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_nsec += duration.count();
	ts.tv_sec += ts.tv_nsec / 1'000'000'000ul;
	ts.tv_nsec %= 1'000'000'000ul;
	errno = 0;
	if (sem_timedwait(handle, &ts) == 0) { return true; }
	if (errno == ETIMEDOUT) { return false; }
	genesis::throw_last_error();
}

inline auto semaphore_base::try_acquire_for(native_handle_type handle, duration_type duration, std::error_code& ec) noexcept -> bool {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_nsec += duration.count();
	ts.tv_sec += ts.tv_nsec / 1'000'000'000ul;
	ts.tv_nsec %= 1'000'000'000ul;
	errno = 0;
	if (sem_timedwait(handle, &ts) == 0) { return true; }
	if (errno == ETIMEDOUT) { return false; }
	ec = genesis::get_last_error();
	return false;
}

inline unnamed_semaphore_base::unnamed_semaphore_base(ptrdiff_t init_count) {
	if (sem_init(&handle_, 1, static_cast<unsigned int>(init_count)) < 0) { genesis::throw_last_error(); }
}

inline unnamed_semaphore_base::~unnamed_semaphore_base() {
	sem_close(&handle_);
}

inline void unnamed_semaphore_base::release() {
	semaphore_base::release(&handle_);
}

inline void unnamed_semaphore_base::release(std::error_code& ec) noexcept {
	semaphore_base::release(&handle_, ec);
}

inline void unnamed_semaphore_base::release(ptrdiff_t count) {
	semaphore_base::release(&handle_, count);
}

inline void unnamed_semaphore_base::release(ptrdiff_t count, std::error_code& ec) noexcept {
	semaphore_base::release(&handle_, count, ec);
}

inline void unnamed_semaphore_base::acquire() {
	semaphore_base::acquire(&handle_);
}

inline void unnamed_semaphore_base::acquire(std::error_code& ec) noexcept {
	semaphore_base::acquire(&handle_, ec);
}

inline bool unnamed_semaphore_base::try_acquire() {
	return semaphore_base::try_acquire(&handle_);
}

inline bool unnamed_semaphore_base::try_acquire(std::error_code& ec) noexcept {
	return semaphore_base::try_acquire(&handle_, ec);
}

inline bool unnamed_semaphore_base::try_acquire_for_ns(duration_type duration) {
	return semaphore_base::try_acquire_for(&handle_, duration);
}

inline bool unnamed_semaphore_base::try_acquire_for_ns(duration_type duration, std::error_code& ec) noexcept {
	return semaphore_base::try_acquire_for(&handle_, duration, ec);
}

inline semaphore_base::native_handle_type unnamed_semaphore_base::native_handle() const noexcept {
	return &handle_;
}


inline named_semaphore_base::named_semaphore_base(gaia::create_t, const std::string& name, ptrdiff_t init_count, ptrdiff_t init_max) :
	sem_name_{name},
	handle_{
		reinterpret_cast<native_handle_type>(sem_open(
			sem_name_.c_str(), O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, static_cast<unsigned int>(init_count)
			)
		)
	}
{
	if (handle_ == SEM_FAILED) { genesis::throw_last_error(); }
}

inline named_semaphore_base::named_semaphore_base(gaia::open_or_create_t, const std::string& name, ptrdiff_t init_count, ptrdiff_t init_max) :
	sem_name_{name},
	handle_{
		reinterpret_cast<native_handle_type>(sem_open(
			sem_name_.c_str(), O_CREAT, S_IREAD | S_IWRITE, static_cast<unsigned int>(init_count)
			)
		)
	}
{
	if (handle_ == SEM_FAILED) { genesis::throw_last_error(); }
}

inline named_semaphore_base::named_semaphore_base(gaia::open_t, const std::string& name) :
	sem_name_{name},
	handle_{sem_open(name.c_str(), 0)}
{
	if (handle_ == SEM_FAILED) { genesis::throw_last_error(); }
}

inline named_semaphore_base::~named_semaphore_base() {
	if (handle_ != SEM_FAILED) {
		sem_close(handle_);
	}
}

inline void named_semaphore_base::remove(const std::string& name) {
	sem_unlink(name.c_str());
}

} // end namespace genesis

#endif

#endif // GAIA_SEMAPHORE_POSIX_HEADER_INCLUDED
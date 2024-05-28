#if !defined GENESIS_SEMAPHORE_WINDOWS_HEADER_INCLUDED
#define GENESIS_SEMAPHORE_WINDOWS_HEADER_INCLUDED
#pragma once

#include <genesis/config.hpp>

#include "gaia/semaphore.hpp"

#include <system_error>

#include <windows.h>

namespace gaia {

namespace {

inline void check(semaphore_base::native_handle_type handle) {
	if (handle == semaphore_base::invalid_handle) { genesis::throw_last_error(); }
}

} // end annon namespace

inline void semaphore_base::release(semaphore_base::native_handle_type handle) {
	LONG previous;
	if (!ReleaseSemaphore(reinterpret_cast<HANDLE>(handle), 1, &previous)) {
		genesis::throw_last_error();
	}
}

inline void semaphore_base::release(semaphore_base::native_handle_type handle, std::error_code& ec) {
	LONG previous;
	if (!ReleaseSemaphore(reinterpret_cast<HANDLE>(handle), 1, &previous)) {
		ec = genesis::get_last_error();
	}
}

inline void semaphore_base::release(semaphore_base::native_handle_type handle, ptrdiff_t count) {
	LONG previous;
	if (!ReleaseSemaphore(reinterpret_cast<HANDLE>(handle), static_cast<LONG>(count), &previous)) {
		genesis::throw_last_error();
	}
}

inline void semaphore_base::release(semaphore_base::native_handle_type handle, ptrdiff_t count, std::error_code& ec) noexcept {
	LONG previous;
	if (!ReleaseSemaphore(reinterpret_cast<HANDLE>(handle), static_cast<LONG>(count), &previous)) {
		ec = genesis::get_last_error();
	}
}

inline void semaphore_base::acquire(semaphore_base::native_handle_type handle) {
	WaitForSingleObject(reinterpret_cast<HANDLE>(handle), INFINITE);
}

inline void semaphore_base::acquire(native_handle_type handle, std::error_code& ec) noexcept {
	if (!WaitForSingleObject(reinterpret_cast<HANDLE>(handle), INFINITE)) {
		ec = genesis::get_last_error();
	}
}

inline bool semaphore_base::try_acquire(semaphore_base::native_handle_type handle) {
	return WaitForSingleObject(reinterpret_cast<HANDLE>(handle), 0L) == WAIT_OBJECT_0;
}

inline auto semaphore_base::try_acquire(native_handle_type handle, std::error_code& ec) noexcept -> bool {
	auto res = WaitForSingleObject(reinterpret_cast<HANDLE>(handle), 0L);
	if (res == WAIT_OBJECT_0) {
		return true;
	}
	if (res != WAIT_OBJECT_0) {
		ec = genesis::get_last_error();
		return false;
	}
	return false;
}

inline bool semaphore_base::try_acquire_for(semaphore_base::native_handle_type handle, duration_type duration) {
	// MS time is in milliseconds (1000000 nanoseconds)
	const auto ms = duration.count() / 1000000u;
	switch (WaitForSingleObject(reinterpret_cast<HANDLE>(handle), static_cast<DWORD>(ms))) {
	case WAIT_OBJECT_0: return true;
	case WAIT_TIMEOUT: return false;
	case WAIT_ABANDONED: throw std::system_error{static_cast<int>(std::errc::operation_not_permitted), std::generic_category()};
	default: genesis::throw_last_error();
	}
}

inline auto semaphore_base::try_acquire_for(native_handle_type handle, duration_type duration, std::error_code& ec) noexcept -> bool {
	// MS time is in milliseconds (1000000 nanoseconds)
	const auto ms = duration.count() / 1000000u;
	switch (WaitForSingleObject(reinterpret_cast<HANDLE>(handle), static_cast<DWORD>(ms))) {
	case WAIT_OBJECT_0: return true;
	case WAIT_TIMEOUT: return false;
	case WAIT_ABANDONED: ec = genesis::get_last_error(); return false;
	default: ec = genesis::get_last_error(); return false;
	}
}

// unnamed semaphore functions
inline unnamed_semaphore_base::unnamed_semaphore_base(ptrdiff_t init_count) :
	handle_{reinterpret_cast<native_handle_type>(
	  CreateSemaphore(nullptr, static_cast<LONG>(init_count), LONG_MAX, nullptr))}
{
	check(handle_);
}

inline unnamed_semaphore_base::~unnamed_semaphore_base() {
	if (reinterpret_cast<HANDLE>(handle_) != 0) {
		CloseHandle(reinterpret_cast<HANDLE>(handle_));
	}
}

inline void unnamed_semaphore_base::release() { semaphore_base::release(handle_); }

inline void unnamed_semaphore_base::release(ptrdiff_t count) { semaphore_base::release(handle_, count); }

inline bool unnamed_semaphore_base::try_acquire() { return semaphore_base::try_acquire(handle_); }

inline void unnamed_semaphore_base::acquire() { semaphore_base::acquire(handle_); }

inline bool unnamed_semaphore_base::try_acquire_for_ns(uint64_t ns) {
	return semaphore_base::try_acquire_for(handle_, ns);
}

inline semaphore_base::native_handle_type unnamed_semaphore_base::native_handle() const noexcept {
	return handle_;
}

// named semaphore functions
inline named_semaphore_base::named_semaphore_base(create_t, const std::string& name, ptrdiff_t init_count, ptrdiff_t init_max) :
	name_{name},
	handle_{reinterpret_cast<native_handle_type>(
	  CreateSemaphore(nullptr, static_cast<LONG>(init_count), static_cast<LONG>(init_max), name.c_str()))}
{
	check(handle_);
}

inline named_semaphore_base::named_semaphore_base(open_or_create_t, const std::string& name, ptrdiff_t init_count, ptrdiff_t init_max) :
	name_{name},
	handle_{reinterpret_cast<native_handle_type>(
	  CreateSemaphore(nullptr, static_cast<LONG>(init_count), static_cast<LONG>(init_max), name.c_str()))}
{
	check(handle_);
}

inline named_semaphore_base::named_semaphore_base(open_t, const std::string& name) :
	handle_{reinterpret_cast<native_handle_type>(
	  OpenSemaphore(DELETE | SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, TRUE, name.c_str()))}
{
	check(handle_);
}

inline named_semaphore_base::~named_semaphore_base() {
	if (reinterpret_cast<HANDLE>(handle_) != nullptr) {
		CloseHandle(reinterpret_cast<HANDLE>(handle_));
	}
}

inline void named_semaphore_base::remove(const std::string& name) { /* No unlink needed for Windows semaphores*/ }
	
} // end namespace genesis

#endif
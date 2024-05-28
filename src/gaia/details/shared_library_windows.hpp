#if !defined GAIA_SHARED_LIBRARY_WINDOWS_HEADER_INCLUDED
#define GAIA_SHARED_LIBRARY_WINDOWS_HEADER_INCLUDED
#pragma once

#include <genesis/config.hpp>

#if GENESIS_MICROSOFT

#include "gaia/shared_library.hpp"

#include <genesis/errno.hpp>

#include <Windows.h>

namespace gaia {

inline auto shared_library::open() -> shared_library::native_handle_type {
	return reinterpret_cast<native_handle_type>(LoadLibrary(path_.c_str()));
}

inline void shared_library::close() {
	if (handle_ != invalid_handle) {
		FreeLibrary(reinterpret_cast<HMODULE>(handle_));
	}
}

inline auto shared_library::symbol(const char* name) const -> void* {
	auto ptr = GetProcAddress(reinterpret_cast<HMODULE>(handle_), name);
	if (ptr == nullptr) { genesis::throw_last_error(); }
	return ptr;
}

inline auto shared_library::symbol_if(const char* name) const -> void* {
	return GetProcAddress(reinterpret_cast<HMODULE>(handle_), name);
}

void shared_library::check(std::error_code& ec) {
	ec.clear();
	if (handle_ == invalid_handle) {
		ec = genesis::get_last_error();
	}
}

void shared_library::check() {
	if (handle_ == invalid_handle) {
		genesis::throw_last_error();
	}
}

} // end namespace gaia

#endif // GENESIS_MICROSOFT

#endif // GAIA_SHARED_LIBRARY_WINDOWS_HEADER_INCLUDED
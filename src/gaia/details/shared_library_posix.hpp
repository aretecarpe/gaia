#if !defined GAIA_SHARED_LIBRARY_POSIX_HEADER_INCLUDED
#define GAIA_SHARED_LIBRARY_POSIX_HEADER_INCLUDED
#pragma once

#include <genesis/config.hpp>

#if GENESIS_POSIX
#include "gaia/shared_library.hpp"

#include <genesis/errno.hpp>

#include <dlfcn.h>

namespace gaia {

inline auto shared_library::open() -> shared_library::native_handle_type {
	int flags{0};
	if ((mode_ & rtld_lazy) != 0) { flags |= RTLD_LAZY; }
	if ((mode_ & rtld_now) != 0) { flags |= RTLD_NOW; }
	if ((mode_ & rtld_global) != 0) { flags |= RTLD_GLOBAL; }
	if ((mode_ & rtld_local) != 0) { flags |= RTLD_LOCAL; }
	if ((mode_ & rtld_now) != 0) { flags |= RTLD_NOW; }
	return dlopen(path_.c_str(), flags);
}

inline void shared_library::close() {
	dlclose(handle_) == 0;
}

inline auto shared_library::symbol(const char* name) const -> void* {
	auto ptr = dlsym(handle_, name);
	if (ptr == nullptr) { genesis::throw_last_error(); }
	return ptr;
}

inline auto shared_library::symbol_if(const char* name) const -> void* {
	return dlsym(handle_, name);
}

void shared_library::check(std::error_code& ec) {
	ec.clear();
	if (handle_ == invalid_handle) {
		ec = genesis::get_last_error();
	}
}

void shared_library::check() {
	if (handle_ == invalid_handle) {
		auto error = dlerror();
		throw std::runtime_error{error != nullptr ? error : "Unspecified error in dlopen"};
	}
}

} // end namespace gaia

#endif // GENESIS_POSIX

#endif // GAIA_SHARED_LIBRARY_POSIX_HEADER_INCLUDED
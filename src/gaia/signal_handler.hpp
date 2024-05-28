#if !defined GAIA_SIGNAL_HEADER_INCLUDED
#define GAIA_SIGNAL_HEADER_INCLUDED
#pragma once

#include <genesis/config.hpp>

#include <csignal>
#include <cstdint>
#include <functional>

namespace gaia {

using signal_handler_callback_t = void (*)(int);

#if GENESIS_MICROSOFT
#define SIGEV_THREAD 0
#define SIGBUS SIGSEGV // SIGBUS/SIGSEGV signal serious program erros, SIGSEGV is best alternative
#define SIGHUP SIGTERM // SIGHUP signaled when controlling terminal is closed, SIGTERM is best alternative
#define SIGKILL 9
#endif

enum class posix_signal : int {
	sigbus = SIGBUS,
	sigint = SIGINT,
	sigterm = SIGTERM,
	sighup = SIGHUP,
	sigabort = SIGABRT,
	sigsegv = SIGSEGV,
};

enum class signal_guard_error : uint8_t {
	invalid_signal_enum_value,
	undefined_error_in_system_call,
};

inline void register_signal(posix_signal signal, signal_handler_callback_t cb) {
	std::signal(static_cast<int>(signal), cb);
}

} // end namespace gaia

#endif
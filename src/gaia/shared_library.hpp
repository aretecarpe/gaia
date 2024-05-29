#if !defined GAIA_SHARED_LIBRARY_HEADER_INCLUDED
#define GAIA_SHARED_LIBRARY_HEADER_INCLUDED
#pragma once

#include <genesis/config.hpp>

#include <cstdint>
#include <filesystem>
#include <system_error>

namespace gaia {

namespace fs = std::filesystem;

class shared_library {
public:
	#if GENESIS_MICROSOFT
	using native_handle_type = intptr_t;
	static constexpr native_handle_type invalid_handle = 0;
	#elif GENESIS_POSIX
	using native_handle_type = void*;
	static constexpr native_handle_type invalid_handle = nullptr;
	#endif

	using dl_mode = uint32_t;

	static constexpr dl_mode rtld_lazy = 0x01;
	/// <summary>Load all symbols on library load.</summary>
	static constexpr dl_mode rtld_now = 0x02;
	/// <summary>Load only global symbols.</summary>
	static constexpr dl_mode rtld_global = 0x04;
	/// <summary>Load only local symbols.</summary>
	static constexpr dl_mode rtld_local = 0x08;

private:
	fs::path path_;
	dl_mode mode_;
	native_handle_type handle_;

public:
	shared_library() noexcept :
		path_{},
		mode_{rtld_lazy},
		handle_{invalid_handle}
	{ }

	explicit shared_library(const fs::path& path) :
		path_{path},
		mode_{rtld_lazy},
		handle_{open()}
	{ check(); }

	shared_library(const fs::path& path, dl_mode mode) :
		path_{path},
		mode_{mode},
		handle_{open()}
	{ check(); }

	shared_library(const fs::path& path, std::error_code& ec) :
		path_{path},
		mode_{rtld_lazy},
		handle_{open()}
	{ check(ec); }

	shared_library(const fs::path& path, dl_mode mode, std::error_code& ec) :
		path_{path},
		mode_{mode},
		handle_{open()}
	{ check(ec); }

	shared_library(const shared_library& other) :
		path_{other.path_},
		mode_{other.mode_},
		handle_{open()}
	{ check(); }

	shared_library(shared_library&& other) noexcept :
		handle_{other.handle_},
		path_{std::move(other.path_)}
	{
		other.handle_ = invalid_handle;
	}

	shared_library& operator=(const shared_library& other) {
		if (&other != this) {
			close();
			path_ = other.path_;
			mode_ = other.mode_;
			handle_ = open();
		}
		return *this;
	}

	shared_library& operator=(shared_library&& other) noexcept {
		if (&other != this) {
			path_ = std::move(other.path_);
			handle_ = other.handle_;
			other.handle_ = invalid_handle;
		}
		return *this;
	}

	~shared_library() { close(); }

	auto is_loaded() const noexcept -> bool { return handle_ != 0; }

	void reset() {
		if (handle_ != invalid_handle) {
			close();
			handle_ = invalid_handle;
			path_.clear();
		}
	}

	explicit operator bool() const noexcept { return handle_ != 0; }

	auto native_handle() const noexcept { return handle_; }

	template <typename T>
	auto get(const char* name) const -> T {
		return reinterpret_cast<T>(symbol(name));
	}

	friend bool operator==(const shared_library& x, const shared_library& y) noexcept {
		return x.handle_ == y.handle_;
	}

	friend bool operator!=(const shared_library& x, const shared_library& y) noexcept {
		return !(x == y);
	}

	friend bool operator<(const shared_library& x, const shared_library& y) noexcept {
		return x.handle_ < y.handle_;
	}

	friend bool operator>(const shared_library& x, const shared_library& y) noexcept {
		return y < x;
	}

	friend bool operator<=(const shared_library& x, const shared_library& y) noexcept {
		return !(y < x);
	}

	friend bool operator>=(const shared_library& x, const shared_library& y) noexcept {
		return !(y < x);
	}

private:
	auto symbol(const char*) const -> void*;

	auto symbol_if(const char*) const -> void*;

	auto open() -> native_handle_type;

	void close();

	void check(std::error_code& ec);

	void check();
};

} // end namespace gaia

#if GENESIS_POSIX
#include "gaia/details/shared_library_posix.hpp"
#else
#include "gaia/details/shared_library_windows.hpp"
#endif

#endif
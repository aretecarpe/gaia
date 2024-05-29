#include <gaia/shared_library.hpp>
#include <gaia/filesystem_helpers.hpp>

#include <iostream>

int main(int argc, char** argv) {
	#if GENESIS_MICROSOFT
	auto path = gaia::find_file_recursively("dummy_lib.dll");
	#else
	auto path = gaia::find_file_recursively("libdummy_lib.so");
	#endif
	if (path) {
		gaia::shared_library slib{*path};
		using function_ptr = int(*)();
		auto fptr = slib.get<function_ptr>("dummy");
		std::cout << "Value from slib: " << fptr() << std::endl;
	} else {
		std::cout << "Failed to find shared lib" << std::endl;
	}
	return 0;
}
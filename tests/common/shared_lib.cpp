#include <genesis/config.hpp>

extern "C" {

// #if GENESIS_MICROSOFT
// int __declspec(dllexport) foobar() { return 42; }
// #else
int foobar() { return 42; }
// #endif

} // end extern
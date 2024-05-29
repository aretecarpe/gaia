#include <genesis/config.hpp>

extern "C" {

#if GENESIS_MICROSOFT
int __declspec(dllexport) dummy() { return 21 * 2; }
#else
int dummy() { return 21 * 2; }
#endif

} // end extern
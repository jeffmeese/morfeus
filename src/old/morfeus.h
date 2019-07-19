#ifndef MORFEUS_SDK
#define MORFEUS_SDK

// decl definitions for shared library
#if defined(MORFEUS_COMPILE_LIBRARY)
#  define MORFEUS_LIB_DECL __declspec(dllexport)
#elif defined(MORFEUS_USE_LIBRARY)
#  define MORFEUS_LIB_DECL __declspec(dllimport)
#else
#  define MORFEUS_LIB_DECL
#endif

#endif // MORFEUS_SDK


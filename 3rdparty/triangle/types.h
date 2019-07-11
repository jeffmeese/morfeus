#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#ifndef SINGLE
#define REAL double
#else
#define REAL float
#endif

#ifdef _UNIX
#define ULONGLONG uint64_t
#else
#define ULONGLONG __int64
#endif

#endif // TYPES_H

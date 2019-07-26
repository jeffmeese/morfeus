#ifndef RANDOM_H
#define RANDOM_H

#include "triangle.h"

extern unsigned ULONGLONG randomseed;                     /* Current random number seed. */

unsigned ULONGLONG randomnation(unsigned int choices);

#endif // RANDOM_H

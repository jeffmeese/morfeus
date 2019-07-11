#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "morfeus.h"

class Matrix3x3
{
public:
  MORFEUS_LIB_DECL Matrix3x3();

private:
  double mMatrix[3][3];
};

#endif // MATRIX3X3_H

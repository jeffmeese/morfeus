#ifndef PLANEWAVE_H
#define PLANEWAVE_H

#include "excitation.h"

class Planewave
    : public Excitation
{
public:
  MORFEUS_LIB_DECL Planewave(int32_t id);

protected:
  void doExcite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs);
};

#endif // PLANEWAVE_H

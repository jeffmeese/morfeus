#ifndef FARFIELD_H
#define FARFIELD_H

#include "observation.h"

class FarField
    : public Observation
{
public:
  MORFEUS_LIB_DECL FarField(int32_t id);

protected:
  void doCalculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield) override;
};

#endif // FARFIELD_H

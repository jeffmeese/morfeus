#include "farfield.h"

#include "mesh.h"
#include "meshinformation.h"

FarField::FarField(int32_t id)
  : Observation (id)
{
}

void FarField::doCalculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield)
{

}

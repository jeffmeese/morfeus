#include "observation.h"

Observation::Observation(int32_t id)
  : mId(id)
{
}

Observation::~Observation()
{
}

void Observation::calculate(double freqGHz, const Mesh *mesh, const MeshInformation *meshInfo, const vector &efield)
{
  doCalculate(freqGHz, mesh, meshInfo, efield);
}

#include "observation.h"

Observation::Observation()
{

}

Observation::Observation(int32_t number)
  : MorfeusObject (number)
{
}

Observation::~Observation()
{
}

void Observation::calculate(double freqGHz, const Mesh *mesh, const MeshInformation *meshInfo, const vector &efield)
{
  doCalculate(freqGHz, mesh, meshInfo, efield);
}

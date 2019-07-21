#include "excitation.h"

Excitation::Excitation()
{

}

Excitation::Excitation(int32_t number)
  : MorfeusObject (number)
{
}

Excitation::Excitation(const std::string & name, int32_t number)
  : MorfeusObject (name, number)
{

}

void Excitation::excite(double freqGHz, const Mesh *mesh, const MeshInformation *meshInfo, vector &rhs) const
{
  doExcite(freqGHz, mesh, meshInfo, rhs);
}

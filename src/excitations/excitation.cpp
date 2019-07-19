#include "excitation.h"

Excitation::Excitation(int32_t id)
  : mId(id)
{
}

Excitation::~Excitation()
{
}

void Excitation::excite(double freqGHz, const Mesh *mesh, const MeshInformation *meshInfo, vector &rhs) const
{
  doExcite(freqGHz, mesh, meshInfo, rhs);
}

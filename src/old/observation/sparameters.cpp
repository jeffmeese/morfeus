#include "sparameters.h"

SParameters::SParameters(std::size_t id)
  : Observation(id, "SParameters")
{
}

SParameters::SParameters(std::size_t id, const std::string & name)
  : Observation(id, name)
{
}

void SParameters::calculate(const Solution * solution, const Mesh & mesh, double freq, const dcomplex_array & efield)
{

}

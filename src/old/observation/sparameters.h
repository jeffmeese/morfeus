#ifndef SPARAMETERS_H
#define SPARAMETERS_H

#include "morfeus.h"

#include "observation/observation.h"

class SParameters
    : public Observation
{
public:
  MORFEUS_LIB_DECL SParameters(std::size_t id);
  MORFEUS_LIB_DECL SParameters(std::size_t id, const std::string & name);

public:
  MORFEUS_LIB_DECL virtual void calculate(const Solution * solution, const Mesh & mesh, double freq, const dcomplex_array & efield);
};

#endif // SPARAMETERS_H

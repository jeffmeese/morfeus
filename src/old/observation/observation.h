#ifndef OBSERVATION_H
#define OBSERVATION_H

#include "morfeus.h"
#include "types.h"

#include <cstddef>
#include <string>

class InputData;
class Mesh;
class Solution;

class Observation
{
public:
  MORFEUS_LIB_DECL Observation(std::size_t id, const std::string & name);

public:
  MORFEUS_LIB_DECL std::size_t id() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL virtual void calculate(const Solution * solution, const Mesh & mesh, double freq, const dcomplex_array & efield) = 0;

private:
  std::size_t mId;
  std::string mName;
};

#endif // OBSERVATION_H

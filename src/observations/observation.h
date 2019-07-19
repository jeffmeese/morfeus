#ifndef OBSERVATION_H
#define OBSERVATION_H

#include "morfeus.h"

#include <boost/numeric/ublas/vector.hpp>

class Mesh;
class MeshInformation;

class Observation
{
public:
  MORFEUS_LIB_DECL virtual ~Observation();

protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL void calculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield);

protected:
  Observation(int32_t id);

protected:
  virtual void doCalculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield) = 0;

private:
  int32_t mId;
};

#endif // OBSERVATION_H

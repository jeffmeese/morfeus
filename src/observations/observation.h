#ifndef OBSERVATION_H
#define OBSERVATION_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <boost/numeric/ublas/vector.hpp>

#include <Factory.h>
#include <Singleton.h>

class Mesh;
class MeshInformation;

class Observation
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL virtual ~Observation();

protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL void calculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield);

public:
  typedef Loki::SingletonHolder<Loki::Factory<Observation, std::string> > Factory;

protected:
  Observation();
  Observation(int32_t number);

protected:
  virtual void doCalculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield) = 0;

private:
  int32_t mId;
};

#endif // OBSERVATION_H

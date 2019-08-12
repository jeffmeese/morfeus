#ifndef MORFEUS_OBSERVATION_OBSERVATION_H
#define MORFEUS_OBSERVATION_OBSERVATION_H

#include "morfeus.h"

#include "core/morfeusobject.h"
#include "core/factory.h"

#include "math/types.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

namespace morfeus {

  namespace mesh {
    class Mesh;
  }

  namespace solution {
    class MeshInformation;
  }
}

namespace morfeus {
namespace observation {

class Observation
    : public core::MorfeusObject
{
  class ObservationFactory;

public:
  MORFEUS_LIB_DECL void calculate(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh,
                                  const solution::MeshInformation * meshInfo, const math::vector & efield);
  MORFEUS_LIB_DECL void report(std::ostream & output) const;

public:
  MORFEUS_LIB_DECL static ObservationFactory & factory();

protected:
  Observation(const std::string & type);
  Observation(const std::string & type, const std::string & name);
  Observation(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual void doCalculate(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh,
                           const solution::MeshInformation * meshInfo, const math::vector & efield) = 0;
  virtual void doReport(std::ostream & output) const = 0;

private:
  class ObservationFactory
  {
  public:
    MORFEUS_LIB_DECL Observation * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Observation*()> creator);

  private:
    core::Factory<Observation*, std::string, boost::function<Observation*()> > mFactory;
  };
};

}
}

#endif // MORFEUS_OBSERVATION_OBSERVATION_H

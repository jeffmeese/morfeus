#include "observation.h"

namespace morfeus {
namespace observation {

Observation::Observation(const std::string & type)
  : MorfeusObject (type)
{

}

Observation::Observation(const std::string & type, const std::string & name)
  : MorfeusObject (type, name)
{
}

Observation::Observation(const std::string & type, const std::string & id, const std::string & name)
  : MorfeusObject (type, id, name)
{
}

void Observation::calculate(double freqGHz, double thetaInc, double phiInc,
                            const mesh::Mesh *mesh, const solution::MeshInformation *meshInfo, const math::vector &efield)
{
  doCalculate(freqGHz, thetaInc, phiInc, mesh, meshInfo, efield);
}

Observation::ObservationFactory & Observation::factory()
{
  static ObservationFactory f;
  return f;
}

Observation * Observation::ObservationFactory::create(const std::string & type)
{
  Observation * object = mFactory.create(type);
  return object;
}

bool Observation::ObservationFactory::registerType(const std::string & type, boost::function<Observation*()> creator)
{
  mFactory.registerFactory(type, creator);
  return true;
}

void Observation::report(std::ostream &output) const
{
  doReport(output);
}

}
}

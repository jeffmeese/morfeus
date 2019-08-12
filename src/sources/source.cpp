#include "source.h"

namespace morfeus {
namespace sources {

Source::Source(const std::string & type)
  : MorfeusObject (type)
  , mAngleDependent(false)
{
}

Source::Source(const std::string & type, const std::string & name)
  : MorfeusObject (type, name)
  , mAngleDependent(false)
{
}

Source::Source(const std::string & type, const std::string & id, const std::string & name)
  : MorfeusObject (type, id, name)
  , mAngleDependent(false)
{
}
void Source::excite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh,
                        const solution::MeshInformation * meshInfo, math::vector & rhs) const
{
  doExcite(freqGHz, thetaInc, phiInc, mesh, meshInfo, rhs);
}

Source::SourceFactory & Source::factory()
{
  static SourceFactory f;
  return f;
}

Source * Source::SourceFactory::create(const std::string & type)
{
  return mFactory.create(type);
}

bool Source::SourceFactory::registerType(const std::string &type, boost::function<Source *()> creator)
{
  mFactory.registerFactory(type, creator);
  return true;
}

std::ostream & operator<<(std::ostream & output, const Source & object)
{
  object.print(output);
  return output;
}

}
}

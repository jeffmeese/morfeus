#ifndef MORFEUS_SOURCES_SOURCE__H
#define MORFEUS_SOURCES_SOURCE__H

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
namespace sources {

class Source
    : public core::MorfeusObject
{
  class SourceFactory;

public:
  MORFEUS_LIB_DECL bool angleDependent() const;
  MORFEUS_LIB_DECL void setAngleDependent(bool value);

public:
  MORFEUS_LIB_DECL void excite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh,
                               const solution::MeshInformation * meshInfo, math::vector & rhs) const;

public:
  MORFEUS_LIB_DECL static SourceFactory & factory();

protected:
  Source(const std::string & type);
  Source(const std::string & type, const std::string & name);
  Source(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual void doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh,
                        const solution::MeshInformation * meshInfo, math::vector & rhs) const = 0;

private:
  class SourceFactory
  {
    friend class Source;

  public:
    MORFEUS_LIB_DECL Source * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Source*()> creator);

  private:
    core::Factory<Source*, std::string, boost::function<Source*()> > mFactory;
  };

private:
  bool mAngleDependent;
  static SourceFactory mFactory;
};

inline bool Source::angleDependent() const
{
  return mAngleDependent;
}

inline void Source::setAngleDependent(bool value)
{
  mAngleDependent = value;
}


}
}

#endif // MORFEUS_EXCITATION_EXCITATION_H

#ifndef MORFEUS_SOURCES_VOLTAGESOURCE_H
#define MORFEUS_SOURCES_VOLTAGESOURCE_H

#include "source.h"

#include "math/types.h"

namespace morfeus {
  namespace model {
    namespace ports {
      class Port;
    }
  }
}

namespace morfeus {
namespace sources {

class VoltageSource
    : public Source
{
public:
  MORFEUS_LIB_DECL VoltageSource();
  MORFEUS_LIB_DECL VoltageSource(const std::string & name);
  MORFEUS_LIB_DECL VoltageSource(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL model::ports::Port * port();
  MORFEUS_LIB_DECL const model::ports::Port * port() const;
  MORFEUS_LIB_DECL math::dcomplex voltage() const;
  MORFEUS_LIB_DECL void setPort(model::ports::Port * port);
  MORFEUS_LIB_DECL void setVoltage(const math::dcomplex & value);

protected:
  void doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh,
                const solution::MeshInformation * meshInfo, math::vector & rhs) const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  math::dcomplex mVoltage;
  model::ports::Port * mPort;
};

inline model::ports::Port * VoltageSource::port()
{
  return mPort;
}

inline const model::ports::Port * VoltageSource::port() const
{
  return mPort;
}

inline math::dcomplex VoltageSource::voltage() const
{
  return mVoltage;
}

inline void VoltageSource::setVoltage(const math::dcomplex & value)
{
  mVoltage = value;
}

inline void VoltageSource::setPort(model::ports::Port * port)
{
  mPort = port;
}

}
}

#endif // MORFEUS_EXCITATIONS_VOLTAGESOURCE_H

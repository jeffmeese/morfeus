#ifndef MORFEUS_MEDIA_PERFECTELECTRICCONDUCTOR_H
#define MORFEUS_MEDIA_PERFECTELECTRICCONDUCTOR_H

#include "dielectric.h"

namespace morfeus {
namespace model {
namespace media {

class PerfectElectricConductor
    : public Dielectric
{
public:
  MORFEUS_LIB_DECL PerfectElectricConductor();

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  math::dcomplex doValue(double freqGHz) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;
};

}
}
}

#endif // MORFEUS_MEDIA_PERFECTELECTRICCONDUCTOR_H

#ifndef FREESPACE_H
#define FREESPACE_H

#include "dielectric.h"

namespace morfeus {
namespace model {
namespace media {

class FreeSpace
    : public Dielectric
{
public:
  MORFEUS_LIB_DECL FreeSpace();

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  math::dcomplex doValue(double freqGHz) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;
};

}
}
}

#endif // FREESPACE_H

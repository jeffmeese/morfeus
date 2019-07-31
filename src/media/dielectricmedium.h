#ifndef DIELECTRICMEDIUM_H
#define DIELECTRICMEDIUM_H

#include "medium.h"

namespace morfeus {
namespace media {

class DielectricMedium
    : public Medium
{
public:
  MORFEUS_LIB_DECL DielectricMedium();
  MORFEUS_LIB_DECL DielectricMedium(const std::string & name);
  MORFEUS_LIB_DECL DielectricMedium(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL DielectricMedium(dcomplex permittivity, dcomplex permeability);
  MORFEUS_LIB_DECL DielectricMedium(const std::string & name, dcomplex permittivity, dcomplex permeability);
  MORFEUS_LIB_DECL DielectricMedium(const std::string & id, const std::string & name, dcomplex permittivity, dcomplex permeability);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  dcomplex doValue(double freqGHz) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  dcomplex mPermeability;
  dcomplex mPermittivity;
};

}
}

#endif // DIELECTRICMEDIUM_H

#ifndef PLANEWAVE_H
#define PLANEWAVE_H

#include "excitation.h"

namespace morfeus {

class Planewave
    : public Excitation
{
public:
  MORFEUS_LIB_DECL Planewave();
  MORFEUS_LIB_DECL Planewave(const std::string & name);
  MORFEUS_LIB_DECL Planewave(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL double fieldPolarization() const;
  MORFEUS_LIB_DECL void setFieldPolarization(double fieldPolarization);

protected:
  void doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mFieldPolarization;
};

inline double Planewave::fieldPolarization() const
{
  return mFieldPolarization;
}

inline void Planewave::setFieldPolarization(double fieldPolarization)
{
  mFieldPolarization = fieldPolarization;
}

}

#endif // PLANEWAVE_H

#ifndef PLANEWAVE_H
#define PLANEWAVE_H

#include "excitation.h"

class Planewave
    : public Excitation
{
public:
  MORFEUS_LIB_DECL Planewave();
  MORFEUS_LIB_DECL Planewave(const std::string & name);
  MORFEUS_LIB_DECL Planewave(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL double fieldPolarization() const;
  MORFEUS_LIB_DECL double phiIncident() const;
  MORFEUS_LIB_DECL double thetaIncident() const;
  MORFEUS_LIB_DECL void setFieldPolarization(double fieldPolarization);
  MORFEUS_LIB_DECL void setPhiIncident(double phi);
  MORFEUS_LIB_DECL void setThetaIncident(double theta);

protected:
  void doExcite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mFieldPolarization;
  double mPhiIncident;
  double mThetaIncident;
};

inline double Planewave::fieldPolarization() const
{
  return mFieldPolarization;
}

inline double Planewave::phiIncident() const
{
  return mPhiIncident;
}

inline double Planewave::thetaIncident() const
{
  return mThetaIncident;
}

inline void Planewave::setFieldPolarization(double fieldPolarization)
{
  mFieldPolarization = fieldPolarization;
}

inline void Planewave::setThetaIncident(double theta)
{
  mThetaIncident = theta;
}

inline void Planewave::setPhiIncident(double phi)
{
  mPhiIncident = phi;
}

#endif // PLANEWAVE_H

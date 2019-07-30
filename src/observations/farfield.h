#ifndef FARFIELD_H
#define FARFIELD_H

#include "observation.h"

namespace Morfeus {
namespace observation {

class FarField
    : public Observation
{
public:
  MORFEUS_LIB_DECL FarField();
  MORFEUS_LIB_DECL FarField(const std::string & name);
  MORFEUS_LIB_DECL FarField(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL bool monostatic() const;
  MORFEUS_LIB_DECL double phiIncr() const;
  MORFEUS_LIB_DECL double phiStart() const;
  MORFEUS_LIB_DECL double phiStop() const;
  MORFEUS_LIB_DECL double thetaIncr() const;
  MORFEUS_LIB_DECL double thetaStart() const;
  MORFEUS_LIB_DECL double thetaStop() const;
  MORFEUS_LIB_DECL void setMonostatic(bool value);
  MORFEUS_LIB_DECL void setPhiIncr(double value);
  MORFEUS_LIB_DECL void setPhiStart(double value);
  MORFEUS_LIB_DECL void setPhiStop(double value);
  MORFEUS_LIB_DECL void setThetaIncr(double value);
  MORFEUS_LIB_DECL void setThetaStart(double value);
  MORFEUS_LIB_DECL void setThetaStop(double value);

protected:
  void doCalculate(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const MeshInformation * meshInfo, const vector & efield) override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doReport(std::ostream & output) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init();

private:
  bool mMonostatic;
  double mPhiIncr;
  double mPhiStart;
  double mPhiStop;
  double mThetaIncr;
  double mThetaStart;
  double mThetaStop;
};

inline bool FarField::monostatic() const
{
  return mMonostatic;
}

inline double FarField::phiIncr() const
{
  return mPhiIncr;
}

inline double FarField::phiStart() const
{
  return mPhiStart;
}

inline double FarField::phiStop() const
{
  return mPhiStop;
}

inline double FarField::thetaIncr() const
{
  return mThetaIncr;
}

inline double FarField::thetaStart() const
{
  return mThetaStart;
}

inline double FarField::thetaStop() const
{
  return mThetaStop;
}

inline void FarField::setMonostatic(bool value)
{
  mMonostatic = value;
}

inline void FarField::setPhiIncr(double value)
{
  mPhiIncr = value;
}

inline void FarField::setPhiStart(double value)
{
  mPhiStart = value;
}

inline void FarField::setPhiStop(double value)
{
  mPhiStop = value;
}

inline void FarField::setThetaIncr(double value)
{
  mThetaIncr = value;
}

inline void FarField::setThetaStart(double value)
{
  mThetaStart = value;
}

inline void FarField::setThetaStop(double value)
{
  mThetaStop = value;
}

}
}

#endif // FARFIELD_H

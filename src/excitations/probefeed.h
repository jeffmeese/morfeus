#ifndef PROBEFEED_H
#define PROBEFEED_H

#include "morfeus.h"

#include "excitation.h"

namespace Morfeus {

class ProbeFeed
    : public Excitation
{
public:
  MORFEUS_LIB_DECL ProbeFeed();
  MORFEUS_LIB_DECL ProbeFeed(const std::string & name);
  MORFEUS_LIB_DECL ProbeFeed(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL double x1() const;
  MORFEUS_LIB_DECL double x2() const;
  MORFEUS_LIB_DECL double y1() const;
  MORFEUS_LIB_DECL double y2() const;
  MORFEUS_LIB_DECL double z1() const;
  MORFEUS_LIB_DECL double z2() const;
  MORFEUS_LIB_DECL dcomplex voltage() const;
  MORFEUS_LIB_DECL void setX1(double x1);
  MORFEUS_LIB_DECL void setX2(double x2);
  MORFEUS_LIB_DECL void setY1(double y1);
  MORFEUS_LIB_DECL void setY2(double y2);
  MORFEUS_LIB_DECL void setZ1(double z1);
  MORFEUS_LIB_DECL void setZ2(double z2);
  MORFEUS_LIB_DECL void setPosition(double x1, double y1, double z1, double x2, double y2, double z2);
  MORFEUS_LIB_DECL void setVoltage(const dcomplex & value);

protected:
  void doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(double x1, double y1, double z1, double x2, double y2, double z2, const dcomplex & value);

private:
  double mX1;
  double mY1;
  double mZ1;
  double mX2;
  double mY2;
  double mZ2;
  dcomplex mVoltage;
};

inline void ProbeFeed::setX1(double x1)
{
  mX1 = x1;
}

inline void ProbeFeed::setX2(double x2)
{
  mX2 = x2;
}

inline void ProbeFeed::setY1(double y1)
{
  mY1 = y1;
}

inline void ProbeFeed::setY2(double y2)
{
  mY2 = y2;
}

inline void ProbeFeed::setZ1(double z1)
{
  mZ1 = z1;
}

inline void ProbeFeed::setZ2(double z2)
{
  mZ2 = z2;
}

inline void ProbeFeed::setVoltage(const dcomplex &value)
{
  mVoltage = value;
}

inline dcomplex ProbeFeed::voltage() const
{
  return mVoltage;
}

inline double ProbeFeed::x1() const
{
  return mX1;
}

inline double ProbeFeed::x2() const
{
  return mX2;
}

inline double ProbeFeed::y1() const
{
  return mY1;
}

inline double ProbeFeed::y2() const
{
  return mY2;
}

inline double ProbeFeed::z1() const
{
  return mZ1;
}

inline double ProbeFeed::z2() const
{
  return mZ2;
}

}

#endif // PROBEFEED_H

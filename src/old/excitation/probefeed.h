#ifndef PROBEFEED_H
#define PROBEFEED_H

#include "morfeus.h"

#include "excitation/excitation.h"

class ProbeFeed
    : public Excitation
{
public:
  MORFEUS_LIB_DECL ProbeFeed(std::size_t id);
  MORFEUS_LIB_DECL ProbeFeed(std::size_t id, double x1, double y1, double z1, double x2, double y2, double z2, const dcomplex & value);

public:
  MORFEUS_LIB_DECL double x1() const;
  MORFEUS_LIB_DECL double x2() const;
  MORFEUS_LIB_DECL double y1() const;
  MORFEUS_LIB_DECL double y2() const;
  MORFEUS_LIB_DECL double z1() const;
  MORFEUS_LIB_DECL double z2() const;
  MORFEUS_LIB_DECL dcomplex excitation() const;
  MORFEUS_LIB_DECL void setX1(double x1);
  MORFEUS_LIB_DECL void setX2(double x2);
  MORFEUS_LIB_DECL void setY1(double y1);
  MORFEUS_LIB_DECL void setY2(double y2);
  MORFEUS_LIB_DECL void setZ1(double z1);
  MORFEUS_LIB_DECL void setZ2(double z2);
  MORFEUS_LIB_DECL void setPosition(double x1, double y1, double z1, double x2, double y2, double z2);
  MORFEUS_LIB_DECL void setExcitation(const dcomplex & value);

public:
  MORFEUS_LIB_DECL virtual void excite(const Mesh & mesh, double freq, dcomplex_array & rhs) const;

private:
  double mX1;
  double mY1;
  double mZ1;
  double mX2;
  double mY2;
  double mZ2;
  dcomplex mExcitation;
};

#endif // PROBEFEED_H

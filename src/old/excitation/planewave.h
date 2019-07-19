#ifndef PLANEWAVE_H
#define PLANEWAVE_H

#include "morfeus.h"

#include "excitation/excitation.h"

class Planewave
    : public Excitation
{
public:
  MORFEUS_LIB_DECL Planewave(std::size_t id);
  MORFEUS_LIB_DECL Planewave(std::size_t id, double fieldPolarization, double thetaIncident, double phiIncident);

public:
  MORFEUS_LIB_DECL double fieldPolarization() const;
  MORFEUS_LIB_DECL double phiIncident() const;
  MORFEUS_LIB_DECL double thetaIncident() const;
  MORFEUS_LIB_DECL void setFieldPolarization(double fieldPolarization);
  MORFEUS_LIB_DECL void setPhiIncident(double phi);
  MORFEUS_LIB_DECL void setThetaIncident(double theta);

public:
  MORFEUS_LIB_DECL virtual void excite(const Mesh & mesh, double freq, dcomplex_array & rhs) const;

private:
  double mFieldPolarization;
  double mPhiIncident;
  double mThetaIncident;
};

#endif // PLANEWAVE_H

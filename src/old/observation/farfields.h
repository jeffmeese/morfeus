#ifndef FARFIELDS_H
#define FARFIELDS_H

#include "morfeus.h"

#include "observation/observation.h"

#include <vector>
#include <fstream>

class FarFields
    : public Observation
{
public:
  MORFEUS_LIB_DECL FarFields(std::size_t id);
  MORFEUS_LIB_DECL FarFields(std::size_t id, const std::string & name);

public:
  MORFEUS_LIB_DECL void addPhiAngle(double phiAngle);
  MORFEUS_LIB_DECL void addThetaAngle(double thetaAngle);
  MORFEUS_LIB_DECL std::string fileName() const;
  MORFEUS_LIB_DECL double phiAngle(std::size_t index) const;
  MORFEUS_LIB_DECL double thetaAngle(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalPhiAngles() const;
  MORFEUS_LIB_DECL std::size_t totalThetaAngle() const;
  MORFEUS_LIB_DECL void setFileName(const std::string & fileName);

public:
  MORFEUS_LIB_DECL virtual void calculate(const Solution * solution, const Mesh & mesh, double freq, const dcomplex_array & efield);

private:
  std::vector<double> mThetaAngles;
  std::vector<double> mPhiAngles;
  std::string mFileName;
  std::ofstream mOutput;
};

#endif // FARFIELDS_H

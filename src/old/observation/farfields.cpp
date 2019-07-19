#include "farfields.h"

#include "math/constants.h"
#include "math/functions.h"

#include "mesh/face.h"
#include "mesh/mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

FarFields::FarFields(std::size_t id)
  : Observation(id, "Far Fields")
{
}

FarFields::FarFields(std::size_t id, const std::string & name)
  : Observation(id, name)
{
}

void FarFields::addPhiAngle(double phiAngle)
{
  mPhiAngles.push_back(phiAngle);
}

void FarFields::addThetaAngle(double thetaAngle)
{
  mThetaAngles.push_back(thetaAngle);
}

void FarFields::calculate(const Solution * solution, const Mesh & mesh, double freq, const dcomplex_array & efield)
{
  double k0 = math::frequencyToWaveNumber(freq);
  double lam = math::frequencyToWavelength(freq);

  mOutput.open(mFileName, std::ios_base::out|std::ios_base::app);

  std::vector<std::unique_ptr<Face>> boundaryFaces = mesh.getBoundaryFaces();

  for (std::size_t i = 0; i < mThetaAngles.size(); i++) {
    double thetaAngle = mThetaAngles.at(i);
    for (std::size_t j = 0; j < mPhiAngles.size(); j++) {
      double phiAngle = mPhiAngles.at(j);

      dcomplex iTheta(math::ZERO), iPhi(math::ZERO);
      for (std::size_t k = 0; k < boundaryFaces.size(); k++) {
        Face * face = boundaryFaces.at(k).get();
        Face::FarFieldEntry fieldEntry = face->computeFarFieldEntry(freq, thetaAngle, phiAngle, efield, mesh);
        iTheta += fieldEntry.i1;
        iPhi += fieldEntry.i2;
      }
      dcomplex ETheta = iPhi;
      dcomplex EPhi = -iTheta;
      //dcomplex EPhi = iTheta;

      double factor = sqrt(math::PI/2.0) / (sqrt(math::Z0)*lam*lam);
      EPhi *= factor;
      ETheta *= factor;

      double constant = (4.0*math::PI) / (k0*k0);
      //double constant = 4.0*math::PI*k0*k0*math::Y0*math::Y0;
      //double constant = k0*k0/math::PI;
      double magTheta = 10.0*log10( constant * abs(ETheta) * abs(ETheta) );
      double magPhi = 10.0*log10( constant * abs(EPhi) * abs(EPhi) );
      double phaseTheta = atan2(ETheta.imag(), ETheta.real()) * math::RAD_TO_DEG;
      double phasePhi = atan2(EPhi.imag(), EPhi.real()) * math::RAD_TO_DEG;

      std::ostringstream oss;

			int w = 12;
			oss << std::setprecision(4) << std::setiosflags(std::ios::fixed) << freq
						 << std::setw(w) << std::setprecision(2) << std::setiosflags(std::ios::fixed) << thetaAngle
						 << std::setw(w) << std::setprecision(2) << std::setiosflags(std::ios::fixed) << phiAngle
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << magTheta
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << phaseTheta
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << magPhi
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << phasePhi
						 << std::endl;

			std::cout << oss.str();

			mOutput << std::setprecision(4) << std::setiosflags(std::ios::fixed) << freq
						 << std::setw(w) << std::setprecision(2) << std::setiosflags(std::ios::fixed) << thetaAngle
						 << std::setw(w) << std::setprecision(2) << std::setiosflags(std::ios::fixed) << phiAngle
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << magTheta
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << phaseTheta
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << magPhi
						 << std::setw(w) << std::setprecision(4) << std::setiosflags(std::ios::fixed) << phasePhi
						 << std::endl;

			mOutput.close();
    }
  }
}

std::string FarFields::fileName() const
{
  return mFileName;
}

double FarFields::phiAngle(std::size_t index) const
{
  return mPhiAngles.at(index);
}

void FarFields::setFileName(const std::string &fileName)
{
  mFileName = fileName;
  mOutput.open(fileName.c_str());

  std::ostringstream oss;
  oss << "Freq (GHz),Theta,Phi,EThetaMag,EThetaPhase,EPhiTheta,EPhiPhase\n";
  mOutput << oss.str();
  mOutput.close();
}

double FarFields::thetaAngle(std::size_t index) const
{
  return mThetaAngles.at(index);
}

std::size_t FarFields::totalPhiAngles() const
{
  return mPhiAngles.size();
}

std::size_t FarFields::totalThetaAngle() const
{
  return mThetaAngles.size();
}

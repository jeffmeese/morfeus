#include "solver/solution.h"

#include "excitation/excitation.h"
#include "excitation/planewave.h"

#include "math/constants.h"
#include "math/functions.h"
#include "math/gaussquad.h"
#include "math/vector.h"

#include "mesh/face.h"
#include "mesh/mesh.h"
#include "mesh/meshproperties.h"

#include "observation/observation.h"

#include <iostream>

Solution::Solution(const Mesh & mesh)
  : mMesh(mesh)
  , mMeshProperties(mesh)
  , mVerbose(false)
{
}

void Solution::addFrequency(double freq)
{
  mFrequencies.push_back(freq);
}

void Solution::addNonPlanewaveExcitation(std::unique_ptr<Excitation> excitation)
{
  mNonPlanewaveExcitations.push_back(std::move(excitation));
}

void Solution::addObservation(std::unique_ptr<Observation> observation)
{
  mObservations.push_back(std::move(observation));
}

void Solution::addPlanewave(std::unique_ptr<Planewave> planewave)
{
  mPlanewaves.push_back(std::move(planewave));
}

Excitation * Solution::findExcitation(std::size_t id) const
{
  for (std::size_t i = 0; i < mNonPlanewaveExcitations.size(); i++) {
    if (mNonPlanewaveExcitations.at(i).get()->id() == id)
      return mNonPlanewaveExcitations.at(i).get();
  }

  for (std::size_t i = 0; i < mPlanewaves.size(); i++) {
    if (mPlanewaves.at(i).get()->id() == id)
      return mPlanewaves.at(i).get();
  }

  return nullptr;
}

double Solution::frequency(std::size_t index) const
{
  return mFrequencies.at(index);
}

Excitation * Solution::nonPlanewaveExcitation(std::size_t index) const
{
  return mNonPlanewaveExcitations.at(index).get();
}

Observation * Solution::observation(std::size_t index) const
{
  return mObservations.at(index).get();
}

Planewave * Solution::planewave(std::size_t index) const
{
  return mPlanewaves.at(index).get();
}

dcomplex Solution::linearIntegral(double freq, size_t test, size_t source, Face * testFace, Face * sourceFace) const
{
	static const double tolerance = 1e-6;

  double lambda = math::frequencyToWavelength(freq);
  double k0 = math::frequencyToWaveNumber(freq);

	double xi[3], yi[3], zi[3], xj[3], yj[3], zj[3];
	for (size_t k = 0; k < 3; k++) {
		Node * node1 = mMesh.getNode(testFace->globalNode(k));
		xi[k] = node1->x();
		yi[k] = node1->y();
		zi[k] = node1->z();

		Node * node2 = mMesh.getNode(sourceFace->globalNode(k));
		xj[k] = node2->x();
		yj[k] = node2->y();
		zj[k] = node2->z();
	}

	double xt = xi[test];
	double yt = yi[test];
	double xs = xj[source];
	double ys = yj[source];

	// Determine the number of integration points needed
	double xc = 0.0, yc = 0.0;
	double xcp = 0.0, ycp = 0.0;
	math::centroid(3, xi, yi, xc, yc);
	math::centroid(3, xj, yj, xcp, ycp);
	double cdiff = sqrt( (xc-xcp)*(xc-xcp) + (yc-ycp)*(yc-ycp) );

	// Determine the number of integration points needed
	double singExtract = 0.0;
	bool selfCell = false;
	size_t numTestPoints = 1, numSourcePoints = 4;
	if (testFace->isCoincident(sourceFace)) {
		// Self Cell
		numTestPoints = 1;
		numSourcePoints = 7;
		selfCell = true;
		singExtract = 1.0;
	}
	else if (cdiff / lambda <= 0.2) {
		// Near Cell
		numTestPoints = 4;
		numSourcePoints = 4;
	}

	// Compute the gaussian quadrature points
	std::vector<math::TrianglePoint> testPoints, sourcePoints;
	numTestPoints = math::gaussQuadTriangle(numTestPoints, testPoints);
	numSourcePoints = math::gaussQuadTriangle(numSourcePoints, sourcePoints);

	// Do the numeric integral
	dcomplex result(0.0,0.0);
	for (size_t k = 0; k < numTestPoints; k++) {
		double a = testPoints[k].abs1;
		double b = testPoints[k].abs2;
		double c = testPoints[k].abs3;
		double wi = testPoints[k].weight;
		double x = xi[0]*a + xi[1]*b + xi[2]*c;
		double y = yi[0]*a + yi[1]*b + yi[2]*c;

		for (size_t m = 0; m < numSourcePoints; m++) {
			double a = sourcePoints[m].abs1;
			double b = sourcePoints[m].abs2;
			double c = sourcePoints[m].abs3;
			double wj = sourcePoints[m].weight;
			double xp = xj[0]*a + xj[1]*b + xj[2]*c;
			double yp = yj[0]*a + yj[1]*b + yj[2]*c;

			// Compute the numeric integration
			double R = sqrt((x-xp)*(x-xp) + (y-yp)*(y-yp));
			if (R <= tolerance) {
				result += (wi*wj)*(-math::CJ*k0);
			}
			else {
				double arg = ( (x-xt)*(xp-xs) + (y-yt)*(yp-ys) );
				result += (wi*wj) * arg * (exp(-math::CJ*k0*R) - singExtract) / R;
			}
		}
	}

	// Do the analytic integral
	double sing = 0.0;
	if (selfCell) {
		std::vector<math::TrianglePoint> singPoints;
		math::gaussQuadTriangle(1, singPoints);

		double a = singPoints[0].abs1;
		double b = singPoints[0].abs2;
		double c = singPoints[0].abs3;
		double x = xi[0]*a + xi[1]*b + xi[2]*c;
		double y = yi[0]*a + yi[1]*b + yi[2]*c;
		double area = math::triangleArea(xi, yi);

		math::vector<double> r(x, y, 0.0);
		math::vector<double> ri(xi[test], yi[test], 0.0);
		math::vector<double> rj(xi[source], yi[source], 0.0);

		double unipot = singularUniformIntegral(x, y, xi, yi);
		math::vector<double> linpot = singularLinearIntegral(x, y, xi, yi);
		sing = math::dotProduct(r-ri, linpot) + math::dotProduct(r-ri, r-rj)*unipot;
		sing /= area;
	}
	result += sing;

	return result;
}

const Mesh & Solution::mesh() const
{
  return mMesh;
}

const MeshProperties & Solution::meshProperties() const
{
  return mMeshProperties;
}

bool Solution::postProcess(double freq, const dcomplex_array &efield)
{
  for (std::size_t i = 0; i < totalObservations(); i++) {
    Observation * observation = mObservations.at(i).get();
    observation->calculate(this, mMesh, freq, efield);
  }

  return true;
}

void Solution::setVerbose(bool verbose)
{
  mVerbose = verbose;
}

math::vector<double> Solution::singularLinearIntegral(double x, double y, double * xi, double * yi) const
{
	math::vector<double> zhat(0.0,0.0,1.0);
	math::vector<double> r(x, y, 0.0);

	math::vector<double> linpot;
	for (size_t i = 0; i < 3; i++) {
		math::vector<double> rimin(xi[i], yi[i], 0.0);
		math::vector<double> riplus(xi[0], yi[0], 0.0);
		if (i != 2)
			riplus.set(xi[i+1], yi[i+1], 0.0);

		math::vector<double> li(riplus - rimin);
		math::vector<double> lihat(li/li.length());

		double liplus = math::dotProduct(riplus-r, lihat);
		double limin = math::dotProduct(rimin-r, lihat);

		math::vector<double> uihat = math::crossProduct(lihat, zhat);

		math::vector<double> Riplus(r - riplus);
		math::vector<double> Rimin(r - rimin);
		double magRiplus = Riplus.length();
		double magRimin = Rimin.length();

		math::vector<double> Pio((riplus-r) - lihat*liplus);
		double magPio = Pio.length();

		double marg = 0.0, parg = 0.0;
		if (limin < 0) {
			marg = magPio*magPio / ( fabs(limin) + sqrt(magPio*magPio+limin*limin) );
		}
		else {
			marg = magRimin + limin;
		}

		if (liplus < 0) {
			parg = magPio*magPio / ( fabs(liplus) + sqrt(magPio*magPio + liplus*liplus) );
		}
		else {
			parg = magRiplus + liplus;
		}

		double val = magPio*magPio * log(parg/marg) + liplus*magRiplus - limin*magRimin;
		math::vector<double> linelem(uihat.x()*val, uihat.y()*val, uihat.z()*val);
		linpot += linelem;
	}

	linpot /= 2.0;
	return linpot;
}

double Solution::singularUniformIntegral(double x, double y, double * xi, double * yi) const
{
	math::vector<double> zhat(0.0,0.0,1.0);
	math::vector<double> r(x, y, 0.0);

	double result = 0.0;
	for (size_t i = 0; i < 3; i++) {
		math::vector<double> rimin(xi[i], yi[i], 0.0);
		math::vector<double> riplus(xi[0], yi[0], 0.0);
		if (i != 2)
			riplus.set(xi[i+1], yi[i+1], 0.0);

		math::vector<double> li(riplus - rimin);
		math::vector<double> lihat(li/li.length());

		double liplus = math::dotProduct(riplus-r, lihat);
		double limin = math::dotProduct(rimin-r, lihat);

		math::vector<double> uihat = math::crossProduct(lihat, zhat);

		math::vector<double> Riplus(r - riplus);
		math::vector<double> Rimin(r - rimin);
		double magRiplus = Riplus.length();
		double magRimin = Rimin.length();

		math::vector<double> Pio((riplus-r) - lihat*liplus);
		double magPio = Pio.length();
		math::vector<double> Piohat(Pio/Pio.length());

		double marg = 0.0, parg = 0.0;
		if (limin < 0) {
			marg = magPio*magPio / ( fabs(limin) + sqrt(magPio*magPio+limin*limin) );
		}
		else {
			marg = magRimin + limin;
		}

		if (liplus < 0) {
			 parg = magPio*magPio / ( fabs(liplus) + sqrt(magPio*magPio + liplus*liplus) );
		}
		else {
			parg = magRiplus + liplus;
		}
		double logterm = magPio * log(parg/marg);
		result += math::dotProduct(Piohat,uihat) * logterm;
	}
	return result;
}

std::size_t Solution::totalFrequencies() const
{
	return mFrequencies.size();
}

std::size_t Solution::totalNonPlanewaveExcitations() const
{
	return mNonPlanewaveExcitations.size();
}

std::size_t Solution::totalObservations() const
{
	return mObservations.size();
}

std::size_t Solution::totalPlanewaves() const
{
	return mPlanewaves.size();
}

dcomplex Solution::uniformIntergal(double freq, Face * testFace, Face * sourceFace) const
{
	static const double tolerance = 1e-6;

	const Mesh & mesh = this->mesh();

  double lambda = math::frequencyToWavelength(freq);
  double k0 = math::frequencyToWaveNumber(freq);

	// Compute the non-edge dependent integration
	double xi[3], yi[3], zi[3], xj[3], yj[3], zj[3];
	for (std::size_t k = 0; k < 3; k++) {
		Node * node1 = mesh.getNode(testFace->globalNode(k));
		xi[k] = node1->x();
		yi[k] = node1->y();
		zi[k] = node1->z();

		Node * node2 = mesh.getNode(sourceFace->globalNode(k));
		xj[k] = node2->x();
		yj[k] = node2->y();
		zj[k] = node2->z();
	}

	//exit(1);
	double xc = 0.0, yc = 0.0;
	double xcp = 0.0, ycp = 0.0;
	math::centroid(3, xi, yi, xc, yc);
	math::centroid(3, xj, yj, xcp, ycp);
	double cdiff = sqrt( (xc-xcp)*(xc-xcp) + (yc-ycp)*(yc-ycp) );

	// Determine the number of integration points needed
	double singExtract = 0.0;
	bool selfCell = false;
	size_t numTestPoints = 1, numSourcePoints = 4;
	if (testFace->isCoincident(sourceFace)) {
		// Self Cell
		numTestPoints = 1;
		numSourcePoints = 7;
		selfCell = true;
		singExtract = 1.0;
	}
	else if (cdiff / lambda <= 0.2) {
		// Near Cell
		numTestPoints = 4;
		numSourcePoints = 4;
	}

	// Compute the gaussian quadrature points
	std::vector<math::TrianglePoint> testPoints, sourcePoints;
	numTestPoints = math::gaussQuadTriangle(numTestPoints, testPoints);
	numSourcePoints = math::gaussQuadTriangle(numSourcePoints, sourcePoints);

	// Do the numeric integral
	dcomplex result(0.0,0.0);
	for (size_t k = 0; k < numTestPoints; k++) {
		double a = testPoints[k].abs1;
		double b = testPoints[k].abs2;
		double c = testPoints[k].abs3;
		double wi = testPoints[k].weight;
		double x = xi[0]*a + xi[1]*b + xi[2]*c;
		double y = yi[0]*a + yi[1]*b + yi[2]*c;

		for (size_t m = 0; m < numSourcePoints; m++) {
			double a = sourcePoints[m].abs1;
			double b = sourcePoints[m].abs2;
			double c = sourcePoints[m].abs3;
			double wj = sourcePoints[m].weight;
			double xp = xj[0]*a + xj[1]*b + xj[2]*c;
			double yp = yj[0]*a + yj[1]*b + yj[2]*c;

			// Compute the numeric integration
			double R = sqrt((x-xp)*(x-xp) + (y-yp)*(y-yp));
			if (R <= tolerance) {
				result += wi*wj*(-math::CJ*k0);
			}
			else {
				result += wi*wj*(exp(-math::CJ*k0*R) - singExtract) / R;
			}
		}
	}
	return result;

	// Do the analytic integral
	if (selfCell) {
		std::vector<math::TrianglePoint> singPoints;
		math::gaussQuadTriangle(1, singPoints);

		double a = singPoints[0].abs1;
		double b = singPoints[0].abs2;
		double c = singPoints[0].abs3;
		double x = xi[0]*a + xi[1]*b + xi[2]*c;
		double y = yi[0]*a + yi[1]*b + yi[2]*c;

		// Compute the analytic portion of the singular integral
		double area = math::triangleArea(xi, yi);
		double sing = singularUniformIntegral(x, y, xi, yi);
		sing /= area;
		result += sing;
	}

	return result;
}

bool Solution::verbose() const
{
	return mVerbose;
}

void Solution::writeExcitationArray(const dcomplex_array & array, std::ostream & outputStream)
{
  outputStream << "Unknown, Real, Imag\n";
  for (std::size_t i = 0; i < array.size(); i++) {
    outputStream << i+1 << "," << array[i].real() << "," << array[i].imag() << std::endl;
  }
}

void Solution::writeFieldArray(const dcomplex_array &array, std::ostream &outputStream)
{
  outputStream << "Unknown, Real, Imag\n";
  for (std::size_t i = 0; i < array.size(); i++) {
    outputStream << i+1 << "," << array[i].real() << "," << array[i].imag() << std::endl;
  }
}


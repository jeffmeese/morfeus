#include "mesh/triangle.h"
#include "mesh/prism.h"
#include "mesh/mesh.h"

#include "math/constants.h"
#include "math/functions.h"
#include "math/gaussquad.h"

#include "excitation/planewave.h"

#include <cmath>

Triangle::Triangle()
  : Face(3, 3)
{
}

double Triangle::computeArea(const Mesh & mesh) const
{
	double x[3], y[3], z[3];
	for (std::size_t i = 0; i < totalNodes(); i++) {
		x[i] = mesh.getNode(globalNode(i))->x();
		y[i] = mesh.getNode(globalNode(i))->y();
		z[i] = mesh.getNode(globalNode(i))->z();
	}

	double xx[3], yy[3];
	for (size_t i = 0; i < 3; i++)
	{
		xx[i] = x[i];
		yy[i] = y[i];
	}

	math::centerPolygon(3, xx, yy);
	return math::triangleArea(xx, yy);
}

Face::FarFieldEntry Triangle::computeFarFieldEntry(double freq, double theta, double phi, const dcomplex_array & efield, const Mesh & mesh) const
{
  double k0 = math::frequencyToWaveNumber(freq);

	theta *= math::DEG_TO_RAD;
	phi *= math::DEG_TO_RAD;

		// Get the node positions
	double xi[3], yi[3], zi[3];
	for (size_t i = 0; i < 3; i++) {
		Node * node = mesh.getNode(globalNode(i));
		xi[i] = node->x();
		yi[i] = node->y();
		zi[i] = node->z();
	}

	// Compute the gaussian quadrature points
	std::vector<math::TrianglePoint> points;
	size_t numPoints = math::gaussQuadTriangle(4, points);

	dcomplex i1(math::ZERO), i2(math::ZERO);
	for (size_t i = 0; i < 3; i++) {
		Edge * edge = mesh.getEdge(globalEdge(i));
		int unknown = edge->unknown();
		if (unknown > 0) {
			double xj = xi[i];
			double yj = yi[i];

			double length = edge->computeLength(mesh);
			double sign = edgeSign(i);
			dcomplex Ei = efield[unknown-1];

			dcomplex ith(math::ZERO);
			dcomplex iphi(math::ZERO);
			for (size_t j = 0; j < numPoints; j++) {
				double a = points[j].abs1;
				double b = points[j].abs2;
				double c = points[j].abs3;
				double w = points[j].weight;

				double x = xi[0]*a + xi[1]*b + xi[2]*c;
				double y = yi[0]*a + yi[1]*b + yi[2]*c;

				double arg = x*cos(phi) + y*sin(phi);
				dcomplex phase = exp(math::CJ*k0*sin(theta)*arg);

				//double argx = (yj-y)*cos(theta)*sin(phi) - (x-xj)*cos(theta)*cos(phi);
				//double argy = (yj-y)*cos(phi) + (x-xj)*sin(phi);
				double argx = (x-xj)*cos(theta)*cos(phi) - (y-yj)*cos(theta)*sin(phi);
				double argy = (x-xj)*sin(phi) + (y-yj)*cos(phi);

				ith += w * argx * phase;
				iphi += w * argy * phase;
			}

			i1 -= (0.5*Ei*length*sign) * ith;
			i2 -= (0.5*Ei*length*sign) * iphi;
		}
	}

	FarFieldEntry entry;
	entry.i1 = i1;
	entry.i2 = i2;
	return entry;
}

dcomplex Triangle::computePlanewaveEntry(std::size_t localEdge, double freq, const Planewave * planewave, const Mesh & mesh) const
{
  double k0 = math::frequencyToWaveNumber(freq);

	double alpha = planewave->fieldPolarization() * math::DEG_TO_RAD;
	double theta = planewave->thetaIncident() * math::DEG_TO_RAD;
	double phi = planewave->phiIncident() * math::DEG_TO_RAD;

	// Get the node positions
	double xi[3], yi[3], zi[3];
	for (std::size_t i = 0; i < 3; i++) {
		Node * node = mesh.getNode(globalNode(i));
		xi[i] = node->x();
		yi[i] = node->y();
		zi[i] = node->z();
	}

	// Compute the gaussian quadrature points
	std::vector<math::TrianglePoint> points;
	size_t numPoints = math::gaussQuadTriangle(4, points);

	// Get the edge properties
	Edge * edge = mesh.getEdge(globalEdge(localEdge));

	// Compute the entry
	double xj = xi[localEdge];
	double yj = yi[localEdge];
	double length = edge->computeLength(mesh);
	double sign = edgeSign(localEdge);

	dcomplex result(math::ZERO);
	for (size_t j = 0; j < numPoints; j++) {
		double a = points[j].abs1;
		double b = points[j].abs2;
		double c = points[j].abs3;
		double w = points[j].weight;

		double x = xi[0]*a + xi[1]*b + xi[2]*c;
		double y = yi[0]*a + yi[1]*b + yi[2]*c;

		double arg = x*cos(phi) + y*sin(phi);
		dcomplex phase = std::exp(math::CJ*k0*sin(theta)*arg);
		double vx = sin(alpha)*cos(theta)*cos(phi) + cos(alpha)*sin(phi);
		double vy = sin(alpha)*cos(theta)*sin(phi) - cos(alpha)*cos(phi);
		double argx = (x-xj)*vx;
		double argy = (y-yj)*vy;

		result += w * (argx + argy) * phase;
	}

	result *= math::CJ*k0*length*sign;
	return result;
}

std::unique_ptr<Element> Triangle::extrudeElement(std::size_t id, std::size_t nodeOffset) const
{
  std::unique_ptr<Prism> prism = std::make_unique<Prism>(id);
  for (std::size_t i = 0; i < totalNodes(); i++) {
    prism->setGlobalNode(i, globalNode(i));
    prism->setGlobalNode(i+3,globalNode(i)+nodeOffset);
  }

  for (std::size_t i = 0; i < totalEdges(); i++) {
    prism->setGlobalEdge(i, globalEdge(i));
  }

  return std::move(prism);
}

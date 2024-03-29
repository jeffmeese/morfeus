#include "mesh/prism.h"
#include "mesh/face.h"
#include "mesh/edge.h"
#include "mesh/node.h"
#include "mesh/mesh.h"
#include "mesh/triangle.h"
#include "mesh/quadrilateral.h"

#include "math/constants.h"
#include "math/functions.h"

#include "material/material.h"

#include <array>
#include <iostream>

Prism::Prism(std::size_t id)
	: Element(id, 6, 9, 5)
{
	for (std::size_t i = 0; i < 9; i++)
		setEdgeSign(i, 1);
}

dcomplex Prism::computeMatrixEntry(const Material * eps, const Material * mu, const Mesh & mesh, double freq, std::size_t source, std::size_t test) const
{
  double k0 = math::frequencyToWaveNumber(freq);

	Edge * testEdge = mesh.getEdge(globalEdge(test));
	Edge * sourceEdge = mesh.getEdge(globalEdge(source));
	double testLength = testEdge->computeLength(mesh);
	double sourceLength = sourceEdge->computeLength(mesh);

	// Get the node positions
	double x[6], y[6], z[6];
	for (std::size_t i = 0; i < 6; i++) {
		Node * node = mesh.getNode(globalNode(i));
		x[i] = node->x();
		y[i] = node->y();
		z[i] = node->z();
	}

  // Center the triangle
  math::centerPolygon(6, x, y);

  // Compute some needed values
  double zl = z[3];
  double zu = z[0];
  double height = zu - zl;
  double area = math::triangleArea(x, y);

	dcomplex i1(math::ZERO);
	dcomplex i2(math::ZERO);

	if (test < 3) {

		// Top-Top
		if (source < 3) {
			double xi = x[test];
			double xj = x[source];
			double yi = y[test];
			double yj = y[source];

			double num = testLength * sourceLength;
			double den = 4.0 * height * height * area * area;
			double mu_coeff = num / den;

			double t1 = ixxs(xi, xj, x, y);
			double t2 = iyys(yi, yj, x, y);
			double t3 = izzl(zl, zu, zl, zl);

			i1 = height * t1 + height * t2 + 4.0 * area * t3;
			i1 *= mu_coeff;

			i2 = t3 * t1 + t3 * t2;
			i2 *= mu_coeff;
		}

		// Top-Bottom
		if (source >= 3 && source < 6) {
			double xi = x[test];
			double xj = x[source];
			double yi = y[test];
			double yj = y[source];

			double num = testLength * sourceLength;
			double den = 4.0 * height * height * area * area;
			double mu_coeff = -(num / den);

			double t1 = ixxs(xi, xj, x, y);
			double t2 = iyys(yi, yj, x, y);
			double t3 = izzl(zl, zu, zl, zu);

			i1 = height * t1 + height * t2 + 4.0 * area * t3;
			i1 *= mu_coeff;

			i2 = t3 * t1 +  t3 * t2;
			i2 *= mu_coeff;
		}

		// Top-Side
		if (source >= 6) {
			double xi = x[test];
			double yi = y[test];

			double num = testLength;
			double den = 4.0 * area * area;
			double mu_coeff = -(num / den);
			double t1 = ixs(xi, x, y);
			double t2 = iys(yi, x, y);
			std::size_t k1_s = 0, k2_s = 0;
			edgeConstants(source, k1_s, k2_s);

			i1 = (x[k2_s] - x[k1_s]) * t1 + (y[k2_s] - y[k1_s]) * t2;
			i1 *= mu_coeff;
		}
	}

	if (test >= 3 && test < 6) {

		// Bottom-Top
		if (source < 3) {
			double xi = x[test];
			double xj = x[source];
			double yi = y[test];
			double yj = y[source];

			double num = testLength * sourceLength;
			double den = 4.0 * height * height * area * area;
			double mu_coeff = -(num / den);

			double t1 = ixxs(xi, xj, x, y);
			double t2 = iyys(yi, yj, x, y);
			double t3 = izzl(zl, zu, zl, zu);

			i1 = height * t1 + height * t2 + 4.0 * area * t3;
			i1 *= mu_coeff;

			i2 = t3 * t1 + t3 * t2;
			i2 *= mu_coeff;
		}

		// Bottom-Bottom
		if (source >= 3 && source < 6) {
			double xi = x[test];
			double xj = x[source];
			double yi = y[test];
			double yj = y[source];

			double num = testLength * sourceLength;
			double den = 4.0 * height * height * area * area;
			double mu_coeff = (num / den);

			double t1 = ixxs(xi, xj, x, y);
			double t2 = iyys(yi, yj, x, y);
			double t3 = izzl(zl, zu, zu, zu);

			i1 = height * t1 + height * t2 + 4.0 * area * t3;
			i1 *= mu_coeff;

			i2 = t3 * t1 + t3 * t2;
			i2 *= mu_coeff;
		}

		// Bottom-Side
		if (source >= 6) {
			double xi = x[test];
			double yi = y[test];

			double num = testLength;
			double den = 4.0 * area * area;
			double mu_coeff = (num / den);

			double t1 = ixs(xi, x, y);
			double t2 = iys(yi, x, y);

			size_t k1_s = 0, k2_s = 0;
			edgeConstants(source, k1_s, k2_s);

			i1 = (x[k2_s] - x[k1_s]) * t1 + (y[k2_s] - y[k1_s]) * t2;
			i1 *= mu_coeff;
		}
	}

	if (test >= 6) {

		// Side-Top
		if (source < 3) {
			double xj = x[source];
			double yj = y[source];

			double num = sourceLength;
			double den = 4.0 * area * area;
			double mu_coeff = -(num / den);

			double t1 = ixs(xj, x, y);
			double t2 = iys(yj, x, y);

			std::size_t k1_t = 0, k2_t = 0;
			edgeConstants(test, k1_t, k2_t);

			i1 = (x[k2_t] - x[k1_t]) * t1 + (y[k2_t] - y[k1_t]) * t2;
			i1 *= mu_coeff;
		}

		// Side-Bottom
		if (source >= 3 && source < 6) {
			double xj = x[source];
			double yj = y[source];

			double num = sourceLength;
			double den = 4.0 * area * area;
			double mu_coeff = (num / den);

			double t1 = ixs(xj, x, y);
			double t2 = iys(yj, x, y);

			std::size_t k1_t = 0, k2_t = 0;
			edgeConstants(test, k1_t, k2_t);

			i1 = (x[k2_t] - x[k1_t]) * t1 + (y[k2_t] - y[k1_t]) * t2;
			i1 *= mu_coeff;
		}

		// Side-Side
		if (source >= 6) {

			std::size_t k1_t = 0, k2_t = 0, k1_s = 0, k2_s = 0;
			edgeConstants(test, k1_t, k2_t);
			edgeConstants(source, k1_s, k2_s);

			double Ci = (x[k1_t]*y[k2_t] - x[k2_t] * y[k1_t]);
			double Cj = (x[k1_s]*y[k2_s] - x[k2_s] * y[k1_s]);
			double Xi = (x[k2_t] - x[k1_t]);
			double Xj = (x[k2_s] - x[k1_s]);
			double Yi = (y[k1_t] - y[k2_t]);
			double Yj = (y[k1_s] - y[k2_s]);

			double mu_coeff = height / (4.0 * area);

			double t1 = (x[k2_t] - x[k1_t]) * (x[k2_s] - x[k1_s]);
			double t2 = (y[k2_t] - y[k1_t]) * (y[k2_s] - y[k1_s]);

			i1 = t1 + t2;
			i1 *= mu_coeff;

			double t3 = Ci*Cj*area;
			double t4 = (Xi*Yj+Yi*Xj) * sxy(x,y);
			double t5 = (Yi*Yj) * sxx(x,y);
			double t6 = (Xi*Xj) * syy(x,y);

			i2 = (t3 + t4 + t5 + t6);
			i2 *= mu_coeff / area;
		}
	}

	// Only have isotropic materials right now
	dcomplex epsValue = eps->value(Material::Direction::xx);
	dcomplex muValue = mu->value(Material::Direction::xx);

	// Get the edge signs
	int testSign = edgeSign(test);
	int sourceSign = edgeSign(source);

	dcomplex matrixEntry = static_cast<double>(sourceSign * testSign) * (i1/muValue - k0*k0*i2*epsValue);
	return matrixEntry;
}

std::unique_ptr<Element> Prism::copy(size_t newId) const
{
	auto newPrism(std::make_unique<Prism>(newId));

	for (std::size_t i = 0; i < 6; i++)
		newPrism->setGlobalNode(i, globalNode(i));

	for (std::size_t i = 0; i < 9; i++)
		newPrism->setGlobalEdge(i, globalEdge(i));

	return std::move(newPrism);
}

std::unique_ptr<Face> Prism::createFace(size_t localFace) const
{
	static const std::size_t numNodes[] = {3, 4, 4, 4, 3};
	static const std::size_t numEdges[] = {3, 4, 4, 4, 3};
	static const Face::Position positions[] =
	{
		Face::Position::Top,
		Face::Position::Side,
		Face::Position::Side,
		Face::Position::Side,
		Face::Position::Bottom
	};

	using md_array = std::array<std::array<std::size_t, 4>, 5>;
	md_array localNodes = {{{0, 1, 2, 0}, {0, 2, 5, 3}, {1, 0, 3, 4}, {2, 1, 4, 5}, {3, 4, 5, 0}}};
	md_array localEdges = {{{0, 1, 2, 0}, {1, 8, 4, 6}, {2, 6, 5, 7}, {0, 7, 3, 8}, {3, 4, 5, 0}}};

	std::unique_ptr<Face> newFace;
	if (localFace == 0 || localFace == 4) {
		newFace.reset(new Triangle);
	}
	else
		newFace.reset(new Quadrilateral);

	newFace->setPosition(positions[localFace]);
	for (std::size_t i = 0; i < numNodes[localFace]; i++)
		newFace->setGlobalNode(i, globalNode(localNodes[localFace][i]));

	for (std::size_t i= 0; i < numEdges[localFace]; i++) {
		newFace->setGlobalEdge(i, globalEdge(localEdges[localFace][i]));
		newFace->setEdgeSign(i, edgeSign(localEdges[localFace][i]));
	}

//	if (localFace == 0) {
//		newFace.reset(new Triangle);
//		newFace->setPosition(Face::Position::Top);
//		newFace->setGlobalNode(0, globalNode(0));
//		newFace->setGlobalNode(1, globalNode(1));
//		newFace->setGlobalNode(2, globalNode(2));
//		newFace->setGlobalEdge(0, globalEdge(0));
//		newFace->setGlobalEdge(1, globalEdge(1));
//		newFace->setGlobalEdge(2, globalEdge(2));
//		newFace->setEdgeSign(0, edgeSign(0));
//		newFace->setEdgeSign(1, edgeSign(1));
//		newFace->setEdgeSign(2, edgeSign(2));
//	}
//	else if (localFace == 1) {
//		newFace.reset(new Quadrilateral);
//		newFace->setPosition(Face::Position::Side);
//		newFace->setGlobalNode(0, globalNode(0));
//		newFace->setGlobalNode(1, globalNode(2));
//		newFace->setGlobalNode(2, globalNode(5));
//		newFace->setGlobalNode(3, globalNode(3));
//		newFace->setGlobalEdge(0, globalEdge(1));
//		newFace->setGlobalEdge(1, globalEdge(8));
//		newFace->setGlobalEdge(2, globalEdge(4));
//		newFace->setGlobalEdge(3, globalEdge(6));
//		newFace->setEdgeSign(0, 1);
//		newFace->setEdgeSign(1, 1);
//		newFace->setEdgeSign(2, 1);
//		newFace->setEdgeSign(3, 1);
//	}
//	else if (localFace == 2) {
//		newFace.reset(new Quadrilateral);
//		newFace->setPosition(Face::Position::Side);
//		newFace->setGlobalNode(0, globalNode(1));
//		newFace->setGlobalNode(1, globalNode(0));
//		newFace->setGlobalNode(2, globalNode(3));
//		newFace->setGlobalNode(3, globalNode(4));
//		newFace->setGlobalEdge(0, globalEdge(2));
//		newFace->setGlobalEdge(1, globalEdge(6));
//		newFace->setGlobalEdge(2, globalEdge(5));
//		newFace->setGlobalEdge(3, globalEdge(7));
//		newFace->setEdgeSign(0, 1);
//		newFace->setEdgeSign(1, 1);
//		newFace->setEdgeSign(2, 1);
//		newFace->setEdgeSign(3, 1);
//	}
//	else if (localFace == 3) {
//		newFace.reset(new Quadrilateral);
//		newFace->setPosition(Face::Position::Side);
//		newFace->setGlobalNode(0, globalNode(2));
//		newFace->setGlobalNode(1, globalNode(1));
//		newFace->setGlobalNode(2, globalNode(4));
//		newFace->setGlobalNode(3, globalNode(5));
//		newFace->setGlobalEdge(0, globalEdge(0));
//		newFace->setGlobalEdge(1, globalEdge(7));
//		newFace->setGlobalEdge(2, globalEdge(3));
//		newFace->setGlobalEdge(3, globalEdge(8));
//		newFace->setEdgeSign(0, 1);
//		newFace->setEdgeSign(1, 1);
//		newFace->setEdgeSign(2, 1);
//		newFace->setEdgeSign(3, 1);
//	}
//	else if (localFace == 4) {
//		newFace.reset(new Triangle);
//		newFace->setPosition(Face::Position::Bottom);
//		newFace->setGlobalNode(0, globalNode(3));
//		newFace->setGlobalNode(1, globalNode(4));
//		newFace->setGlobalNode(2, globalNode(5));
//		newFace->setGlobalEdge(0, globalEdge(3));
//		newFace->setGlobalEdge(1, globalEdge(4));
//		newFace->setGlobalEdge(2, globalEdge(5));
//		newFace->setEdgeSign(0, edgeSign(3));
//		newFace->setEdgeSign(1, edgeSign(4));
//		newFace->setEdgeSign(2, edgeSign(5));
//	}

//	else {
//		newFace->setPosition(positions[localFace]);
//		for (std::size_t i = 0; i < numNodes[localFace]; i++)
//			newFace->setGlobalNode(i, globalNode(localNodes[localFace][i]));

//		for (std::size_t i= 0; i < numEdges[localFace]; i++) {
//			newFace->setGlobalEdge(i, globalEdge(localEdges[localFace][i]));
//			newFace->setEdgeSign(i, edgeSign(localEdges[localFace][i]));
//		}
	//}

	return std::move(newFace);
}

void Prism::edgeConstants(std::size_t edge, std::size_t& k1, std::size_t& k2) const
{
  static const std::size_t k1_array[] = {0,0,0,0,0,0,1,2,0};
  static const std::size_t k2_array[] = {0,0,0,0,0,0,2,0,1};
  k1 = k1_array[edge];
  k2 = k2_array[edge];
}

void Prism::getEdgeNodes(std::size_t index, std::size_t &node1, std::size_t &node2) const
{
	static const std::size_t n1[] = {1, 2, 0, 4, 5, 3, 0, 1, 2};
	static const std::size_t n2[] = {2, 0, 1, 5, 3, 4, 3, 4, 5};
	node1 = n1[index];
	node2 = n2[index];
}

double Prism::ixs(double a, double const * x, double const * y) const
{
	return (-a*math::triangleArea(x,y));
}

double Prism::iys(double a, double const * x, double const * y) const
{
	return (-a*math::triangleArea(x,y));
}

double Prism::ixxs(double a, double b, double const * x, double const * y) const
{
	return (sxx(x,y) + a*b*math::triangleArea(x,y) );
}

double Prism::iyys(double a, double b, double const * x, double const * y) const
{
	return (syy(x,y) + a*b*math::triangleArea(x,y) );
}

double Prism::izzl(double zl, double zu, double a, double b) const
{
  double t1 = (zu*zu*zu - zl*zl*zl) / 3.0;
  double t2 = ( (zl*zl - zu*zu) / 2.0) * (a+b);
  double t3 = (zu-zl) * a * b;
  return (t1 + t2 + t3);
}

double Prism::sxx(double const * x, double const * y) const
{
  double ret = 0.0;
  for (int node = 0; node < 3; node++)
    ret += x[node]*x[node];

  ret = math::triangleArea(x,y) / 12.0*ret;
  return ret;
}

double Prism::syy(double const * x, double const * y) const
{
  double ret = 0.0;
  for (int node = 0; node < 3; node++)
    ret += y[node]*y[node];

  ret = math::triangleArea(x,y) / 12.0*ret;
  return ret;
}

double Prism::sxy(double const * x, double const * y) const
{
  double ret = 0.0;
  for (int node = 0; node < 3; node++)
    ret += x[node]*y[node];

  ret = math::triangleArea(x,y) / 12.0*ret;
  return ret;
}

#include "prism.h"

#include "edge.h"
#include "mesh.h"
#include "node.h"
#include "triangle.h"

#include "math/functions.h"

namespace morfeus {
namespace mesh {

static const std::string OBJECT_ID("Prism");

Prism::Prism()
  : Element(OBJECT_ID, 6, 9, 5)
{
  createFaces();
}

Prism::Prism(int32_t number)
  : Element(OBJECT_ID, number, 6, 9, 5)
{
  createFaces();
}

void Prism::createFaces()
{
  mFaces.push_back(TrianglePtr(new Triangle));
  mFaces.push_back(TrianglePtr(new Triangle));
  mFaces.push_back(TrianglePtr(new Triangle));
  mFaces.push_back(TrianglePtr(new Triangle));
  mFaces.push_back(TrianglePtr(new Triangle));
}

void Prism::doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const
{
  const Edge * sourceEdge = mesh->edge(edge(localEdge1));
  const Edge * testEdge = mesh->edge(edge(localEdge2));

  // Get the edge signs
  int32_t testSign = edgeSign(localEdge1);
  int32_t sourceSign = edgeSign(localEdge2);

  double testLength = testEdge->computeLength(mesh);
  double sourceLength = sourceEdge->computeLength(mesh);

  // Get the node positions
  double x[6], y[6], z[6];
  for (std::size_t i = 0; i < 6; i++) {
    const Node * elemNode = mesh->node(node(i));
    x[i] = elemNode->x();
    y[i] = elemNode->y();
    z[i] = elemNode->z();
  }

  // Center the triangle
  math::centerPolygon(6, x, y);

  // Compute some needed values
  double zl = z[3];
  double zu = z[0];
  double height = zu - zl;
  double area = math::triangleArea(x, y);

  if (localEdge1 < 3) {

    // Top-Top
    if (localEdge2 < 3) {
      double xi = x[localEdge1];
      double xj = x[localEdge2];
      double yi = y[localEdge1];
      double yj = y[localEdge2];

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
    if (localEdge2 >= 3 && localEdge2 < 6) {
      double xi = x[localEdge1];
      double xj = x[localEdge2];
      double yi = y[localEdge1];
      double yj = y[localEdge2];

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
    if (localEdge2 >= 6) {
      double xi = x[localEdge1];
      double yi = y[localEdge1];

      double num = testLength;
      double den = 4.0 * area * area;
      double mu_coeff = -(num / den);
      double t1 = ixs(xi, x, y);
      double t2 = iys(yi, x, y);
      std::size_t k1_s = 0, k2_s = 0;
      edgeConstants(localEdge2, k1_s, k2_s);

      i1 = (x[k2_s] - x[k1_s]) * t1 + (y[k2_s] - y[k1_s]) * t2;
      i1 *= mu_coeff;
    }
  }

  if (localEdge1 >= 3 && localEdge1 < 6) {

    // Bottom-Top
    if (localEdge2 < 3) {
      double xi = x[localEdge1];
      double xj = x[localEdge2];
      double yi = y[localEdge1];
      double yj = y[localEdge2];

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
    if (localEdge2 >= 3 && localEdge2 < 6) {
      double xi = x[localEdge1];
      double xj = x[localEdge2];
      double yi = y[localEdge1];
      double yj = y[localEdge2];

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
    if (localEdge2 >= 6) {
      double xi = x[localEdge1];
      double yi = y[localEdge1];

      double num = testLength;
      double den = 4.0 * area * area;
      double mu_coeff = (num / den);

      double t1 = ixs(xi, x, y);
      double t2 = iys(yi, x, y);

      size_t k1_s = 0, k2_s = 0;
      edgeConstants(localEdge2, k1_s, k2_s);

      i1 = (x[k2_s] - x[k1_s]) * t1 + (y[k2_s] - y[k1_s]) * t2;
      i1 *= mu_coeff;
    }
  }

  if (localEdge1 >= 6) {

    // Side-Top
    if (localEdge2 < 3) {
      double xj = x[localEdge2];
      double yj = y[localEdge2];

      double num = sourceLength;
      double den = 4.0 * area * area;
      double mu_coeff = -(num / den);

      double t1 = ixs(xj, x, y);
      double t2 = iys(yj, x, y);

      std::size_t k1_t = 0, k2_t = 0;
      edgeConstants(localEdge1, k1_t, k2_t);

      i1 = (x[k2_t] - x[k1_t]) * t1 + (y[k2_t] - y[k1_t]) * t2;
      i1 *= mu_coeff;
    }

    // Side-Bottom
    if (localEdge2 >= 3 && localEdge2 < 6) {
      double xj = x[localEdge2];
      double yj = y[localEdge2];

      double num = sourceLength;
      double den = 4.0 * area * area;
      double mu_coeff = (num / den);

      double t1 = ixs(xj, x, y);
      double t2 = iys(yj, x, y);

      std::size_t k1_t = 0, k2_t = 0;
      edgeConstants(localEdge1, k1_t, k2_t);

      i1 = (x[k2_t] - x[k1_t]) * t1 + (y[k2_t] - y[k1_t]) * t2;
      i1 *= mu_coeff;
    }

    // Side-Side
    if (localEdge2 >= 6) {

      std::size_t k1_t = 0, k2_t = 0, k1_s = 0, k2_s = 0;
      edgeConstants(localEdge1, k1_t, k2_t);
      edgeConstants(localEdge2, k1_s, k2_s);

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
}

void Prism::doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const
{
  static const int edge_node1[] = {0, 1, 2, 3, 4, 5, 0, 1, 2};
  static const int edge_node2[] = {1, 2, 0, 4, 5, 3, 6, 7, 8};
  localNode1 = edge_node1[index];
  localNode2 = edge_node2[index];
}

Face * Prism::doGetFace(std::size_t index) const
{
  Triangle * face = mFaces.at(index).get();
  return face;
}

void Prism::doPrint(std::ostream &output, int tabPos) const
{
}

void Prism::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Prism::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

void Prism::edgeConstants(std::size_t edge, std::size_t& k1, std::size_t& k2) const
{
  static const std::size_t k1_array[] = {0,0,0,0,0,0,1,2,0};
  static const std::size_t k2_array[] = {0,0,0,0,0,0,2,0,1};
  k1 = k1_array[edge];
  k2 = k2_array[edge];
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

}
}

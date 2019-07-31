#include "triangle.h"

#include "edge.h"
#include "mesh.h"
#include "node.h"

#include "excitations/planewave.h"

#include "math/constants.h"
#include "math/functions.h"
#include "math/gaussquad.h"

namespace morfeus {
namespace mesh {

Triangle::Triangle()
  : Face("TriangleFace", 3, 3)
{
}

Triangle::Triangle(int32_t number)
  : Face("TriangleFace", number, 3, 3)
{
}

double Triangle::doComputeArea(const Mesh * mesh) const
{
  return 0.0;
}

dcomplex Triangle::doComputeMomEntry(const Face *otherFace, const Mesh * mesh, std::size_t localEdge) const
{
  dcomplex value(0.0,0.0);
  return value;
}

dcomplex Triangle::doComputePlanewaveEntry(std::size_t localEdge, double freq, double alpha, double theta, double phi, const Mesh *mesh) const
{
  dcomplex result(math::zero);

  double k0 = math::frequencyToWavenumber(freq);

  // Get the node positions
  double xi[3], yi[3], zi[3];
  for (std::size_t i = 0; i < 3; i++) {
    const Node * faceNode = mesh->node(node(i));
    xi[i] = faceNode->x();
    yi[i] = faceNode->y();
    zi[i] = faceNode->z();
  }

  // Compute the gaussian quadrature points
  std::vector<math::TrianglePoint> points;
  std::size_t numPoints = math::gaussQuadTriangle(4, points);

  // Get the edge properties
  const Edge * faceEdge = mesh->edge(edge(localEdge));

  // Compute the entry
  double xj = xi[localEdge];
  double yj = yi[localEdge];
  double length = faceEdge->computeLength(mesh);
  double sign = edgeSign(localEdge);

  for (size_t j = 0; j < numPoints; j++) {
    double a = points[j].abs1;
    double b = points[j].abs2;
    double c = points[j].abs3;
    double w = points[j].weight;

    double x = xi[0]*a + xi[1]*b + xi[2]*c;
    double y = yi[0]*a + yi[1]*b + yi[2]*c;

    double arg = x*cos(phi) + y*sin(phi);
    dcomplex phase = std::exp(math::cj*k0*sin(theta)*arg);
    double vx = sin(alpha)*cos(theta)*cos(phi) + cos(alpha)*sin(phi);
    double vy = sin(alpha)*cos(theta)*sin(phi) - cos(alpha)*cos(phi);
    double argx = (x-xj)*vx;
    double argy = (y-yj)*vy;

    result += w * (argx + argy) * phase;
  }

  result *= math::cj*k0*length*sign;
  return result;
}

void Triangle::doPrint(std::ostream &output, int tabPos) const
{
}

void Triangle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Triangle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

}
}

#include "edge.h"

#include "mesh.h"
#include "node.h"

#include <cmath>

namespace morfeus {
namespace mesh {

Edge::Edge(int32_t number)
  : MorfeusObject("Edge")
  , mNumber(number)
  , mNode1(-1)
  , mNode2(-1)
  , mUnknown(-1)
{
}

Edge::Edge(int32_t number, int32_t node1, int32_t node2)
  : MorfeusObject("Edge")
  , mNumber(number)
  , mNode1(node1)
  , mNode2(node2)
  , mUnknown(-1)
{
}

double Edge::computeLength(const Mesh *mesh) const
{
  const Node * node1 = mesh->node(mNode1);
  const Node * node2 = mesh->node(mNode2);

  double x1 = node1->x();
  double y1 = node1->y();
  double z1 = node1->z();

  double x2 = node2->x();
  double y2 = node2->y();
  double z2 = node2->z();

  return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );
}

void Edge::doPrint(std::ostream &output, int tabPos) const
{
}

void Edge::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Edge::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

}
}

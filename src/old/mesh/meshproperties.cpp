#include "mesh/meshproperties.h"
#include "mesh/mesh.h"

MeshProperties::MeshProperties(const Mesh & mesh)
{
  rescan(mesh);
}

const std::vector<Edge*> & MeshProperties::apertureEdges() const
{
  return mApertureEdges;
}

const std::vector<Node*> & MeshProperties::apertureNodes() const
{
  return mApertureNodes;
}

void MeshProperties::rescan(const Mesh &mesh)
{
  mTotalApertureEdges = mTotalApertureNodes = 0;
  mTotalApertureUnknowns = mTotalUnknowns = 0;
  //mTotalApertureElements = 0;

  mApertureEdges.clear();
  mApertureNodes.clear();

  for (std::size_t i = 0; i < mesh.totalNodes(); i++) {
    Node * node = mesh.getNode(i);
    bool apertureNode = (node->z() >= 0.0);
    if (apertureNode) {
      mApertureNodes.push_back(node);
    }
  }

  for (std::size_t i = 0; i < mesh.totalEdges(); i++) {
    Edge * edge = mesh.getEdge(i);
    Node * node1 = mesh.getNode(edge->globalNode1());
    Node * node2 = mesh.getNode(edge->globalNode2());

    bool apertureNode1 = (node1->z() >= 0.0);
    bool apertureNode2 = (node2->z() >= 0.0);
    bool apertureEdge = (apertureNode1 && apertureNode2);

    if (apertureEdge)
      mApertureEdges.push_back(edge);

    if (edge->unknown() > 0)
      mTotalUnknowns++;
  }

  mTotalApertureNodes = mApertureNodes.size();
  mTotalApertureEdges = mApertureEdges.size();

  for (std::size_t i = 0; i < mApertureEdges.size(); i++) {
    Edge * edge = mApertureEdges.at(i);
    if (edge->unknown() > 0)
      mTotalApertureUnknowns++;
  }
}

std::size_t MeshProperties::totalApertureEdges() const
{
  return mTotalApertureEdges;
}

//std::size_t MeshProperties::totalApertureElements() const
//{
//  return mTotalApertureElements;
//}

std::size_t MeshProperties::totalApertureNodes() const
{
  return mTotalApertureNodes;
}

std::size_t MeshProperties::totalApertureUnknowns() const
{
  return mTotalApertureUnknowns;
}

std::size_t MeshProperties::totalUnknowns() const
{
  return mTotalUnknowns;
}

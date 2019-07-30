#include "meshinformation.h"

#include "mesh/edge.h"
#include "mesh/element.h"
#include "mesh/face.h"
#include "mesh/mesh.h"

#include <vector>

namespace morfeus {

MeshInformation::MeshInformation()
{
  init();
}

void MeshInformation::analyzeMesh(mesh::Mesh * mesh)
{
  init();
  identifyBoundaryFaces(mesh);
  markMeshUnknowns(mesh);

  std::cout << "\n";
  std::cout << "Total Nodes: " << mesh->totalNodes() << "\n";
  std::cout << "Total Edges: " << mesh->totalEdges() << "\n";
  std::cout << "Total Elements: " << mesh->totalElements() << "\n";
  std::cout << "Total Boundary Faces: " << mBoundaryFaces.size() << "\n";
  std::cout << "Total Unknowns: " << mTotalUnknowns << "\n";
}

void MeshInformation::identifyBoundaryFaces(mesh::Mesh *mesh)
{
  mBoundaryFaces.clear();

  std::map<int32_t, std::vector<mesh::Face*> > faceMap;
  for (std::size_t i = 0; i < mesh->totalElements(); i++) {
    mesh::Element * element = mesh->element(i);
    for (std::size_t j = 0; j < element->totalFaces(); j++) {
      mesh::Face * face = element->constructFace(j);
      int32_t nodeSum = 0;
      for (std::size_t k = 0; k < face->totalNodes(); k++) {
        nodeSum += face->node(k);
      }
      faceMap[nodeSum].push_back(face);
      mFaceToElement[face] = element;
    }
  }

  for (std::map<int32_t, std::vector<mesh::Face*> >::iterator itr = faceMap.begin(); itr != faceMap.end(); ++itr) {
    std::vector<mesh::Face*> & v = itr->second;
    while (v.size() > 0) {
      mesh::Face * face1 = v.back();
      v.pop_back();

      bool isBoundaryFace = true;
      for (std::vector<mesh::Face*>::iterator v_itr = v.begin(); v_itr != v.end(); v_itr++) {
        mesh::Face * face2(*v_itr);
        if (face1->isCoincident(face2)) {
          isBoundaryFace = false;
          v.erase(v_itr);
          break;
        }
      }

      if (isBoundaryFace) {
        mBoundaryFaces.push_back(face1);
      }
    }
  }
}

void MeshInformation::init()
{
  mTotalUnknowns = mTotalBoundaryUnknowns = 0;
  mBoundaryFaces.clear();
}

void MeshInformation::markMeshUnknowns(mesh::Mesh *mesh)
{
  for (std::size_t i = 0; i < mesh->totalEdges(); i++) {
    mesh::Edge * edge = mesh->edge(i);
    edge->setUnknownNumber(-1);
  }

  // Mark off all metallic elements
  for (std::size_t i = 0; i < mesh->totalElements(); i++) {
    mesh::Element * element = mesh->element(i);

    int32_t attribute = element->attribute();
    if (attribute == mesh::Element::MetallicElement) {
      for (std::size_t j = 0; j < element->totalEdges(); j++) {
        mesh::Edge * edge = mesh->edge(element->edge(j));
        edge->setUnknownNumber(0);
      }
    }
  }

  // Mark boundary face edges
  for (std::size_t i = 0; i < mBoundaryFaces.size(); i++) {
    mesh::Face * face = mBoundaryFaces.at(i);
    mesh::Element * element = mFaceToElement[face];
    if (element->attribute() == mesh::Element::MetallicBoundary) {
      for (std::size_t j = 0; j < face->totalEdges(); j++) {
        int32_t edgeNumber = face->edge(j);
        mesh::Edge * meshEdge = mesh->edge(edgeNumber);
        meshEdge->setUnknownNumber(0);
      }
    }
  }

  mTotalUnknowns = 0;
  for (std::size_t i = 0; i < mesh->totalEdges(); i++) {
    mesh::Edge * edge = mesh->edge(i);
    if (edge->unknownNumber() < 0) {
      edge->setUnknownNumber(++mTotalUnknowns);
    }
  }
}

}

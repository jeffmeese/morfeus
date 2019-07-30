#include "planewave.h"

#include "mesh/edge.h"
#include "mesh/face.h"
#include "mesh/mesh.h"
#include "solution/meshinformation.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {

static const std::string OBJECT_ID("Planewave");

Planewave::Planewave()
  : Excitation(OBJECT_ID)
{
  mFieldPolarization = 0.0;
  setAngleDependent(true);
}

Planewave::Planewave(const std::string & name)
  : Excitation (OBJECT_ID, name)
{
  mFieldPolarization = 0.0;
  setAngleDependent(true);
}

Planewave::Planewave(const std::string & id, const std::string & name)
  : Excitation (OBJECT_ID, id, name)
{
  mFieldPolarization = 0.0;
  setAngleDependent(true);
}

void Planewave::doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh *mesh, const MeshInformation *meshInfo, vector &rhs) const
{
  std::vector<mesh::Face *> boundaryFaces = meshInfo->boundaryFaces();

  for (size_t i = 0; i < boundaryFaces.size(); i++) {
    const mesh::Face * boundaryFace = boundaryFaces.at(i);

    for (size_t j = 0; j < boundaryFace->totalEdges(); j++) {
      const mesh::Edge * edge = mesh->edge(boundaryFace->edge(j));
      int unknown = edge->unknownNumber();
      if (unknown > 0) {
        dcomplex result = boundaryFace->computePlanewaveEntry(j, freqGHz, mFieldPolarization, thetaInc, phiInc, mesh);
        rhs[unknown-1] += result;
      }
    }
  }
}

void Planewave::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Field Polarization: ", mFieldPolarization);
}

void Planewave::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setFieldPolarization(xmlutils::readAttribute<double>(node, "alpha"));
}

void Planewave::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "alpha", mFieldPolarization);
}

namespace  {
  const bool r = Excitation::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Planewave*>()));
}

}

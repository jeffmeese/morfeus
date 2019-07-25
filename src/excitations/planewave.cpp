#include "planewave.h"

#include "edge.h"
#include "face.h"
#include "mesh.h"
#include "meshinformation.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

static const std::string OBJECT_ID("Planewave");

Planewave::Planewave()
  : Excitation(OBJECT_ID)
{
  mFieldPolarization = mThetaIncident = mPhiIncident = 0.0;
}

Planewave::Planewave(const std::string & name)
  : Excitation (OBJECT_ID, name)
{
  mFieldPolarization = mThetaIncident = mPhiIncident = 0.0;
}

Planewave::Planewave(const std::string & id, const std::string & name)
  : Excitation (OBJECT_ID, id, name)
{
  mFieldPolarization = mThetaIncident = mPhiIncident = 0.0;
}

void Planewave::doExcite(double freqGHz, const Mesh *mesh, const MeshInformation *meshInfo, vector &rhs) const
{
  std::vector<const Face *> boundaryFaces = meshInfo->boundaryFaces();

  for (size_t i = 0; i < boundaryFaces.size(); i++) {
    const Face * boundaryFace = boundaryFaces.at(i);
    //if (boundaryFace->position() == Face::Position::Top) {
      for (size_t j = 0; j < boundaryFace->totalEdges(); j++) {
        const Edge * edge = mesh->edge(boundaryFace->edge(j));
        int unknown = edge->unknownNumber();
        if (unknown > 0) {
          dcomplex result = boundaryFace->computePlanewaveEntry(j, freqGHz, this, mesh);
          rhs[unknown-1] += result;
        }
      }
    //}
  }
}

void Planewave::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Field Polarization: ", mFieldPolarization);
  xmlutils::printValue(output, tabPos, "Theta Incident: ", mThetaIncident);
  xmlutils::printValue(output, tabPos, "Phi Incident: ", mPhiIncident);
}

void Planewave::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setFieldPolarization(xmlutils::readAttribute<double>(node, "alpha"));
  setThetaIncident(xmlutils::readAttribute<double>(node, "theta"));
  setPhiIncident(xmlutils::readAttribute<double>(node, "phi"));
}

void Planewave::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "alpha", mFieldPolarization);
  xmlutils::writeAttribute(document, node, "theta", mThetaIncident);
  xmlutils::writeAttribute(document, node, "phi", mPhiIncident);
}

namespace  {
  const bool r = Excitation::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Planewave*>()));
}

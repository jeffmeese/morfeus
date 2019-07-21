#include "planewave.h"

static const std::string OBJECT_ID("Planewave");

Planewave::Planewave()
{
}

Planewave::Planewave(int32_t number)
  : Excitation (number)
{
  mFieldPolarization = mThetaIncident = mPhiIncident = 0.0;
}

void Planewave::doExcite(double freqGHz, const Mesh *mesh, const MeshInformation *meshInfo, vector &rhs) const
{

}

void Planewave::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "Field Polarization: ", mFieldPolarization);
  xmlutils::printValue(output, tabPos+2, "Theta Incident: ", mThetaIncident);
  xmlutils::printValue(output, tabPos+2, "Phi Incident: ", mPhiIncident);
}

void Planewave::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setFieldPolarization(std::stod(xmlutils::readAttribute<std::string>(node, "alpha")));
  setThetaIncident(std::stod(xmlutils::readAttribute<std::string>(node, "theta")));
  setPhiIncident(std::stod(xmlutils::readAttribute<std::string>(node, "phi")));
}

void Planewave::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "alpha", mFieldPolarization);
  xmlutils::writeAttribute(document, node, "theta", mThetaIncident);
  xmlutils::writeAttribute(document, node, "phi", mPhiIncident);
}

namespace  {
  Excitation * createFunc()
  {
    return new Planewave;
  }

  const bool registered = Excitation::Factory::Instance().Register(OBJECT_ID, createFunc);
}

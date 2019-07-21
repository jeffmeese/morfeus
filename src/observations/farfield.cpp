#include "farfield.h"

#include "mesh.h"
#include "meshinformation.h"

static const std::string OBJECT_ID("FarField");

FarField::FarField()
{

}

FarField::FarField(int32_t number)
  : Observation (number)
{
}

void FarField::doCalculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield)
{

}

void FarField::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Far Field");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "Theta Start: ", mThetaStart);
  xmlutils::printValue(output, tabPos+2, "Theta Stop: ", mThetaStop);
  xmlutils::printValue(output, tabPos+2, "Theta Increment: ", mThetaIncr);
  xmlutils::printValue(output, tabPos+2, "Phi Start: ", mPhiStart);
  xmlutils::printValue(output, tabPos+2, "Phi Stop: ", mPhiStop);
  xmlutils::printValue(output, tabPos+2, "Phi Increment: ", mPhiIncr);
}

void FarField::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setThetaStart(std::stod(xmlutils::readAttribute<std::string>(node, "theta-start")));
  setThetaStop(std::stod(xmlutils::readAttribute<std::string>(node, "theta-stop")));
  setThetaIncr(std::stod(xmlutils::readAttribute<std::string>(node, "theta-incr")));
  setPhiStart(std::stod(xmlutils::readAttribute<std::string>(node, "phi-start")));
  setPhiStop(std::stod(xmlutils::readAttribute<std::string>(node, "phi-stop")));
  setPhiIncr(std::stod(xmlutils::readAttribute<std::string>(node, "phi-incr")));
}

void FarField::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "theta-start", mThetaStart);
  xmlutils::writeAttribute(document, node, "theta-stop", mThetaStop);
  xmlutils::writeAttribute(document, node, "theta-incr", mThetaIncr);
  xmlutils::writeAttribute(document, node, "phi-start", mPhiStart);
  xmlutils::writeAttribute(document, node, "phi-stop", mPhiStop);
  xmlutils::writeAttribute(document, node, "phi-incr", mPhiIncr);
}

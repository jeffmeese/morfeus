#include "farfield.h"

#include "mesh.h"
#include "meshinformation.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

static const std::string OBJECT_ID("Far Field");

FarField::FarField()
  : Observation (OBJECT_ID)
{
}

FarField::FarField(const std::string & name)
  : Observation (OBJECT_ID, name)
{
}

FarField::FarField(const std::string & id, const std::string & name)
  : Observation (OBJECT_ID, id, name)
{
}

void FarField::doCalculate(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, const vector & efield)
{
}

void FarField::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Theta Start: ", mThetaStart);
  xmlutils::printValue(output, tabPos, "Theta Stop: ", mThetaStop);
  xmlutils::printValue(output, tabPos, "Theta Increment: ", mThetaIncr);
  xmlutils::printValue(output, tabPos, "Phi Start: ", mPhiStart);
  xmlutils::printValue(output, tabPos, "Phi Stop: ", mPhiStop);
  xmlutils::printValue(output, tabPos, "Phi Increment: ", mPhiIncr);
}

void FarField::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setThetaStart(xmlutils::readAttribute<double>(node, "theta-start"));
  setThetaStop(xmlutils::readAttribute<double>(node, "theta-stop"));
  setThetaIncr(xmlutils::readAttribute<double>(node, "theta-incr"));
  setPhiStart(xmlutils::readAttribute<double>(node, "phi-start"));
  setPhiStop(xmlutils::readAttribute<double>(node, "phi-stop"));
  setPhiIncr(xmlutils::readAttribute<double>(node, "phi-incr"));
}

void FarField::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "theta-start", mThetaStart);
  xmlutils::writeAttribute(document, node, "theta-stop", mThetaStop);
  xmlutils::writeAttribute(document, node, "theta-incr", mThetaIncr);
  xmlutils::writeAttribute(document, node, "phi-start", mPhiStart);
  xmlutils::writeAttribute(document, node, "phi-stop", mPhiStop);
  xmlutils::writeAttribute(document, node, "phi-incr", mPhiIncr);
}

namespace  {
  const bool r = Observation::factory().registerType(OBJECT_ID, boost::bind(boost::factory<FarField*>()));
}

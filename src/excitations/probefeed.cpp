#include "probefeed.h"

#include "constants.h"
#include "edge.h"
#include "functions.h"
#include "mesh/mesh.h"
#include "node.h"
#include "xmlutils.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace Morfeus {

static const std::string OBJECT_ID("Probe Feed");

ProbeFeed::ProbeFeed()
  : Excitation (OBJECT_ID)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, dcomplex(1.0, 0.0));
}

ProbeFeed::ProbeFeed(const std::string & name)
  : Excitation(OBJECT_ID, name)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, dcomplex(1.0, 0.0));
}

ProbeFeed::ProbeFeed(const std::string & id, const std::string & name)
  : Excitation(OBJECT_ID, id, name)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, dcomplex(1.0, 0.0));
}

void ProbeFeed::doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const
{
  double k0 = math::frequencyToWavenumber(freqGHz);

  //double lambda = 1.0 / freq;
  //double k0 = 2.0*math::PI/lambda;

  //double lam_cm = 30.0 / freqGHz;
  //double k0 = 2.0 * math::PI / lam_cm;

  int32_t n1 = mesh->findNearestNode(mX1, mY1, mZ1);
  int32_t n2 = mesh->findNearestNode(mX2, mY2, mZ2);
  const mesh::Edge * edge = mesh->edge(1);
  int unknown = edge->unknownNumber();

  rhs[unknown-1] = -(math::cj * k0 * math::z0 * mVoltage * edge->computeLength(mesh));
}

void ProbeFeed::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x1: ", mX1);
  xmlutils::printValue(output, tabPos, "y1: ", mY1);
  xmlutils::printValue(output, tabPos, "z1: ", mZ1);
  xmlutils::printValue(output, tabPos, "x2: ", mX2);
  xmlutils::printValue(output, tabPos, "y2: ", mY2);
  xmlutils::printValue(output, tabPos, "z2: ", mZ2);
  xmlutils::printValue(output, tabPos, "Voltage: ", mVoltage);
}

void ProbeFeed::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setX1(xmlutils::readAttribute<double>(node, "x1"));
  setY1(xmlutils::readAttribute<double>(node, "y1"));
  setZ1(xmlutils::readAttribute<double>(node, "z1"));
  setX2(xmlutils::readAttribute<double>(node, "x2"));
  setY2(xmlutils::readAttribute<double>(node, "y2"));
  setZ2(xmlutils::readAttribute<double>(node, "z2"));
  double vo_r = xmlutils::readAttribute<double>(node, "vo_r");
  double vo_i = xmlutils::readAttribute<double>(node, "vo_i");
  setVoltage(dcomplex(vo_r, vo_i));
}

void ProbeFeed::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "x1", mX1);
  xmlutils::writeAttribute(document, node, "y1", mY1);
  xmlutils::writeAttribute(document, node, "z1", mZ1);
  xmlutils::writeAttribute(document, node, "x2", mX2);
  xmlutils::writeAttribute(document, node, "y2", mY2);
  xmlutils::writeAttribute(document, node, "z2", mZ2);
  xmlutils::writeAttribute(document, node, "vo_r", mVoltage.real());
  xmlutils::writeAttribute(document, node, "vo_i", mVoltage.imag());
}

void ProbeFeed::init(double x1, double y1, double z1, double x2, double y2, double z2, const dcomplex & value)
{
  mX1 = x1;
  mY1 = y1;
  mZ1 = z1;
  mX2 = x2;
  mY2 = y2;
  mZ2 = z2;
  mVoltage = value;
}

void ProbeFeed::setPosition(double x1, double y1, double z1, double x2, double y2, double z2)
{
  mX1 = x1;
  mY1 = y1;
  mZ1 = z1;
  mX2 = x2;
  mY2 = y2;
  mZ2 = z2;
}

namespace  {
  const bool r = Excitation::factory().registerType(OBJECT_ID, boost::bind(boost::factory<ProbeFeed*>()));
}

}

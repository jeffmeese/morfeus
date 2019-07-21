#include "probefeed.h"

#include "constants.h"
#include "edge.h"
#include "functions.h"
#include "mesh.h"
#include "node.h"
#include "xmlutils.h"

static const std::string OBJECT_ID("Probe Feed");

ProbeFeed::ProbeFeed()
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, dcomplex(1.0, 0.0));
}

ProbeFeed::ProbeFeed(int32_t number)
  : Excitation(number)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, dcomplex(1.0, 0.0));
}

ProbeFeed::ProbeFeed(int32_t number, double x1, double y1, double z1, double x2, double y2, double z2, const dcomplex & excitation)
  : Excitation(number)
{
  init(x1, y1, z1, x2, y2, z2, excitation);
}

void ProbeFeed::doExcite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const
{
  double k0 = math::frequencyToWavenumber(freqGHz);

  //double lambda = 1.0 / freq;
  //double k0 = 2.0*math::PI/lambda;

  //double lam_cm = 30.0 / freqGHz;
  //double k0 = 2.0 * math::PI / lam_cm;

  int32_t n1 = mesh->findNearestNode(mX1, mY1, mZ1);
  int32_t n2 = mesh->findNearestNode(mX2, mY2, mZ2);
  const Edge * edge = mesh->findEdge(n1, n2);
  int unknown = edge->unknownNumber();

  rhs[unknown-1] = -(math::cj * k0 * math::z0 * mExcitation * edge->computeLength(mesh));
}

void ProbeFeed::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Probe Feed");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "x1: ", mX1);
  xmlutils::printValue(output, tabPos+2, "y1: ", mY1);
  xmlutils::printValue(output, tabPos+2, "z1: ", mZ1);
  xmlutils::printValue(output, tabPos+2, "x2: ", mX2);
  xmlutils::printValue(output, tabPos+2, "y2: ", mY2);
  xmlutils::printValue(output, tabPos+2, "z2: ", mZ2);
  xmlutils::printValue(output, tabPos+2, "excitation: ", mExcitation);
}

void ProbeFeed::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setX1(std::stod(xmlutils::readAttribute<std::string>(node, "x1")));
  setY1(std::stod(xmlutils::readAttribute<std::string>(node, "y1")));
  setZ1(std::stod(xmlutils::readAttribute<std::string>(node, "z1")));
  setX2(std::stod(xmlutils::readAttribute<std::string>(node, "x2")));
  setY2(std::stod(xmlutils::readAttribute<std::string>(node, "y2")));
  setZ2(std::stod(xmlutils::readAttribute<std::string>(node, "z2")));
  // Need excitation
}

void ProbeFeed::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "x1", mX1);
  xmlutils::writeAttribute(document, node, "y1", mY1);
  xmlutils::writeAttribute(document, node, "z1", mZ1);
  xmlutils::writeAttribute(document, node, "x2", mX2);
  xmlutils::writeAttribute(document, node, "y2", mY2);
  xmlutils::writeAttribute(document, node, "z2", mZ2);
  // Need excitation
}

void ProbeFeed::init(double x1, double y1, double z1, double x2, double y2, double z2, const dcomplex & value)
{
  mX1 = x1;
  mY1 = y1;
  mZ1 = z1;
  mX2 = x2;
  mY2 = y2;
  mZ2 = z2;
  mExcitation = value;
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

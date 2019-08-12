#include "polygon.h"

#include "face.h"
#include "facet.h"
#include "point.h"
#include "vertex.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Polygon::mTotal = 0;
static const std::string OBJECT_ID("Polygon");

Polygon::Polygon()
  : Part(OBJECT_ID)
{
  init();
  setName(createName(mNumber));
}

Polygon::Polygon(const std::string & name)
  : Part(OBJECT_ID, name)
{
  init();
}

Polygon::Polygon(const std::string & id, const std::string & name)
  : Part(OBJECT_ID, id, name)
{
  init();
}

void Polygon::addPoint(std::unique_ptr<Point> point)
{
  VertexPtr vertex(new Vertex(point->x(), point->y(), point->z()));
  vertex->setNumber(mVertices.size());
  mFace->facet(0)->addVertex(vertex.get());
  mVertices.push_back(std::move(vertex));
  mPoints.push_back(std::move(point));
}

math::vec3d Polygon::computeNormal() const
{
  assert(mPoints.size() >= 3);

  math::vec3d v[3];
  for (std::size_t i = 0; i < 3; i++) {
    const Point * point = mPoints[i].get();
    v[i].a[0] = point->x();
    v[i].a[1] = point->y();
    v[i].a[2] = point->z();
  }

  math::vec3d v4(v[0]-v[1]);
  math::vec3d v5(v[0]-v[2]);
  math::vec3d normal = boost::qvm::cross(v4, v5);
  boost::qvm::normalize(normal);
  return normal;
}

bool Polygon::doContainsPoint(const Point &pt) const
{
  return false;
}

std::vector<Face*> Polygon::doGetFaceList() const
{
  std::vector<Face*> faceList;
  faceList.push_back(mFace.get());
  return faceList;
}

std::vector<Region*> Polygon::doGetRegionList() const
{
  std::vector<Region*> regionList;
  return regionList;
}

std::vector<Segment*> Polygon::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  for (std::size_t i = 0; i < mSegments.size(); i++) {
    segmentList.push_back(mSegments.at(i).get());
  }
  return segmentList;
}

std::vector<Vertex*> Polygon::doGetVertexList() const
{
  std::vector<Vertex*> vertexList;
  for (std::size_t i = 0; i < mVertices.size(); i++) {
    vertexList.push_back(mVertices.at(i).get());
  }
  return vertexList;
}

std::vector<Wire*> Polygon::doGetWireList() const
{
  std::vector<Wire*> wireList;
  return wireList;
}

void Polygon::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Points");
  for (std::size_t i = 0; i < mPoints.size(); i++) {
    mPoints.at(i)->print(output, tabPos+2);
  }
}

void Polygon::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  mPoints.clear();

  rapidxml::xml_node<> * pointsNode = node->first_node("Points", 0, false);
  if (pointsNode != nullptr) {
    rapidxml::xml_node<> * pointNode = pointsNode->first_node("Point", 0, false);
    while (pointNode != nullptr) {
      std::unique_ptr<Point> point(new Point);
      point->readFromXml(document, pointNode);
      mPoints.push_back(std::move(point));
    }
  }
}

void Polygon::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  for (std::size_t i = 0; i < mPoints.size(); i++) {
    const Point * pt = mPoints.at(i).get();
    rapidxml::xml_node<> * pointNode = xmlutils::createNode(document, "point");
    pt->writeToXml(document, pointNode);
    node->append_node(pointNode);
  }
}

void Polygon::init()
{
  mNumber = mTotal++;
  std::unique_ptr<Facet> facet;
  mFace.reset(new Face);
  mFace->addFacet(std::move(facet));
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Polygon*>()));
}

}
}
}

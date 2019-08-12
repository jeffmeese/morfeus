#include "cuboid.h"

#include "face.h"
#include "facet.h"
#include "region.h"
#include "segment.h"
#include "vertex.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Cuboid::mTotal = 0;
static const std::string OBJECT_ID("Cuboid");

Cuboid::Cuboid()
  : Part(OBJECT_ID)
{
  Point pt(0.0, 0.0, 0.0);
  init(pt, 1.0, 1.0, 1.0);
  setName(createName(mNumber));
}

Cuboid::Cuboid(const std::string & name)
  : Part(OBJECT_ID, name)
{
  Point pt(0.0, 0.0, 0.0);
  init(pt, 1.0, 1.0, 1.0);
}

Cuboid::Cuboid(const std::string & id, const std::string & name)
  : Part(OBJECT_ID, id, name)
{
  Point pt(0.0, 0.0, 0.0);
  init(pt, 1.0, 1.0, 1.0);
}

Cuboid::Cuboid(const Point & pt, double length, double width, double height)
  : Part(OBJECT_ID)
{
  init(pt, length, width, height);
  setName(createName(mNumber));
}

Cuboid::Cuboid(const std::string & name, const Point & pt, double length, double width, double height)
  : Part(OBJECT_ID, name)
{
  init(pt, length, width, height);
}

Cuboid::Cuboid(const std::string & id, const std::string & name, const Point & pt, double length, double width, double height)
  : Part(OBJECT_ID, id, name)
{
  init(pt, length, width, height);
}

bool Cuboid::doContainsPoint(const Point &pt) const
{
  double x = pt.x();
  double y = pt.y();
  double z = pt.z();

  double xl = mBaseCenter.x() - mWidth / 2;
  double xu = mBaseCenter.x() + mWidth / 2;
  double yl = mBaseCenter.y() - mLength / 2;
  double yu = mBaseCenter.y() + mLength / 2;
  double zl = mBaseCenter.z();
  double zu = mBaseCenter.z() + mHeight;

  return ( (x >= xl && x <= xu) && (y >= yl && y <= yu) && (z >= zl && z <= zu) );
}

std::vector<Face*> Cuboid::doGetFaceList() const
{
  std::vector<Face*> faceList;
  for (std::size_t i = 0; i < mFaces.size(); i++) {
    Face * face = mFaces.at(i).get();
    if (face->medium() == nullptr && mRegion->medium() != nullptr) {
      face->setMedium(mRegion->medium());
    }
    faceList.push_back(face);
  }
  return faceList;
}

std::vector<Region*> Cuboid::doGetRegionList() const
{
  std::vector<Region*> regionList;
  regionList.push_back(mRegion.get());
  return regionList;
}

std::vector<Segment*> Cuboid::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  for (std::size_t i = 0; i < 12; i++) {
    segmentList.push_back(mSegments.at(i).get());
  }

  return segmentList;
}

std::vector<Vertex*> Cuboid::doGetVertexList() const
{
  std::vector<Vertex*> vertexList;
  for (std::size_t i = 0; i < mVertices.size(); i++) {
    vertexList.push_back(mVertices.at(i).get());
  }

  return vertexList;
}

std::vector<Wire*> Cuboid::doGetWireList() const
{
  std::vector<Wire*> wireList;
  return wireList;
}

void Cuboid::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Center");
  mBaseCenter.print(output, tabPos+2);
  xmlutils::printValue(output, tabPos, "Length: ", mLength);
  xmlutils::printValue(output, tabPos, "Width: ", mWidth);
  xmlutils::printValue(output, tabPos, "Height: ", mHeight);
}

void Cuboid::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * centerNode = node->first_node("Center", 0, false);
  if (centerNode != nullptr) {
    mBaseCenter.readFromXml(document, centerNode);
  }
  setLength(xmlutils::readAttribute<double>(node, "length"));
  setWidth(xmlutils::readAttribute<double>(node, "width"));
  setHeight(xmlutils::readAttribute<double>(node, "height"));
}

void Cuboid::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "length", mLength);
  xmlutils::writeAttribute(document, node, "height", mHeight);
  xmlutils::writeAttribute(document, node, "width", mWidth);
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Center");
  mBaseCenter.writeToXml(document, centerNode);
}

void Cuboid::init(const Point &pt, double length, double width, double height)
{
  static const std::size_t face_vertex1[] = {0, 4, 0, 1, 2, 3};
  static const std::size_t face_vertex2[] = {1, 5, 1, 2, 3, 0};
  static const std::size_t face_vertex3[] = {2, 6, 5, 6, 7, 4};
  static const std::size_t face_vertex4[] = {3, 7, 4, 5, 6, 7};
  static const std::size_t segment_vertex1[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3};
  static const std::size_t segment_vertex2[] = {1, 2, 3, 0, 5, 6, 7, 4, 4, 5, 6, 7};

  mNumber = mTotal++;
  mBaseCenter = pt;
  mLength = length;
  mWidth = width;
  mHeight = height;

  // Create vertices
  for (std::size_t i = 0; i < 8; i++) {
    VertexPtr v(new Vertex);
    v->setNumber(i);
    mVertices.push_back(std::move(v));
  }

  // Create segments
  for (std::size_t i = 0; i < 12; i++) {
    mSegments.push_back(SegmentPtr(new Segment));
  }

  // Create faces
  for (std::size_t i = 0; i < 6; i++) {
    std::size_t v1 = face_vertex1[i];
    std::size_t v2 = face_vertex2[i];
    std::size_t v3 = face_vertex3[i];
    std::size_t v4 = face_vertex4[i];

    std::unique_ptr<Facet> facet(new Facet);
    facet->addVertex(mVertices.at(v1).get());
    facet->addVertex(mVertices.at(v2).get());
    facet->addVertex(mVertices.at(v3).get());
    facet->addVertex(mVertices.at(v4).get());

    FacePtr face(new Face);
    face->addFacet(std::move(facet));
    mFaces.push_back(std::move(face));
  }

  // Assign segment vertices
  for (std::size_t i = 0; i < 12; i++) {
    std::size_t v1 = segment_vertex1[i];
    std::size_t v2 = segment_vertex2[i];
    mSegments[i]->setVertex1(mVertices[v1].get());
    mSegments[i]->setVertex2(mVertices[v2].get());
  }

  // Create the region
  mRegion.reset(new Region);

  updatePosition();
}

void Cuboid::setBaseCenter(const Point & pt)
{
  mBaseCenter = pt;
  updatePosition();
}

void Cuboid::setBaseCenter(double x, double y, double z)
{
  setBaseCenter(Point(x, y, z));
}

void Cuboid::setHeight(double value)
{
  mHeight = value;
  updatePosition();
}

inline void Cuboid::setLength(double value)
{
  mLength = value;
  updatePosition();
}

void Cuboid::setWidth(double value)
{
  mWidth = value;
  updatePosition();
}

void Cuboid::updatePosition()
{
  double xl = mBaseCenter.x() - mWidth / 2;
  double xu = mBaseCenter.x() + mWidth / 2;
  double yl = mBaseCenter.y() - mLength / 2;
  double yu = mBaseCenter.y() + mLength / 2;
  double zl = mBaseCenter.z();
  double zu = mBaseCenter.z() + mHeight;

  mVertices[0]->setPosition(xl, yl, zu);
  mVertices[1]->setPosition(xu, yl, zu);
  mVertices[2]->setPosition(xu, yu, zu);
  mVertices[3]->setPosition(xl, yu, zu);
  mVertices[4]->setPosition(xl, yl, zl);
  mVertices[5]->setPosition(xu, yl, zl);
  mVertices[6]->setPosition(xu, yu, zl);
  mVertices[7]->setPosition(xl, yu, zl);
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Cuboid*>()));
}

}
}
}

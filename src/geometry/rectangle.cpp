#include "rectangle.h"

#include "geometry.h"
#include "segment.h"
#include "vertex.h"

Rectangle::Rectangle()
{
}

void Rectangle::doAddToGeometry(Geometry *geometry) const
{
//  std::size_t totalVertices = geometry->totalVertices();
//  std::size_t totalSegments = geometry->totalSegments();

//  int32_t node1 = static_cast<int32_t>(totalVertices + 1);
//  int32_t node2 = node1 + 1;
//  int32_t node3 = node2 + 1;
//  int32_t node4 = node3 + 1;

//  double x1 = mCenter.x() - mWidth / 2;
//  double x2 = mCenter.x() + mWidth / 2;
//  double y1 = mCenter.y() - mHeight / 2;
//  double y2 = mCenter.y() + mHeight / 2;

//  std::unique_ptr<Vertex> v1(new Vertex(x1, y1));
//  std::unique_ptr<Vertex> v2(new Vertex(x2, y1));
//  std::unique_ptr<Vertex> v3(new Vertex(x2, y2));
//  std::unique_ptr<Vertex> v4(new Vertex(x1, y2));

//  v1->setNumber(node1);
//  v2->setNumber(node2);
//  v3->setNumber(node3);
//  v4->setNumber(node4);

//  geometry->addVertex(std::move(v1));
//  geometry->addVertex(std::move(v2));
//  geometry->addVertex(std::move(v3));
//  geometry->addVertex(std::move(v4));

//  int32_t segment1 = static_cast<int32_t>(totalSegments);
//  std::unique_ptr<Segment> s1(new Segment(node1, node2));
//  std::unique_ptr<Segment> s2(new Segment(node2, node3));
//  std::unique_ptr<Segment> s3(new Segment(node3, node4));
//  std::unique_ptr<Segment> s4(new Segment(node4, node1));

//  s1->setNumber(segment1);
//  s2->setNumber(segment1+1);
//  s3->setNumber(segment1+2);
//  s4->setNumber(segment1+3);

//  geometry->addSegment(std::move(s1));
//  geometry->addSegment(std::move(s2));
//  geometry->addSegment(std::move(s3));
//  geometry->addSegment(std::move(s4));
}

void Rectangle::doReadFromXml(ptree &tree)
{

}

void Rectangle::doWriteToXml(ptree &tree) const
{
  ptree itemTree;
  itemTree.put("<xmlattr>.type", "Rectangle");
  itemTree.put("<xmlattr>.name", name());
  itemTree.put("<xmlattr>.number", number());
  itemTree.put("<xmlattr>.left", mLeft);
  itemTree.put("<xmlattr>.right", mRight);
  itemTree.put("<xmlattr>.top", mTop);
  itemTree.put("<xmlattr>.bottom", mBottom);
  tree.push_back(std::make_pair("Shape", itemTree));
}

double Rectangle::height() const
{
  return fabs(mTop-mBottom);
}

double Rectangle::width() const
{
  return fabs(mRight-mLeft);
}

void Rectangle::setPosition(double left, double right, double top, double bottom)
{
  mLeft = left;
  mRight = right;
  mTop = top;
  mBottom = bottom;
}

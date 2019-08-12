#ifndef MORFEUS_MODEL_GEOMETRY_LINE_H
#define MORFEUS_MODEL_GEOMETRY_LINE_H

#include "part.h"
#include "point.h"

namespace morfeus {
namespace model {
namespace geometry {

class Line
    : public Part
{
public:
  MORFEUS_LIB_DECL Line();
  MORFEUS_LIB_DECL Line(const std::string & name);
  MORFEUS_LIB_DECL Line(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Line(const Point & startPt, const Point & endPoint);
  MORFEUS_LIB_DECL Line(const std::string & name, const Point & startPt, const Point & endPoint);
  MORFEUS_LIB_DECL Line(const std::string & id, const std::string & name, const Point & startPt, const Point & endPoint);

public:
  MORFEUS_LIB_DECL Point endPoint() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL Point startPoint() const;
  MORFEUS_LIB_DECL void setEndPoint(const Point & pt);
  MORFEUS_LIB_DECL void setStartPoint(const Point & pt);

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  bool doContainsPoint(const Point & pt) const override;
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Region*> doGetRegionList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  std::vector<Vertex*> doGetVertexList() const override;
  std::vector<Wire*> doGetWireList() const override;
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(const Point & startPoint, const Point & endPoint);

private:
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::unique_ptr<Wire> WirePtr;

private:
  static int32_t mTotal;
  int32_t mNumber;
  Point mEndPoint;
  Point mStartPoint;
  VertexPtr mVertex1;
  VertexPtr mVertex2;
  WirePtr mWire;
};

inline Point Line::endPoint() const
{
  return mEndPoint;
}

inline int32_t Line::number() const
{
  return mNumber;
}

inline Point Line::startPoint() const
{
  return mStartPoint;
}

inline void Line::setEndPoint(const Point & pt)
{
  mEndPoint = pt;
}

inline void Line::setStartPoint(const Point & pt)
{
  mStartPoint = pt;
}

inline int32_t Line::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_LINE_H

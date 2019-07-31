#ifndef MORFEUS_GEOMETRY_RECTANGLE_H
#define MORFEUS_GEOMETRY_RECTANGLE_H

#include "surface.h"
#include "point.h"

namespace morfeus {
namespace geometry {

class Rectangle
    : public Surface
{
public:
  MORFEUS_LIB_DECL Rectangle();
  MORFEUS_LIB_DECL Rectangle(const std::string & name);
  MORFEUS_LIB_DECL Rectangle(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Rectangle(const Point & center, double length, double width);
  MORFEUS_LIB_DECL Rectangle(const std::string & name, const Point & center, double length, double width);
  MORFEUS_LIB_DECL Rectangle(const std::string & id, const std::string & name, const Point & center, double length, double width);

public:
  MORFEUS_LIB_DECL Point center() const;
  MORFEUS_LIB_DECL double length() const;
  MORFEUS_LIB_DECL double width() const;
  MORFEUS_LIB_DECL void setCenter(Point center);
  MORFEUS_LIB_DECL void setLength(double value);
  MORFEUS_LIB_DECL void setWidth(double value);

protected:
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  Point mCenter;
  double mLength;
  double mWidth;
  std::vector<std::unique_ptr<Face>> mFaces;
};

inline Point Rectangle::center() const
{
  return mCenter;
}

inline double Rectangle::length() const
{
  return mLength;
}

inline double Rectangle::width() const
{
  return mWidth;
}


inline void Rectangle::setCenter(Point value)
{
  mCenter = value;
}

inline void Rectangle::setLength(double value)
{
  mLength = value;
}

inline void Rectangle::setWidth(double value)
{
  mWidth = value;
}

}
}

#endif // MORFEUS_GEOMETRY_RECTANGLE_H

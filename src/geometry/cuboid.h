#ifndef CUBOID_H
#define CUBOID_H

#include "solid.h"
#include "point.h"

namespace morfeus {
  namespace geometry {
    class Face;
    class Region;
    class Segment;
  }
}

namespace morfeus {
namespace geometry {

class Cuboid
    : public Solid
{
public:
  MORFEUS_LIB_DECL Cuboid();
  MORFEUS_LIB_DECL Cuboid(const std::string & name);
  MORFEUS_LIB_DECL Cuboid(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Cuboid(const Point & pt, double length, double width, double height);
  MORFEUS_LIB_DECL Cuboid(const std::string & name, const Point & pt, double length, double width, double height);
  MORFEUS_LIB_DECL Cuboid(const std::string & id, const std::string & name, const Point & pt, double length, double width, double height);

public:
  MORFEUS_LIB_DECL Point baseCenter() const;
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL double length() const;
  MORFEUS_LIB_DECL double width() const;
  MORFEUS_LIB_DECL void setBaseCenter(const Point & pt);
  MORFEUS_LIB_DECL void setHeight(double value);
  MORFEUS_LIB_DECL void setLength(double value);
  MORFEUS_LIB_DECL void setWidth(double value);

protected:
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Region*> doGetRegionList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void createFaces();
  void createRegions();
  void initSegments();
  void init(const Point & pt, double length, double width, double height);
  void updateFaces();

private:
  typedef std::unique_ptr<Face> FacePtr;
  typedef std::unique_ptr<Region> RegionPtr;
  typedef std::unique_ptr<Segment> SegmentPtr;

private:
  Point mBaseCenter;
  double mHeight;
  double mLength;
  double mWidth;
  std::vector<FacePtr> mFaces;
  std::vector<RegionPtr> mRegions;
  std::vector<SegmentPtr> mSegments;
};

inline Point Cuboid::baseCenter() const
{
  return mBaseCenter;
}

inline double Cuboid::height() const
{
  return mHeight;
}

inline double Cuboid::length() const
{
  return mLength;
}

inline double Cuboid::width() const
{
  return mWidth;
}

inline void Cuboid::setBaseCenter(const Point & pt)
{
  mBaseCenter = pt;
  updateFaces();
}

inline void Cuboid::setHeight(double value)
{
  mHeight = value;
  updateFaces();
}

inline void Cuboid::setLength(double value)
{
  mLength = value;
  updateFaces();
}

inline void Cuboid::setWidth(double value)
{
  mWidth = value;
  updateFaces();
}

}
}

#endif // CUBOID_H

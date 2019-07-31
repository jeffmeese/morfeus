#ifndef MORFEUS_GEOMETRY_REGION_H
#define MORFEUS_GEOMETRY_REGION_H

#include "morfeus.h"

#include "core/morfeusobject.h"

#include "geometry/point.h"

namespace morfeus {
  namespace media {
    class Medium;
  }
}

namespace morfeus {
namespace geometry {

class Region
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Region();
  MORFEUS_LIB_DECL Region(const std::string & name);
  MORFEUS_LIB_DECL Region(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Region(const Point & pt);
  MORFEUS_LIB_DECL Region(const std::string & name, const Point & pt);
  MORFEUS_LIB_DECL Region(const std::string & id, const std::string & name, const Point & pt);

public:
  MORFEUS_LIB_DECL double attribute() const;
  MORFEUS_LIB_DECL media::Medium * medium();
  MORFEUS_LIB_DECL const media::Medium * medium() const;
  MORFEUS_LIB_DECL double maxArea() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL Point position() const;
  MORFEUS_LIB_DECL void setAttribute(double value);
  MORFEUS_LIB_DECL void setMaxArea(double value);
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setPosition(const Point & pt);
  MORFEUS_LIB_DECL void setMedium(media::Medium * medium);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mAttribute;
  double mMaxArea;
  std::string mName;
  Point mPosition;
  media::Medium * mMedium;
};

inline media::Medium * Region::medium()
{
  return mMedium;
}

inline const media::Medium * Region::medium() const
{
  return mMedium;
}

inline double Region::maxArea() const
{
  return mMaxArea;
}

inline std::string Region::name() const
{
  return mName;
}

inline Point Region::position() const
{
  return mPosition;
}

inline void Region::setAttribute(double value)
{
  mAttribute = value;
}

inline void Region::setMedium(media::Medium *medium)
{
  mMedium = medium;
}

inline void Region::setMaxArea(double value)
{
  mMaxArea = value;
}

inline void Region::setName(const std::string &name)
{
  mName = name;
}

inline void Region::setPosition(const Point & pt)
{
  mPosition = pt;
}

}
}

#endif // MORFEUS_GEOMETRY_REGION_H

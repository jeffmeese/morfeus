#ifndef MORFEUS_MODEL_GEOMETRY_REGION_H
#define MORFEUS_MODEL_GEOMETRY_REGION_H

#include "core/morfeusobject.h"

#include "point.h"

namespace morfeus {

  namespace model {
    namespace media {
      class Medium;
    }
  }
}

namespace morfeus {
namespace model {
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
  MORFEUS_LIB_DECL double localMeshSize() const;
  MORFEUS_LIB_DECL media::Medium * medium();
  MORFEUS_LIB_DECL const media::Medium * medium() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL Point position() const;
  MORFEUS_LIB_DECL void setLocalMeshSize(double value);
  MORFEUS_LIB_DECL void setMedium(media::Medium * medium);
  MORFEUS_LIB_DECL void setPosition(double x, double y, double z);
  MORFEUS_LIB_DECL void setPosition(const Point & pt);

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(const Point & pt);

private:
  static int32_t mTotal;
  int32_t mNumber;
  double mLocalMeshSize;
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

inline int32_t Region::number() const
{
  return mNumber;
}

inline double Region::localMeshSize() const
{
  return mLocalMeshSize;
}

inline Point Region::position() const
{
  return mPosition;
}

inline void Region::setMedium(media::Medium *medium)
{
  mMedium = medium;
}

inline void Region::setLocalMeshSize(double value)
{
  mLocalMeshSize = value;
}

inline void Region::setPosition(double x, double y, double z)
{
  setPosition(Point(x, y, z));
}

inline void Region::setPosition(const Point & pt)
{
  mPosition = pt;
}

inline int32_t Region::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_REGION_H

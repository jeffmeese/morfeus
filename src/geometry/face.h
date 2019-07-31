#ifndef MORFEUS_GEOMETRY_FACE_H
#define MORFEUS_GEOMETRY_FACE_H

#include "morfeus.h"
#include "segment.h"

#include <vector>

namespace morfeus {
  namespace media {
    class Medium;
  }
}

namespace morfeus {
namespace geometry {

class Face
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Face();
  MORFEUS_LIB_DECL Face(const std::string & name);
  MORFEUS_LIB_DECL Face(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL media::Medium * medium();
  MORFEUS_LIB_DECL const media::Medium * medium() const;
  MORFEUS_LIB_DECL void setMedium(media::Medium * condition);

protected:
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::string mName;
  media::Medium * mMedium;
};

inline std::string Face::name() const
{
  return mName;
}

inline void Face::setName(const std::string & name)
{
  mName = name;
}

inline media::Medium * Face::medium()
{
  return mMedium;
}

inline const media::Medium * Face::medium() const
{
  return mMedium;
}

inline void Face::setMedium(media::Medium *medium)
{
  mMedium = medium;
}

}
}

#endif // FACET_H

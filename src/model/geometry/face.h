#ifndef MORFEUS_MODEL_GEOMETRY_FACE_H
#define MORFEUS_MODEL_GEOMETRY_FACE_H

#include "morfeus.h"
#include "point.h"
#include "segment.h"

#include "math/types.h"

#include <vector>

namespace morfeus {

  namespace model {
    class Model;
    namespace geometry {
      class Facet;
    }
  }

  namespace model {
    namespace media {
      class Medium;
    }
  }
}

namespace morfeus {
namespace model {
namespace geometry {

class Face
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Face();
  MORFEUS_LIB_DECL Face(const std::string & name);
  MORFEUS_LIB_DECL Face(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL void addFacet(std::unique_ptr<Facet> facet);
  MORFEUS_LIB_DECL double localMeshSize() const;
  MORFEUS_LIB_DECL const media::Medium * medium() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL Facet * facet(std::size_t index);
  MORFEUS_LIB_DECL const Facet * facet(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalFacets() const;
  MORFEUS_LIB_DECL void setLocalMeshSize(double value);
  MORFEUS_LIB_DECL void setMedium(const media::Medium * medium);

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init();

private:
  typedef std::unique_ptr<Facet> FacetPtr;
  typedef std::vector<FacetPtr> FacetVector;

private:
  static int32_t mTotal;
  int32_t mNumber;
  double mLocalMeshSize;
  const media::Medium * mMedium;
  FacetVector mFacets;
};

inline Facet * Face::facet(std::size_t index)
{
  return mFacets.at(index).get();
}

inline const Facet * Face::facet(std::size_t index) const
{
  return mFacets.at(index).get();
}

inline double Face::localMeshSize() const
{
  return mLocalMeshSize;
}

inline const media::Medium * Face::medium() const
{
  return mMedium;
}

inline void Face::setLocalMeshSize(double value)
{
  mLocalMeshSize = value;
}

inline void Face::setMedium(const media::Medium *medium)
{
  mMedium = medium;
}

inline int32_t Face::totalCreated()
{
  return mTotal;
}

inline std::size_t Face::totalFacets() const
{
  return mFacets.size();
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_FACE_H

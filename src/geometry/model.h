#ifndef MORFEUS_GEOMETRY_MODEL_H
#define MORFEUS_GEOMETRY_MODEL_H

#include "morfeus.h"

#include "core/morfeusobject.h"

#include <memory>
#include <vector>

namespace morfeus {
  namespace geometry {
    class Hole;
    class Mesh;
    class Part;
    class Region;
    class Segment;
    class Vertex;
  }
}

namespace morfeus {
namespace geometry {

class Model
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Model();
  MORFEUS_LIB_DECL virtual ~Model();

public:
  MORFEUS_LIB_DECL Part * part(std::size_t index);
  MORFEUS_LIB_DECL const Part * part(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalParts() const;

public:
  MORFEUS_LIB_DECL void addPart(std::unique_ptr<Part> part);
  MORFEUS_LIB_DECL void createMesh(Mesh & mesh);

protected:
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  typedef std::unique_ptr<Part> PartPtr;
  typedef std::vector<PartPtr> PartVector;

private:
  double mMaxArea;
  double mMinArea;
  PartVector mParts;
};

inline const Part * Model::part(std::size_t index) const
{
  return mParts.at(index).get();
}

inline Part * Model::part(std::size_t index)
{
  return mParts.at(index).get();
}

inline std::size_t Model::totalParts() const
{
  return mParts.size();
}

}
}

#endif // MORFEUS_GEOMETRY_MODEL_H

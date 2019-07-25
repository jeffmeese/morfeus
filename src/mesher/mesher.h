#ifndef MESHER_H
#define MESHER_H

#include "morfeus.h"
#include "morfeusobject.h"

#include "mesherfacet.h"
#include "vertex.h"

#include <vector>

class Geometry;
class Mesh;
class MeshRefinement;

class Mesher
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Mesher();
  MORFEUS_LIB_DECL virtual ~Mesher() override;

public:
  MORFEUS_LIB_DECL double maxTriangleArea() const;
  MORFEUS_LIB_DECL double minTriangleArea() const;
  MORFEUS_LIB_DECL double maxEdgeLength() const;
  MORFEUS_LIB_DECL double minEdgeLength() const;
  MORFEUS_LIB_DECL void setMaxTriangleArea(double value);
  MORFEUS_LIB_DECL void setMinTriangleArea(double value);
  MORFEUS_LIB_DECL void setMaxEdgeLength(double value);
  MORFEUS_LIB_DECL void setMinEdgeLength(double value);

public:
  MORFEUS_LIB_DECL void createMesh(const Geometry * geometry, double cavityHeight, Mesh * mesh) const;
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Mesher & object);

private:
  void formVertexList(std::vector<Vertex> & vertices, const Geometry & geometry);
  void formFacets(std::vector<MesherFacet> & facets, const Geometry & geometry);

private:
  double mMaxEdgeLength;
  double mMaxTriangleArea;
  double mMinEdgeLength;
  double mMinTriangleArea;
};

inline double Mesher::maxTriangleArea() const
{
  return mMaxTriangleArea;
}

inline double Mesher::minTriangleArea() const
{
  return mMinTriangleArea;
}

inline double Mesher::maxEdgeLength() const
{
  return mMaxEdgeLength;
}

inline double Mesher::minEdgeLength() const
{
  return mMinEdgeLength;
}

inline void Mesher::setMaxTriangleArea(double value)
{
  mMaxTriangleArea = value;
}

inline void Mesher::setMinTriangleArea(double value)
{
  mMinTriangleArea = value;
}

inline void Mesher::setMaxEdgeLength(double value)
{
  mMaxEdgeLength = value;
}

inline void Mesher::setMinEdgeLength(double value)
{
  mMinEdgeLength = value;
}

#endif // MESHER_H

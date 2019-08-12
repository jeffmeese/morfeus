#ifndef MORFEUS_MESHER_MESHER_H
#define MORFEUS_MESHER_MESHER_H

#include "core/morfeusobject.h"

#include "model/geometry/vertex.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <vector>

#include <boost/qvm/vec.hpp>

class tetgenio;

namespace morfeus {

  namespace model {
    class Model;
  }

  namespace mesh {
    class Mesh;
  }
}

namespace morfeus {
namespace mesher {

class Mesher
    : public core::MorfeusObject
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
  MORFEUS_LIB_DECL void createMesh(const model::Model * model, mesh::Mesh * mesh) const;

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void addFacets(const model::Model * model, tetgenio * in) const;
  void addHoles(const model::Model * model, tetgenio * in) const;
  void addMeshElements(const tetgenio * out, const model::Model * model, mesh::Mesh * mesh) const;
  void addMeshNodes(const tetgenio * out, mesh::Mesh * mesh) const;
  void addPoints(const model::Model * model, tetgenio * in) const;
  void addRegions(const model::Model * model, tetgenio * in) const;
  void applyMedia(const model::Model * model, mesh::Mesh * mesh) const;
  void writePolyFile(const std::string & fileName, const tetgenio * in) const;

private:
  class Polygon
  {
  public:
    const model::geometry::Vertex * vertex(std::size_t index) const;
    std::size_t totalVertices() const;

  public:
    void addVertex(const model::geometry::Vertex * vertex);
    void clearVertices();
    boost::qvm::vec<double, 3> computeNormal() const;
    bool isCoplanar(const Mesher::Polygon & polygon, double tolerance = 1e-8) const;

  private:
    std::vector<const model::geometry::Vertex*> mVertices;
  };

  class Facet
  {
  public:
    const Mesher::Polygon & polygon(std::size_t index) const;
    std::size_t totalPolygons() const;

  public:
    void addPolygon(const Mesher::Polygon & polygon);

  private:
    std::vector<Mesher::Polygon> mPolygons;
  };

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

}
}

#endif // MORFEUS_MESHER_MESHER_H

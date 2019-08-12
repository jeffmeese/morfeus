#ifndef MORFEUS_MODEL_GEOMETRY_WIRE_H
#define MORFEUS_MODEL_GEOMETRY_WIRE_H

#include "core/morfeusobject.h"

namespace morfeus {
  namespace model {
    namespace geometry {
      class Vertex;
    }
  }
}

namespace morfeus {
namespace model {
namespace geometry {

class Wire
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Wire();
  MORFEUS_LIB_DECL Wire(const std::string & name);
  MORFEUS_LIB_DECL Wire(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Wire(const Vertex * vertex1, const Vertex * vertex2);
  MORFEUS_LIB_DECL Wire(const std::string & name, const Vertex * vertex1, const Vertex * vertex2);
  MORFEUS_LIB_DECL Wire(const std::string & id, const std::string & name, const Vertex * vertex1, const Vertex * vertex2);

public:
  MORFEUS_LIB_DECL double localMeshSize() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL const Vertex * vertex1() const;
  MORFEUS_LIB_DECL const Vertex * vertex2() const;
  MORFEUS_LIB_DECL void setLocalMeshSize(double value);
  MORFEUS_LIB_DECL void setVertex1(const Vertex * vertex);
  MORFEUS_LIB_DECL void setVertex2(const Vertex * vertex);
  MORFEUS_LIB_DECL void setVertices(const Vertex * vertex1, const Vertex * vertex2);

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(const Vertex * vertex1, const Vertex * vertex2);

private:
  static int32_t mTotal;
  int32_t mNumber;
  const Vertex * mVertex1;
  const Vertex * mVertex2;
  double mLocalMeshSize;
};

inline double Wire::localMeshSize() const
{
  return mLocalMeshSize;
}

inline int32_t Wire::number() const
{
  return mNumber;
}

inline void Wire::setLocalMeshSize(double value)
{
  mLocalMeshSize = value;
}

inline void Wire::setVertex1(const Vertex * value)
{
  mVertex1 = value;
}

inline void Wire::setVertex2(const Vertex * value)
{
  mVertex2 = value;
}

inline void Wire::setVertices(const Vertex * vertex1, const Vertex * vertex2)
{
  mVertex1 = vertex1;
  mVertex2 = vertex2;
}

inline int32_t Wire::totalCreated()
{
  return mTotal;
}

inline const Vertex * Wire::vertex1() const
{
  return mVertex1;
}

inline const Vertex * Wire::vertex2() const
{
  return mVertex2;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_WIRE_H

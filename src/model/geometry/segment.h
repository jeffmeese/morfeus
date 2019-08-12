#ifndef MORFEUS_MODEL_GEOMETRY_SEGMENT_H
#define MORFEUS_MODEL_GEOMETRY_SEGMENT_H

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

class Segment
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Segment();
  MORFEUS_LIB_DECL Segment(const std::string & name);
  MORFEUS_LIB_DECL Segment(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Segment(const Vertex * vertex1, const Vertex * vertex2);
  MORFEUS_LIB_DECL Segment(const std::string & name, const Vertex * vertex1, const Vertex * vertex2);
  MORFEUS_LIB_DECL Segment(const std::string & id, const std::string & name, const Vertex * vertex1, const Vertex * vertex2);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL const Vertex * vertex1() const;
  MORFEUS_LIB_DECL const Vertex * vertex2() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setVertex1(const Vertex * vertex);
  MORFEUS_LIB_DECL void setVertex2(const Vertex * vertex);
  MORFEUS_LIB_DECL void setVertices(const Vertex * vertex1, const Vertex * vertex2);

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(const Vertex * vertex1, const Vertex * vertex2);

private:
  static int32_t mTotal;
  int32_t mNumber;
  bool mBoundary;
  const Vertex * mVertex1;
  const Vertex * mVertex2;
};

inline bool Segment::boundary() const
{
  return mBoundary;
}

inline const Vertex * Segment::vertex1() const
{
  return mVertex1;
}

inline const Vertex * Segment::vertex2() const
{
  return mVertex2;
}

inline int32_t Segment::number() const
{
  return mNumber;
}

inline void Segment::setBoundary(bool value)
{
  mBoundary = value;
}

inline void Segment::setVertex1(const Vertex *vertex)
{
  mVertex1 = vertex;
}

inline void Segment::setVertex2(const Vertex * vertex)
{
  mVertex2 = vertex;
}

inline void Segment::setVertices(const Vertex *vertex1, const Vertex *vertex2)
{
  mVertex1 = vertex1;
  mVertex2 = vertex2;
}

inline int32_t Segment::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_SEGMENT_H

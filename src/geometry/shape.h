#ifndef SHAPE_H
#define SHAPE_H

#include "morfeus.h"
#include "morfeusobject.h"
#include "segment.h"
#include "vertex.h"

#include <string>
#include <vector>

#include <Factory.h>
#include <Singleton.h>

class Shape
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL std::vector<Segment> getSegmentList() const;
  MORFEUS_LIB_DECL std::vector<Vertex> getVertexList() const;

public:
  typedef Loki::SingletonHolder<Loki::Factory<Shape, std::string> > Factory;

protected:
  Shape();
  Shape(int32_t number);
  Shape(const std::string & name, int32_t number);

protected:
  virtual std::vector<Segment> doGetSegmentList() const = 0;
  virtual std::vector<Vertex> doGetVertexList() const = 0;
};

#endif // SHAPE_H

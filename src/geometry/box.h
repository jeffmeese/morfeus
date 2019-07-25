#ifndef BOX_H
#define BOX_H

#include "shape.h"

class Box
    : public Shape
{
public:
  MORFEUS_LIB_DECL Box();
  MORFEUS_LIB_DECL Box(const std::string & name);

public:
  MORFEUS_LIB_DECL double xl() const;
  MORFEUS_LIB_DECL double xu() const;
  MORFEUS_LIB_DECL double yl() const;
  MORFEUS_LIB_DECL double yu() const;
  MORFEUS_LIB_DECL double zl() const;
  MORFEUS_LIB_DECL double zu() const;
  MORFEUS_LIB_DECL void setXl(double value);
  MORFEUS_LIB_DECL void setXu(double value);
  MORFEUS_LIB_DECL void setYl(double value);
  MORFEUS_LIB_DECL void setYu(double value);
  MORFEUS_LIB_DECL void setZl(double value);
  MORFEUS_LIB_DECL void setZu(double value);

protected:
  std::vector<MesherPolygon> doGetMesherPolygons() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mXl;
  double mXu;
  double mYl;
  double mYu;
  double mZl;
  double mZu;
};

inline double Box::xl() const
{
  return mXl;
}

inline double Box::xu() const
{
  return mXu;
}

inline double Box::yl() const
{
  return mYl;
}

inline double Box::yu() const
{
  return mYu;
}

inline double Box::zl() const
{
  return mZl;
}

inline double Box::zu() const
{
  return mZu;
}

inline void Box::setXl(double value)
{
  mXl = value;
}

inline void Box::setXu(double value)
{
  mXu = value;
}

inline void Box::setYl(double value)
{
  mYl = value;
}

inline void Box::setYu(double value)
{
  mYu = value;
}

inline void Box::setZl(double value)
{
  mZl = value;
}

inline void Box::setZu(double value)
{
  mZu = value;
}

#endif // BOX_H

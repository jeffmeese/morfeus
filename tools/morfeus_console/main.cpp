#include "application.h"

#include "geometry.h"
#include "rectangle.h"
#include "box.h"
#include "mesherfacet.h"
#include "mesher.h"

#include <iostream>

int main(int argc, char ** argv)
{
  Geometry geometry;
  std::unique_ptr<Rectangle> rect1(new Rectangle(-1.5, 1.5, -1.5, 1.5));
  std::unique_ptr<Rectangle> rect2(new Rectangle(-1.0, 1.0, -1.0, 1.0));
  std::unique_ptr<Rectangle> rect3(new Rectangle(-1.0, 1.0, -1.0, 1.0));
  rect3->setZ(-1.0);

  geometry.addShape(std::move(rect1));
  geometry.addShape(std::move(rect2));
  geometry.addShape(std::move(rect3));

  std::unique_ptr<Box> b(new Box(-2.0, 2.0, -2.0, 2.0, -2.0, 0.0));
  geometry.addShape(std::move(b));

  std::vector<MesherFacet> facets;
  Mesher mesher;
  mesher.formFacets(facets, &geometry);

  std::cout << "Total Facets: " << facets.size() << "\n";
  for (std::size_t i = 0; i < facets.size(); i++) {
    const MesherFacet & facet = facets.at(i);
    std::cout << "Facet " << i + 1 << "\n";
    for (std::size_t j = 0; j < facet.totalPolygons(); j++) {
      const MesherPolygon & p = facet.polygon(j);
      for (std::size_t k = 0; k < p.totalPoints(); k++) {
        std::cout << k+1 << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }

  //Application application(argc, argv);
  //application.execute();
}

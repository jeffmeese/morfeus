#include "application.h"

int main(int argc, char ** argv)
{
  Application application(argc, argv);
  application.execute();
}

#include "geometry.h"
#include "rectangle.h"
#include "region.h"
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

void testXml()
{
  using namespace boost::property_tree;

  Geometry geometry;

  std::unique_ptr<Rectangle> rect(new Rectangle);
  rect->setName("rect1");
  rect->setNumber(1);
  rect->setPosition(-1.0, 1.0, -1.0, 1.0);
  geometry.addShape(std::move(rect));

  std::unique_ptr<Rectangle> rect2(new Rectangle);
  rect2->setName("rect2");
  rect2->setNumber(2);
  rect2->setPosition(-2.0, 2.0, -2.0, 2.0);
  geometry.addShape(std::move(rect2));

  std::unique_ptr<Region> region(new Region(0.0, 0.0));
  region->setAttribute(1);
  region->setMaxArea(0.025);
  geometry.addRegion(std::move(region));

  ptree rootTree;
  ptree projectTree;
  geometry.writeToXml(projectTree);
  rootTree.add_child("Project", projectTree);

  xml_writer_settings<std::string> settings(' ', 2);
  std::string fileName("c:\\users\\jmeese\\desktop\\temp.xml");
  write_xml(fileName, rootTree, std::locale(), settings);
}

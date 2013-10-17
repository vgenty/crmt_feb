#include "Geometry.h"

Geometry::Geometry()
{
  fName = "module";
}

Geometry::~Geometry() {}


void Geometry::dump()
{
  std::cout << "Name: " << fName << std::endl;
}



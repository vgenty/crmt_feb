#include "Fiber.h"

Fiber::Fiber()
{
  fid = 0;
  fx  = 0;
  fy  = 0;
}

Fiber::~Fiber() {}


void Fiber::set_id(int a)
{
  fid = a;
}

void Fiber::set_x(double a)
{
  fx = a;
}

void Fiber::set_y(double a)
{
  fy = a;
}
void Fiber::set_pixel(int a)
{
  fpixel = a;
}
void Fiber::set_pin(int a)
{
  fpin = a;
}

void Fiber::dump()
{
  std::cout << " id " << id() << " x " << x() << " y " << y() << std::endl;
  std::cout << " x: " << fCoords.first << " y: " << fCoords.second << std::endl;
  std::cout << "pin: " << pin() <<  " pixel: " << pixel() << std::endl;
}

void Fiber::set_coords(std::pair<double,double> coords)
{
  fCoords=coords;
}

bool Fiber::near(Fiber b){

  double dx = this->x() - b.x();
  double dy = this->y() - b.y();

  if( fabs(dx) < 2 &&
      fabs(dy) < 2 ) return true;

  return false;

}

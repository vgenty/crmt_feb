#include <cmath>
#include "Fiber.h"

bool Fiber::near(Fiber b){

  double dx = this->x() - b.x();
  double dy = this->y() - b.y();

  if( fabs(dx) < 2 &&
      fabs(dy) < 2 ) return true;

  return false;

}

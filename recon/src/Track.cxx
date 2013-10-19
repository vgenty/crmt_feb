#include "Track.h"
#include "Fiber.h"

Track::Track()
{
  fid = 0;
}

Track::~Track() {}

void Track::add_fiber(Fiber a)
{
  fFibers.push_back(a);
}

void Track::set_id(int a)
{
  fid = a;
}

bool Track::contains(Fiber a)
{
  for(auto fib : fFibers){
    if( fib.id() == a.id() &&
	fib.x() == a.x() &&
	fib.y() == a.y() ) return true;
  }
  return false;
}

void Track::dump()
{
  std::cout << " track id " << id() << std::endl;
  for(auto fib : fFibers){
    fib.dump();
  }
}

void Track::fit()
{
  //  fFit = new TGraphErrors("fFit");
  fFit = new TGraphErrors();
  std::cout << "fitting" << std::endl;
}
void Track::calculate_angle()
{
  std::cout << "calculating angle" << std::endl;
}

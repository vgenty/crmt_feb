#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "Fiber.h"

class Track {

private:
  std::vector<Fiber> fFibers;
  int fid;

public:
  Track();
  ~Track();

  void add_fiber(Fiber a);
  void set_id(int a);
  int id() {return fid;}
  bool contains(Fiber a);
  int size(){return fFibers.size();}
  std::vector<Fiber> fibers(){return fFibers;}
  void dump();
  
};

#endif

#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "math.h"

#include "TMath.h"

#include "Fiber.h"

class Track {
  
private:
  std::vector<Fiber> fFibers;
  bool fHasOneEach;
  int fid;
  bool fChosen = false;

  
  
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
  //void fit();
  //void calculate_angle();
  //void reconstruct();
  void chosen(bool a){fChosen = a;};
  void hasoneeach(bool a);
  //bool is_chosen(){return fChosen;}
  bool is_hasoneeach(){return fHasOneEach;}
};

#endif

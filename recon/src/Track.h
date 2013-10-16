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
  Track(){
    fid=0;
  };
  ~Track(){};

  void add_fiber(Fiber a){
    fFibers.push_back(a);
  }

  void set_id(int a){fid = a;}
  int id(){return fid;}
  bool contains(Fiber a){
    for(auto fib : fFibers){
      if( fib.id() == a.id() &&
	  fib.x() == a.x() &&
	  fib.y() == a.y() ) return true;
    }
    return false;
  }
  int size(){return fFibers.size();}
  std::vector<Fiber> fibers(){return fFibers;}

  void dump(){
    std::cout << " track id " << id() << std::endl;
    for(auto fib : fFibers){
      fib.dump();
    }    
  }


};

#endif

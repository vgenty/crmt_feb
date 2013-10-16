#ifndef RECOMODULE_H
#define RECOMODULE_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "Fiber.h"
#include "Track.h"

class RecoModule {

private:
  std::string fEventFile;
  std::string fPinFile;
  int fEvent;
  int fFiberLocations[4][64];
  int fPinsToPixels[32][2];
  int fPixelsToFibers[16][8];
  std::map<int, std::vector<int> > fEventData;
  std::map<int, std::vector<int> > fHitFibers;
  std::vector<Fiber> fFibers;
  std::vector<Track> fTracks;
  
public:
  RecoModule();
  ~RecoModule();
  void getfiles(std::string efile, std::string pfile);
  void initpixels();
  void initpins(int pins_to_pixels[32][2]);
  void initfibs(int fiber_locations[4][64]);
  void initfile(std::map<int, std::vector<int> > &event_data);
  bool check_event(std::vector<int>& pin_data);
  void find_hit_fibers(std::vector<int>& hit_pins);
  void init_module();
  void fill_fibers();
  std::map<int, std::vector<int> > get_event_data(){return fEventData;}
  void clear(){fHitFibers.clear();fFibers.clear();fTracks.clear();}
  void get_location(int id, double *x, double *y, bool top);
  void print_fibers();
  void clusterize();
  void attach();
  void print_tracks();
  
};

#endif

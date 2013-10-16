#ifndef RECOMODULE_H
#define RECOMODULE_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>


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

public:
  RecoModule();
  ~RecoModule();
  void getfiles(std::string efile, std::string pfile);
  void initpixels(int pixels_to_fibers[16][8]);
  void initpins(int pins_to_pixels[32][2]);
  void initfibs(int fiber_locations[4][64]);
  void initfile(std::map<int, std::vector<int> > &event_data);
  bool check_event(std::vector<int>& pin_data);
  void find_hit_fibers(std::vector<int>& hit_pins,
		       std::map<int, std::vector<int> >& hit_fibers,
		       int pins_to_pixels[32][2],
		       int pixels_to_fibers[16][8]);
  void print_fibers_from_pins();
};

#endif

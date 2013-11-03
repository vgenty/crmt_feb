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
#include "Geometry.h"

class RecoModule {

private:
  std::string fEventFile;
  std::string fPinFile;
  int fEvent;
  int fEvents;
  int fFiberLocations[4][64];
  int fPinsToPixels[32][2];
  int fPixelsToFibers[16][8];
  std::map<int, std::vector<int> >   fEventData;
  std::map<int, std::vector<int> >   fHitFibers;
  std::map<int, std::map<int, std::pair<int,int> > > fFiberPinPixel;
  std::vector<Fiber>  fFibers;
  std::vector<Track>  fTracks;

  double fAngleThreshold;
  int fGoodTrackIndex;
  Geometry g;
  
public:
  RecoModule();
  ~RecoModule();
  void getfiles(std::string efile, std::string pfile);
  void initpixels();
  void initpins();
  void initfibs();
  void initfile();
  bool check_event(std::vector<int>& pin_data);
  void find_hit_fibers(std::vector<int>& hit_pins);
  void init_module();
  void fill_fibers();
  std::map<int, std::vector<int> > get_event_data(){return fEventData;}
  void clear(){fHitFibers.clear();fFibers.clear();fTracks.clear();fFiberPinPixel.clear(); fGoodTrackIndex = -1;}
  void get_location(int id, double *x, double *y, bool top);
  void get_pin(int id, int *pin ,bool top);
  void get_pixel(int id, int *pixel ,bool top);
  void print_fibers();
  void clusterize();
  void attach();
  void print_tracks();
  void reconstruct();
  void fill_root();
  void choose_angles();
  bool is_good_track();
  bool is_good_angle();
  bool conditions_are_met();
  // holy shit i'm a bad programmer
  // need only return the best track and the angle associated with it
  
  double get_Slope();
  double get_YInter();
  double get_Chi();
  double get_Ndf();
  double get_Pvalue();
  double get_Angle();
  double get_CosAngle();
  
};

#endif

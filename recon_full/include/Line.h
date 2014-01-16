#ifndef LINE_H 
#define LINE_H


#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "math.h"


#include "TGraphErrors.h"
#include "TF1.h"
#include "TMath.h"

#include "Track.h"


class Line {
  
private:
 
  bool fisXZ;
  
  TGraphErrors *fTG;
  std::vector<TF1*> fFits;
  std::pair<std::vector<Track>,std::vector<Track> > fTracks;
  
  TF1 fBestLine;
  double fSlope;
  double fYinter;
  double fAngle;
  double fCosAngle;
  double fChi;
  double fNdf;
  double fPvalue;
  
public:
  Line(bool isXZ);
  ~Line();
  
  void do_track_reco(std::vector<Track> tracks_top,std::vector<Track> tracks_bot);
  void fit_tracks();
  void clear_lines();
  void choose_best();

  void dump();
  
  double slope()    {return fSlope;}
  double yinter()   {return fYinter;}
  double angle()    {return fAngle;}
  double chi()      {return fChi;}
  double ndf()      {return fNdf;}
  double pvalue()   {return fPvalue;}
  double cosangle() {return fCosAngle;}
    
  
};

#endif

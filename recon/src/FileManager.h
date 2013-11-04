#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "Fiber.h"
#include "math.h"
#include "TFile.h"
#include "TTree.h"
#include "Track.h"
#include "Fiber.h"

class FileManager {
  
private:
  
  TFile *fReconData;
  TTree *fEventTree;
  TTree *fReconTree;
  
  int    fEventID;
  double fSlope;
  double fYInter;
  double fChi;
  double fNdf;
  double fPvalue;
  double fAngle;
  double fCosAngle;

  std::vector<double> fFibX;
  std::vector<double> fFibY;
  
  
  std::vector<int>   fHitPins;
  std::vector<std::string> fStringTracks;
  
  
public:
  FileManager();
  ~FileManager();
  
  std::string to_string(char let, int id);
  void open_file(std::string file_name);
  void fill_event_tree(int EventID,
		       double Slope,
		       double YInter,
		       double Chi,
		       double Ndf,
		       double Pvalue,
		       double Angle,
		       double CosAngle,
		       std::vector<int>   HitPins,
		       std::vector<Track> Tracks);
  
  void fill_reco_tree(std::vector<double> angles);
  void finish();
};

#endif

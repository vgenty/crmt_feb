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

class FileManager {
  
private:
  
  TFile *fReconData;
  TTree *fEventTree;
  TTree *fReconTree;
  
  int fEventID;
  double fSlope;
  double fYInter;
  double fChi;
  double fNdf;
  double fPvalue;
  double fAngle;
  double fCosAngle;
  std::vector<int> fHitPins;
  //std::ofstream pin_display;
  //std::ofstream track_display;
 
  
public:
  FileManager();
  ~FileManager();
  
  void open_file(std::string file_name);
  void fill_event_tree(int EventID,
		       double Slope,
		       double YInter,
		       double Chi,
		       double Ndf,
		       double Pvalue,
		       double Angle,
		       double CosAngle,
		       std::vector<int> HitPins);
  void fill_reco_tree(std::vector<double> angles);
  void finish();
};

#endif

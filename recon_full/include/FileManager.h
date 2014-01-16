#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <utility>
#include "math.h"
#include <cmath>

#include "TFile.h"
#include "TTree.h"

#include "Line.h"

class FileManager {
  
private:
  
  TFile *fReconData;
  TFile *fRawData;

  TTree *fRawDataTree;
  //TTree *fEventTree;
  TTree *fEventTreeXZ;
  TTree *fEventTreeYZ;
  TTree *fEventTree3D;
  
  std::string fRawDataFileName;
  std::map<int, std::vector<int> > fRawEventData;
  
  
  int fNRawEvents;

  double fSlope_XZ;
  double fYInter_XZ;
  double fChi_XZ;
  double fNdf_XZ;
  double fPvalue_XZ;
  double fAngle_XZ;
  double fCosAngle_XZ;
  

  double fSlope_YZ;
  double fYInter_YZ;
  double fChi_YZ;
  double fNdf_YZ;
  double fPvalue_YZ;
  double fAngle_YZ;
  double fCosAngle_YZ;
  
  
public:
  FileManager();
  ~FileManager();
  
  std::string to_string(char let, int id);
  std::map<int, std::vector<int> > get_raw_data(int event);
  void load_output_data(std::string file_name);

  int get_n_events(){return fNRawEvents;}

  void set_raw_data_name(std::string name);
  void fill_event_tree(std::pair<Line,Line>& lines);
  
  
  void finish();
};

#endif

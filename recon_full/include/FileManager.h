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
  
  //Do Reco
  TFile *fReconData;
  TFile *fRawData;
  
  
  std::string fRawDataFileName;  
  TTree *fRawDataTree;
  TTree *fTestVolumeTree;
  
  //TTree *fEventTree;
  TTree *fEventTreeXZ;
  TTree *fEventTreeYZ;
  TTree *fEventTree3D;
  
  std::map<int, std::vector<int> > fRawEventData;
  
  
  int fNRawEvents;

  double fSlope_XZ;
  double fYInter_XZ;
  double fSlopeErr_XZ;
  double fYInterErr_XZ;
  double fChi_XZ;
  double fRChi_XZ;
  double fNdf_XZ;
  double fPvalue_XZ;
  double fAngle_XZ;
  double fAngleErr_XZ;
  double fCosAngle_XZ;
  

  double fSlope_YZ;
  double fYInter_YZ;
  double fSlopeErr_YZ;
  double fYInterErr_YZ;
  double fChi_YZ;
  double fRChi_YZ;
  double fNdf_YZ;
  
  double fPvalue_YZ;
  double fAngle_YZ;
  double fAngleErr_YZ;
  double fCosAngle_YZ;

  std::vector<double> fFibsXZX;
  std::vector<double> fFibsXZY;
  std::vector<double> fFibsYZX;
  std::vector<double> fFibsYZY;
  
  
  double fPhi;
  double fTheta;

  //Display Reco
  TFile *fInputRecoData;
  TTree *fXZTree;
  TTree *fYZTree;
  TTree *f3DTree;
  
  int fEvent;
  double fGap;
public:
  FileManager();
  ~FileManager();
  
  //Do Reco
  std::string to_string(char let, int id);
  std::map<int, std::vector<int> > get_raw_data(int event);
  void load_output_data(std::string file_name);
  int get_n_events(){return fNRawEvents;}
  void set_raw_data_name(std::string name);
  void fill_event_tree(std::pair<Line,Line>& lines);
  void finish();
  void cleanup();
  
  //View Reco
  void setup_reco_viewer(std::string reco_file,int event);
  std::pair<std::pair<double,double>,std::pair<double,double> > get_slope_yinter();
  
  std::pair<std::pair<std::vector<double>,std::vector<double> >
	    ,std::pair<std::vector<double>,std::vector<double> > > get_hit_points();
  
  void set_gap_sim();
  void set_gap_reco();
  double get_gap(){return fGap;}
  void print_reco_results();
};

#endif

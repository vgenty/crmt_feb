#ifndef VIEWER_H
#define VIEWER_H

#include <map>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>

#include "TH2D.h"
#include "TCanvas.h"
//#include "TApplication.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TPaveText.h"
#include "TAxis.h"
#include "TPad.h"
#include "TFile.h"
#include "TTree.h"

#include "Geometry.h"


//0 = Top XZ
//1 = Top YZ
//2 = Bot XZ
//3 = Bot YZ


class Viewer{

private:
  
  double fGap;

  //TGraphErrors Line Draw HitPoints
  // TCanvas *tgCanv;
  std::pair<TMultiGraph*,TMultiGraph*> tgMgs;
  std::map<int, TGraphErrors*> tgModules;
  std::pair<TF1*,TF1*> tgRecoLines;
  std::pair<TGraphErrors*,TGraphErrors*> tgHitPoints = std::make_pair(new TGraphErrors(), new TGraphErrors());
  
  //TH2D Hit Fibers
  //TCanvas *thCanv;
  std::pair<TMultiGraph*,TMultiGraph*> thMgs;
  std::map<int, TH2D*> thModules;

  //Canvas of Info
  //TCanvas *infoCanv;
  std::vector<TPaveText*> infoText;
  
  //Local geometry
  std::map<int, Geometry*> g;
  
  std::string names[4] = {"Module 0", "Module 1"
			  "Module 2", "Module 3"};


  std::pair<std::pair<double,double>,std::pair<double,double> > SandY;
  std::pair<std::pair<std::vector<double>,std::vector<double> >
	    ,std::pair<std::vector<double>,std::vector<double> > > fHitPoints;
public:
  Viewer(double gap,
	 std::pair<std::pair<double,double>,std::pair<double,double> > sandy,
	 std::pair<std::pair<std::vector<double>,std::vector<double> >
	 ,std::pair<std::vector<double>,std::vector<double> > > hitpts);
  ~Viewer();
  
  void setup_geometry();
  void setup_lines();
  void setup_planes();
  void fill_TGs();
  void setup();
  
  TGraphErrors* get_modules(int a){return tgModules[a];}
  std::pair<TF1*,TF1*> get_recolines(){return tgRecoLines;}

  double flip_yinter(double& slope, double& yinter);
  double flip_slope(double& slope);

  std::pair<TGraphErrors*,TGraphErrors*> get_hit_points(){return tgHitPoints;}

};

#endif


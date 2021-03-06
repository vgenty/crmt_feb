#include <iostream>

#include <map>
#include <vector>

#include <fstream>
#include <cstdlib>
#include <stdlib.h>

#include "FileManager.h"
#include "Detector.h"
#include "Viewer.h"

#include "TCanvas.h"
#include "TApplication.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include "TLatex.h"
#include "TStyle.h"
#include "VStyle.h"

#include <sstream>

void usage()
{
  std::cout << "Reco usage:                       " << std::endl;
  std::cout << "./Reco -r [filename]              " << std::endl;
  std::cout << "./Reco -d [filename] -o [event #] " << std::endl;
  //std::cout << "./Reco -d [filename] -a           " << std::endl;
}

int main(int argc, char *argv[])
{
  
  if (argc == 1){
    usage();
    exit(0);
  }
  
  std::vector<std::string> args;
  for(int k = 1  ; k < argc; ++k)
    args.push_back(argv[k]);
  
  
  FileManager *fm = new FileManager();
  int nevents;
  
  if(args[0] == "-r") { // go ahead with reco
    
    std::cout << "Loading RAW file: " << args[1] << "\n";
    fm->set_raw_data_name(args[1]);
    nevents = fm->get_n_events();
    std::cout << "Got N: " << nevents << " events" << std::endl;
    auto gap = fm->get_gap();
    std::cout << "Got gap: " << gap << std::endl;
    std::cout << "Creating RECO file: " << "recodata.root" << "\n";
    fm->load_output_data("./output/recodata.root");
    
    std::cout << "Creating Detector\n";
    Detector *dd = new Detector(gap);
    
    bool good     = false;
    int  good_cnt = 0;

    for(int i = 0; i < nevents; ++i) {
      auto raw_data   = fm->get_raw_data(i);
      auto recon_data = dd->recon_event(raw_data,good);
      if(good){
	fm->fill_event_tree(recon_data);
	good_cnt++;
      }
      std::cout << "i: " << i << ", good: " << good_cnt <<std::endl;fflush(stdout);
      
    }

    fm->finish();
    
  }   
  if(args[0] == "-d") { // go ahead with display 
    set_style(); //from VStyle.h
    printf("Setting up event display\n"); fflush(stdout);
    const auto event = args[3].c_str();
    fm->setup_reco_viewer(args[1],atoi(event));
    auto gap = fm->get_gap();
    
    Viewer *vv = new Viewer(gap, //gap
			    fm->get_slope_yinter(),
			    fm->get_hit_points()); //20.0 gap
    
    vv->setup();
 
    fm->print_reco_results();
    
    TApplication *tapp = new TApplication("app",&argc,argv);
    TCanvas *can = new TCanvas("tgModules","tgModules",1300,570);
    TPad *padXZ = new TPad("padXZ","padXZ",0.0,0.0,0.5,1.0);
    TPad *padYZ = new TPad("padYZ","padYZ",0.5,0.0,1.0,1.0);
  
    padXZ->cd();
    TMultiGraph *tmgXZ = new TMultiGraph();
    
    tmgXZ->Add(vv->get_modules(1));
    tmgXZ->Add(vv->get_modules(3));
    
    vv->get_hit_points().first->SetMarkerStyle(8);
    tmgXZ->Add(vv->get_hit_points().first);
    tmgXZ->Draw("AP");
    (vv->get_recolines().first)->Draw("SAMES");
    tmgXZ->SetTitle(";z#rightarrow;x#rightarrow");
    
    
    padYZ->cd();
    TMultiGraph *tmgYZ = new TMultiGraph();
    tmgYZ->Add(vv->get_modules(0));
    tmgYZ->Add(vv->get_modules(2));
    vv->get_hit_points().second->SetMarkerStyle(8);
    tmgYZ->Add(vv->get_hit_points().second);
    tmgYZ->Draw("AP");
    (vv->get_recolines().second)->Draw("SAMES");

    tmgYZ->SetTitle(";z#rightarrow;y#rightarrow");
  
    can->cd();
    padXZ->Draw();
    padYZ->Draw();
    
    //Give TMultiGraph Axis
    padXZ->cd();
    tmgXZ->GetXaxis()->CenterTitle();
    tmgXZ->GetYaxis()->CenterTitle();
    TPaveText *XZ_title = new TPaveText(0.36,.93,.7,.95,"brNDC");
    XZ_title->SetTextSize(22);
    XZ_title->SetTextFont(63);
    XZ_title->SetBorderSize(0);
    XZ_title->SetFillColor(0);
    XZ_title->AddText("XZ plane");
    XZ_title->Draw();
    padXZ->Update();
    padXZ->Modified();
     
    padYZ->cd();
    tmgYZ->GetXaxis()->CenterTitle();
    tmgYZ->GetYaxis()->CenterTitle();
    TPaveText *YZ_title = new TPaveText(0.36,.93,.7,.95,"brNDC");
    YZ_title->SetTextSize(22);
    YZ_title->SetTextFont(63);
    YZ_title->SetBorderSize(0);
    YZ_title->SetFillColor(0);
    YZ_title->AddText("YZ plane");
    YZ_title->Draw();
    padYZ->Update();
    padYZ->Modified();
    
    tapp->Run();


  }
  
  return 0;  
  
}

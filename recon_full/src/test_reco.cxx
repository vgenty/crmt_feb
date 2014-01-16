#include <iostream>

#include <map>
#include <vector>

#include <fstream>
#include <cstdlib>
#include <stdlib.h>

#include "FileManager.h"
#include "Detector.h"

#include "TTree.h"
#include "TFile.h"

#include <sstream>


void usage()
{
  std::cout << "Reco usage:                       " << std::endl;
  std::cout << "./Reco -r [filename]              " << std::endl;
  //std::cout << "./Reco -d [filename] -o [event #] " << std::endl;
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
    std::cout << "Creating RECO file: " << "recodata.root" << "\n";
    fm->load_output_data("./output/recodata.root");

    std::cout << "Creating Detector\n";
    Detector *dd = new Detector(20.0); //gap is 20, will read from ROOT file at some point
    
    bool good;
    int good_cnt = 1;
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
    
    /*
  if(args[0] == "-d") { // go ahead with reco
    std::stringstream ss;
    if(args.size() > 2){
      ss << "cd viewer && python view_me.py ";
      ss << args[3];
      std::system((ss.str()).c_str());    
    }
    
    else{
      std::cout << "Not implemented yet, aborting" << std::endl;
    }
  }
  
    */
  }
  return 0;  
}    

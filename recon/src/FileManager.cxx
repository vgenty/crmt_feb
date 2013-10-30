#include <iostream>
#include <fstream>
#include "FileManager.h"
#include "Track.h"
#include "Fiber.h"
#include <cmath>
FileManager::FileManager()
{
  //std::cout << std::endl;
}

FileManager::~FileManager() {}

void FileManager::make_tree(std::string file_name, int n_events,
		       std::vector<double>& fAngles)
{
  fReconData = new TFile(file_name.c_str(), "RECREATE");
  fTree      = new TTree("Recon Tree","Recon Tree");  
  fTree->Branch("ReconAngles", &fAngles);
   
  //make cosine plot
  std::vector<double> lCosines;
  for(int i=0;i<fAngles.size();++i)
    lCosines.push_back(cos(fAngles.at(i)));
  fTree->Branch("ReconAngles_Cos", &lCosines);
  fTree->Fill();
  fReconData->Write();
  fReconData->Close();
}



void FileManager::make_display(std::vector<int>& pins,
			      std::vector<Track>& fTracks){
  
  if (fTracks.size() >= 1){
    pin_display.open ("pins.txt",std::ios::app);
    track_display.open ("tracks.txt",std::ios::app);
    pin_display << "1";
    
    for (auto pin : pins)
      pin_display << "," << pin ;
    
    for (auto tracks : fTracks){
      for (auto fiber : tracks.fibers()){
	if(fiber.y() <= 1)
	  track_display << "t" << fiber.id() << ",";
	else 
	  track_display << "b" << fiber.id() << ",";
      }
      track_display << "n,";	    
    }
    pin_display << std::endl;
    pin_display.close();
    track_display << std::endl;
    track_display.close();
  }
}

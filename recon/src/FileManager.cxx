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

void FileManager::open_file(std::string file_name)
{
  
  fReconData = new TFile(file_name.c_str(), "RECREATE");
  fEventTree = new TTree("Event Tree","Recon Tree");  
  fEventTree->Branch("Event ID",     &fEventID,  "EventID/I"  );
  fEventTree->Branch("Slope",        &fSlope,    "Slope/D"    );
  fEventTree->Branch("Yinter",       &fYInter,   "Yinter/D"   );
  fEventTree->Branch("fChi",         &fChi,      "Chi/D"      );
  fEventTree->Branch("fNdf",         &fNdf,      "Ndf/D"      );
  fEventTree->Branch("fPvalue",      &fPvalue,   "Pvalue/D"   ); 
  fEventTree->Branch("fAngle",       &fAngle,    "Angle/D"    ); 
  fEventTree->Branch("fCosAngle",    &fCosAngle, "CosAngle/D" ); 
  fEventTree->Branch("fHitPins",     &fHitPins                );
}


void FileManager::fill_event_tree(int EventID,
				  double Slope, double YInter, double Chi,
				  double Ndf,   double Pvalue, double Angle,
				  double CosAngle, std::vector<int> HitPins)
{
  
  fEventID  =  EventID;
  fSlope    =  Slope;
  fYInter   =  YInter;
  fChi      =  Chi;
  fNdf      =  Ndf;
  fPvalue   =  Pvalue;
  fHitPins  =  HitPins; //this might error trying to copy vector
  fAngle    =  Angle;
  fCosAngle =  CosAngle;

  fHitPins  = HitPins;
  fEventTree->Fill();
  
}

void FileManager::finish(){
  fEventTree->Write();
  fReconData->Close();
}




/*
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
*/

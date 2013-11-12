#include <iostream>
#include <fstream>
#include "FileManager.h"
#include "Track.h"
#include "Fiber.h"
#include <cmath>
#include <stdio.h>
#include <sstream>
#include "TMath.h"

FileManager::FileManager()
{
  //std::cout << std::endl;
}

FileManager::~FileManager() {}

std::string FileManager::to_string(char let, int id)
{
  std::ostringstream ss;
  ss << let << id;
  return ss.str();
}
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
  fEventTree->Branch("fStringTracks",&fStringTracks           );
  fEventTree->Branch("fFibX",&fFibX             );
  fEventTree->Branch("fFibY",&fFibY             );
  fEventTree->Branch("fx_Slope",&fx_Slope       );
  fEventTree->Branch("fx_Yinter",&fx_Yinter     );
  fEventTree->Branch("fx_Zvalue",&fx_Zvalue     );
  fEventTree->Branch("fx_LowZValue" ,  &fx_LowZValue, "x_LowZValue/D");
  fEventTree->Branch("fx_LowSlope"  ,  &fx_LowSlope ,   "x_LowSlope/D");
  fEventTree->Branch("fx_LowYinter" ,  &fx_LowYinter, "x_LowYinter/D");
  //fEventTree->Branch("fxp_Zvalue",     &fxp_Zvalue     );
  //fEventTree->Branch("fxp_LowZValue" ,  &fxp_LowZValue, "xp_LowZValue/D");

}


void FileManager::fill_event_tree(int EventID,
				  double Slope, double YInter, double Chi,
				  double Ndf,   double Pvalue, double Angle,
				  double CosAngle, std::vector<int> HitPins,
				  std::vector<Track> Tracks,
				  std::map<std::pair<double,double> , double  > Xvalues)
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
  

  double current = 0;
  fx_LowZValue  = 10000;
  
  for(auto coord_pair : Xvalues){
    current = coord_pair.second;
    
    if (current < fx_LowZValue){
      fx_LowZValue = current;
      fx_LowSlope  = (coord_pair.first).first;
      fx_LowYinter = (coord_pair.first).second;
      fxp_LowZValue = TMath::Prob(current,4);
    }

    fx_Slope.push_back((coord_pair.first).first);
    fx_Yinter.push_back((coord_pair.first).second);
    fx_Zvalue.push_back(coord_pair.second);
    fxp_Zvalue.push_back(TMath::Prob(coord_pair.second,4));  

  }


  fPvalue = TMath::Prob(fChi,fNdf);
  for (auto tracks : Tracks){
    for (auto fiber : tracks.fibers()){
      if(fiber.y() <= 1)
	fStringTracks.push_back(to_string('t',fiber.id()));
      else 
	fStringTracks.push_back(to_string('b',fiber.id()));
    }
    
    if (tracks.is_chosen())  { //do it again if its the chosen one..
      for (auto fiber : tracks.fibers()){
	if(fiber.y() <= 1)
	  fStringTracks.push_back(to_string('t',fiber.id()));
	else 
	  fStringTracks.push_back(to_string('b',fiber.id()));
	
	fFibX.push_back((fiber.get_coords()).first);
	fFibY.push_back((fiber.get_coords()).second);
	
      }
      
      
      
    }
    
  }
  
  fHitPins      = HitPins;
  fEventTree->Fill();
  fStringTracks.clear(); //empty string tracks after fill??
  fFibY.clear(); //empty y
  fFibX.clear(); //empt  x
  fx_Slope.clear();
  fx_Yinter.clear();
  fx_Zvalue.clear();
  fxp_Zvalue.clear();
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
    

    pin_display << std::endl;
    pin_display.close();
    track_display << std::endl;
    track_display.close();
  }
}
*/


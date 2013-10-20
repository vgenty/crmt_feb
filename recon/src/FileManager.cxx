#include <iostream>
#include "FileManager.h"
#include "Track.h"
FileManager::FileManager()
{
  std::cout << "hi im filemanager" << std::endl;
}

FileManager::~FileManager() {}

void FileManager::make_tree(std::string file_name, int n_events,
		       std::vector<double>& fAngles)
{
  fReconData = new TFile(file_name.c_str(), "RECREATE");
  fTree      = new TTree("Recon Tree","Recon Tree");  
  fTree->Branch("ReconAngles", &fAngles);
  fTree->Fill();
  fReconData->Write();
  fReconData->Close();
}

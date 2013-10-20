#include <iostream>
#include "FileManager.h"
#include "Track.h"
FileManager::FileManager()
{
  std::cout << "hi" << std::endl;
}

FileManager::~FileManager() {}

void FileManager::make_tree(std::string file_name, int n_events,
		       std::vector<double>& fAngles)
{
  std::cout << "oh boy1 " << std::endl;
  fReconData = new TFile(file_name.c_str(), "RECREATE");
  fTree      = new TTree("Recon Tree","Recon Tree");  
  std::cout << "oh boy " << std::endl;
  fTree->Branch("ReconAngles", &fAngles);
  std::cout << "oh boy2" << std::endl;
  fTree->Fill();
  std::cout << "oh boy3" << std::endl;
  for (auto angle : fAngles)
    std::cout << "filled angle: " << angle << std::endl;
  //  fReconData->Close();
}

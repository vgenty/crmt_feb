#include "FileManager.h"

FileManager::FileManager()
{
  //empty constructor
}

FileManager::~FileManager() {}

std::string FileManager::to_string(char let, int id)
{
  std::ostringstream ss;
  ss << let << id;
  return ss.str();
}

std::map<int, std::vector<int> > FileManager::get_raw_data(int event){
  fRawEventData.clear();

  //bool coincidence;
  bool tMod0[32];
  bool tMod1[32];
  bool tMod2[32];
  bool tMod3[32];
  
  fRawDataTree->SetBranchAddress("PinsArray0",tMod0);
  fRawDataTree->SetBranchAddress("PinsArray1",tMod1);
  fRawDataTree->SetBranchAddress("PinsArray2",tMod2);
  fRawDataTree->SetBranchAddress("PinsArray3",tMod3);
  //fRawDataTree->SetBranchAddress("Coincidence",&coincidence);
  TBranch *b_PinsArray0  = fRawDataTree->GetBranch("PinsArray0");
  TBranch *b_PinsArray1  = fRawDataTree->GetBranch("PinsArray1");
  TBranch *b_PinsArray2  = fRawDataTree->GetBranch("PinsArray2");
  TBranch *b_PinsArray3  = fRawDataTree->GetBranch("PinsArray3");
  //TBranch *b_Coincidence = fRawDataTree->GetBranch("Coincidence");
  
  
  
  b_PinsArray0->GetEvent(event);   
  b_PinsArray1->GetEvent(event);
  b_PinsArray2->GetEvent(event);
  b_PinsArray3->GetEvent(event);
  //b_Coincidence->GetEvent(event);
  
  for (int xx = 0; xx < 32; ++xx){
    if (tMod0[xx]) fRawEventData[0].push_back(xx);
    if (tMod1[xx]) fRawEventData[1].push_back(xx);
    if (tMod2[xx]) fRawEventData[2].push_back(xx);
    if (tMod3[xx]) fRawEventData[3].push_back(xx);  
  }
 
  return fRawEventData;
}

void FileManager::set_raw_data_name(std::string name){
  
  fRawDataFileName = name;
  fRawData     = new TFile(fRawDataFileName.c_str(),"READ");
  //fRawData     = new TFile("input/afile.root","READ");
  fRawDataTree = (TTree*)fRawData->Get("SimulationTree");   
  fNRawEvents = fRawDataTree->GetEntries();
}

void FileManager::load_output_data(std::string file_name)
{
  
  fReconData    = new TFile(file_name.c_str(), "RECREATE");
  
  fEventTreeXZ  = new TTree("Event Tree XZ","Recon Tree XZ");  
  fEventTreeXZ->Branch("Slope"          , &fSlope_XZ       , "Slope/D"       );
  fEventTreeXZ->Branch("Yinter"         , &fYInter_XZ      , "Yinter/D"      );
  fEventTreeXZ->Branch("fChi"           , &fChi_XZ         , "Chi/D"         );
  fEventTreeXZ->Branch("fNdf"           , &fNdf_XZ         , "Ndf/D"         );
  fEventTreeXZ->Branch("fPvalue"        , &fPvalue_XZ      , "Pvalue/D"      ); 
  fEventTreeXZ->Branch("fAngle"         , &fAngle_XZ       , "Angle/D"       ); 
  fEventTreeXZ->Branch("fCosAngle"      , &fCosAngle_XZ    , "CosAngle/D"    ); 


      
  
  fEventTreeYZ  = new TTree("Event Tree YZ","Recon Tree YZ");  
  fEventTreeYZ->Branch("Slope"          , &fSlope_YZ       , "Slope/D"       );
  fEventTreeYZ->Branch("Yinter"         , &fYInter_YZ      , "Yinter/D"      );
  fEventTreeYZ->Branch("fChi"           , &fChi_YZ         , "Chi/D"         );
  fEventTreeYZ->Branch("fNdf"           , &fNdf_YZ         , "Ndf/D"         );
  fEventTreeYZ->Branch("fPvalue"        , &fPvalue_YZ      , "Pvalue/D"      ); 
  fEventTreeYZ->Branch("fAngle"         , &fAngle_YZ       , "Angle/D"       ); 
  fEventTreeYZ->Branch("fCosAngle"      , &fCosAngle_YZ    , "CosAngle/D"    ); 
  
  fEventTree3D  = new TTree("Event Tree 3D","Recon Tree 3D");
  
}


void FileManager::fill_event_tree(std::pair<Line,Line>& lines) {

  fSlope_XZ    =  lines.first.slope();
  fYInter_XZ   =  lines.first.yinter();
  fChi_XZ      =  lines.first.angle();
  fNdf_XZ      =  lines.first.chi();
  fPvalue_XZ   =  lines.first.pvalue();
  fAngle_XZ    =  lines.first.angle();
  fCosAngle_XZ =  lines.first.cosangle();

  
  fSlope_YZ    =  lines.second.slope();
  fYInter_YZ   =  lines.second.yinter();
  fChi_YZ      =  lines.second.angle();
  fNdf_YZ      =  lines.second.chi();
  fPvalue_YZ   =  lines.second.pvalue();
  fAngle_YZ    =  lines.second.angle();
  fCosAngle_YZ =  lines.second.cosangle();
  
  fEventTreeXZ->Fill();
  fEventTreeYZ->Fill();
  
}


void FileManager::finish(){
  fEventTreeXZ->Write();
  fEventTreeYZ->Write();
  
  fReconData->Close();
}


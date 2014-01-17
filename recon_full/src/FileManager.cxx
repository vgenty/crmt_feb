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
  fEventTreeXZ->Branch("fFibXZX"        , &fFibsXZX); 
  fEventTreeXZ->Branch("fFibXZY"        , &fFibsXZY); 


      
  
  fEventTreeYZ  = new TTree("Event Tree YZ","Recon Tree YZ");  
  fEventTreeYZ->Branch("Slope"          , &fSlope_YZ       , "Slope/D"       );
  fEventTreeYZ->Branch("Yinter"         , &fYInter_YZ      , "Yinter/D"      );
  fEventTreeYZ->Branch("fChi"           , &fChi_YZ         , "Chi/D"         );
  fEventTreeYZ->Branch("fNdf"           , &fNdf_YZ         , "Ndf/D"         );
  fEventTreeYZ->Branch("fPvalue"        , &fPvalue_YZ      , "Pvalue/D"      ); 
  fEventTreeYZ->Branch("fAngle"         , &fAngle_YZ       , "Angle/D"       ); 
  fEventTreeYZ->Branch("fCosAngle"      , &fCosAngle_YZ    , "CosAngle/D"    ); 
  fEventTreeYZ->Branch("fFibYZX"        , &fFibsYZX); 
  fEventTreeYZ->Branch("fFibYZY"        , &fFibsYZY); 

  fEventTree3D  = new TTree("Event Tree 3D","Recon Tree 3D");
  
}


void FileManager::fill_event_tree(std::pair<Line,Line>& lines) {

  fSlope_XZ    =  lines.first.slope();
  fYInter_XZ   =  lines.first.yinter();
  fChi_XZ      =  lines.first.chi();
  fNdf_XZ      =  lines.first.ndf();
  fPvalue_XZ   =  lines.first.pvalue();
  fAngle_XZ    =  lines.first.angle();
  fCosAngle_XZ =  lines.first.cosangle();


  //Slightly more painful way to write this data to a tree
  //XZ
  for (auto& fiber: lines.first.get_best_tracks().first.fibers()){
    fFibsXZX.push_back(fiber.coords().first);
    fFibsXZY.push_back(fiber.coords().second);
  }
  for (auto& fiber: lines.first.get_best_tracks().second.fibers()){
    fFibsXZX.push_back(fiber.coords().first);
    fFibsXZY.push_back(fiber.coords().second);
  }
  
  //YZ
  
  for (auto& fiber: lines.second.get_best_tracks().first.fibers()){
    fFibsYZX.push_back(fiber.coords().first);
    fFibsYZY.push_back(fiber.coords().second);
  }
  for (auto& fiber: lines.second.get_best_tracks().second.fibers()){
    fFibsYZX.push_back(fiber.coords().first);
    fFibsYZY.push_back(fiber.coords().second);
  }
  
  
  
  fSlope_YZ    =  lines.second.slope();
  fYInter_YZ   =  lines.second.yinter();
  fChi_YZ      =  lines.second.chi();
  fNdf_YZ      =  lines.second.ndf();
  fPvalue_YZ   =  lines.second.pvalue();
  fAngle_YZ    =  lines.second.angle();
  fCosAngle_YZ =  lines.second.cosangle();
  
  fEventTreeXZ->Fill();
  fEventTreeYZ->Fill();

  
  cleanup();
  
  
}


void FileManager::cleanup(){
  fFibsXZX.clear();
  fFibsXZY.clear();
  fFibsYZX.clear();
  fFibsYZY.clear();
}

void FileManager::finish(){
  fEventTreeXZ->Write();
  fEventTreeYZ->Write();  
  fReconData->Close();
}

void FileManager::setup_reco_viewer(std::string reco_file,int event){  
  printf("okay");
  fInputRecoData = new TFile(reco_file.c_str(),"READ");
  fXZTree = (TTree*)fInputRecoData->Get("Event Tree XZ");
  fYZTree = (TTree*)fInputRecoData->Get("Event Tree YZ"); 

  fEvent = event;
  printf("fEvent: %d", fEvent);
}



std::pair<std::pair<std::vector<double>,std::vector<double> >
	  ,std::pair<std::vector<double>,std::vector<double> > > FileManager::get_hit_points(){
  
  
  std::vector<double> *fibsXZX = 0;
  std::vector<double> *fibsXZY = 0;
  std::vector<double> *fibsYZX = 0;
  std::vector<double> *fibsYZY = 0;
    
  /*
  std::vector<double> *fibsXZX;
  std::vector<double> *fibsXZY;
  std::vector<double> *fibsYZX;
  std::vector<double> *fibsYZY;
  */
  /*
  std::vector<double> *fibsXZX = new std::vector<double>;
  std::vector<double> *fibsXZY = new std::vector<double>;
  std::vector<double> *fibsYZX = new std::vector<double>;
  std::vector<double> *fibsYZY = new std::vector<double>;
  */

  //TBranch *b_fibsXZX=0;
  //TBranch *b_fibsXZY=0;
  //TBranch *b_fibsYZX=0;
  //TBranch *b_fibsYZY=0; 
  
  /*
  fXZTree->SetBranchAddress("fFibXZX" ,&fibsXZX,&b_fibsXZX); //passing address of pointer
  fXZTree->SetBranchAddress("fFibXZY" ,&fibsXZY,&b_fibsXZY);
  fYZTree->SetBranchAddress("fFibYZX" ,&fibsYZX,&b_fibsYZX);
  fYZTree->SetBranchAddress("fFibYZY" ,&fibsYZY,&b_fibsYZY);
  */

  fXZTree->SetBranchAddress("fFibXZX" ,&fibsXZX); //passing address of pointer
  fXZTree->SetBranchAddress("fFibXZY" ,&fibsXZY);
  fYZTree->SetBranchAddress("fFibYZX" ,&fibsYZX);
  fYZTree->SetBranchAddress("fFibYZY" ,&fibsYZY);

  TBranch *b_fibsXZX = fXZTree->GetBranch("fFibXZX");
  TBranch *b_fibsXZY = fXZTree->GetBranch("fFibXZY");
  TBranch *b_fibsYZX=  fYZTree->GetBranch("fFibYZX");;
  TBranch *b_fibsYZY=  fYZTree->GetBranch("fFibYZY");; 
  
  
  printf("a\n");
  b_fibsXZX->GetEvent(fEvent);
  b_fibsXZY->GetEvent(fEvent);
  b_fibsYZX->GetEvent(fEvent);
  b_fibsYZY->GetEvent(fEvent);
  //fXZTree->GetEntry(fEvent);
  printf("b\n");
  //fYZTree->GetEntry(fEvent);
  printf("c\n");

  
  printf("fibsXZX.size(): %lu\n", fibsXZX->size());
  printf("fibsXZY.size(): %lu\n", fibsXZY->size());
  printf("fibsYZX.size(): %lu\n", fibsYZX->size());
  printf("fibsYZY.size(): %lu\n", fibsYZY->size());
  
  for(auto value : *fibsXZX)
    printf("fibsXZX value: %f\n",value);
  printf("\n");
  for(auto value : *fibsXZY)
    printf("fibsXZY value: %f\n",value);
  printf("\n");
  for(auto value : *fibsYZX)
    printf("fibsYZX value: %f\n",value);
  printf("\n");
  for(auto value : *fibsYZY)
    printf("fibsYZY value: %f\n",value);
   
  
  
  printf("returning\n");
  return std::make_pair(std::make_pair(*fibsXZY,*fibsXZX),
  			std::make_pair(*fibsYZY,*fibsYZX));
     
  
}

std::pair<std::pair<double,double>,std::pair<double,double> > FileManager::get_slope_yinter(){
  
  
  //std::pair<std::pair<double,double>,std::pair<double,double> > localinfo;
 
  double sXZ,yXZ,sYZ,yYZ;
  
  fXZTree->SetBranchAddress("Slope" ,&sXZ);
  fXZTree->SetBranchAddress("Yinter",&yXZ);
  fYZTree->SetBranchAddress("Slope" ,&sYZ);
  fYZTree->SetBranchAddress("Yinter",&yYZ);
  
  fXZTree->GetEntry(fEvent);
  fYZTree->GetEntry(fEvent);
  printf("\nsXZ: %f sYZ: %f yXZ: %f yYZ: %f\n",sXZ,sYZ,yXZ,yYZ);
  return std::make_pair(std::make_pair(sXZ,sYZ),std::make_pair(yXZ,yYZ));
  

}

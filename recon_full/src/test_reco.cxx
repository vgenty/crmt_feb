#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>

#include "Fiber.h"
#include "RecoModule.h"
#include "Track.h"
#include "FileManager.h"
#include "ParameterSpace.h"
#include "Detector.h"


#include "TTree.h"
#include "TFile.h"

#include <sstream>


void usage()
{
  std::cout << "Reco usage:                       " << std::endl;
  std::cout << "./Reco -r [filename]              " << std::endl;
  std::cout << "./Reco -r [filename] -ps          " << std::endl;
  std::cout << "./Reco -d [filename] -o [event #] " << std::endl;
  std::cout << "./Reco -d [filename] -a           " << std::endl;
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
  
  if(args[0] == "-r") { // go ahead with reco
    Detector       *dd = new Detector(100.0);
    //RecoModule     *mm = new RecoModule();
    //FileManager    *fm = new FileManager();
    //ParameterSpace *ps = new ParameterSpace();
    
    bool _ps = false;
    /*
    if(args.size() > 2 && args[2] == "-ps") {// build parameterspace?
      ps->set_NSlopeDivisions(200);
      ps->set_NYinterDivisions(80);
      ps->Fill_Space();
      fm->yes_parameterspace();
      _ps = true;
    }
    */
    
    
    //fm->open_file("./output/recodata.root");
    
    //std::map<std::pair<double,double> , double  > Xvalues;
    //std::map<int, std::vector<int> >::iterator itr;
    //    int evt=1;
    //    std::map<int, std::vector<int> > eventdata = mm->get_event_data();//will soon come from filemanager
    std::map<int, std::vector<int> > eventdata;

    TFile *fff = new TFile("./input/afile.root","READ");
    TTree *simtree = (TTree*)(fff->Get("SimulationTree"));
    
    bool tMod0[32];
    bool tMod1[32];
    bool tMod2[32];
    bool tMod3[32];

    simtree->SetBranchAddress("PinsArray0",tMod0);
    simtree->SetBranchAddress("PinsArray1",tMod1);
    simtree->SetBranchAddress("PinsArray2",tMod2);
    simtree->SetBranchAddress("PinsArray3",tMod3);
    TBranch *b_PinsArray0 = simtree->GetBranch("PinsArray0");
    TBranch *b_PinsArray1 = simtree->GetBranch("PinsArray1");
    TBranch *b_PinsArray2 = simtree->GetBranch("PinsArray2");
    TBranch *b_PinsArray3 = simtree->GetBranch("PinsArray3");


    for ( int ient = 0; ient < simtree-> GetEntries(); ++ient ) {
      
      b_PinsArray0->GetEvent(ient);   
      b_PinsArray1->GetEvent(ient);
      b_PinsArray2->GetEvent(ient);
      b_PinsArray3->GetEvent(ient);
    
      for (int xx = 0; xx < 32; ++xx){
	if (tMod0[xx]) eventdata[0].push_back(xx);
	if (tMod1[xx]) eventdata[1].push_back(xx);
	if (tMod2[xx]) eventdata[2].push_back(xx);
	if (tMod3[xx]) eventdata[3].push_back(xx);  
      }
      break; 

    }

    dd->recon_event(eventdata);
    
    /*
      for(itr=eventdata.begin();itr!=eventdata.end();++itr){
      
      if(evt%10==0){
	std::cout << "-------------------" << std::endl;
	std::cout << "Event:   " << evt << std::endl;
      }
      
      mm -> find_hit_fibers((*itr).second);
      mm -> fill_fibers();
      mm -> clusterize();
      mm -> attach();
      mm -> choose_angles();
      if (mm->conditions_are_met()){
	if(_ps) ps -> TrackOpener(mm->get_Tracks());
	if(_ps) fm -> fill_event_tree(evt,
				      mm->get_Slope(),mm->get_YInter(),
				      mm->get_Chi(),mm->get_Ndf(),
				      mm->get_Pvalue(),mm->get_Angle(),
				      mm->get_CosAngle(),(*itr).second,
				      mm->get_Tracks(),
				      ps->CreateSpace()); //only diff is this line
	else    fm -> fill_event_tree(evt,
				      mm->get_Slope(),mm->get_YInter(),
				      mm->get_Chi(),mm->get_Ndf(),
				      mm->get_Pvalue(),mm->get_Angle(),
				      mm->get_CosAngle(),(*itr).second,
				      mm->get_Tracks(),
				      Xvalues);
	evt++;	
      }
      mm -> clear();   
    }
    fm -> finish();
  }
    */

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

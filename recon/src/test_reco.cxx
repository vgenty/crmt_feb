#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "Fiber.h"
#include "RecoModule.h"
#include "Track.h"
#include "FileManager.h"
#include "ParameterSpace.h"

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
  std::cout << "Have : " << (argc-1) << " arguments" << std::endl;
  std::vector<std::string> args;
  
  for(int k = 1  ; k < argc; ++k)
    args.push_back(argv[k]);
  
  if(args[0] == "-r") { // go ahead with reco
    RecoModule     *mm = new RecoModule();
    FileManager    *fm = new FileManager();
    ParameterSpace *ps = new ParameterSpace();
    
    bool _ps = false;

    if(args.size() > 2 && args[2] == "-ps") {// build parameterspace?
      ps->set_NSlopeDivisions(500); // Set number of divisions in parameter space
      ps->set_NYinterDivisions(80); // Set number of divisions in parameter space
      ps->Fill_Space();
      fm->yes_parameterspace();
      _ps = true;
    }

    mm->getfiles(args[1].c_str(),"./input/pins.csv");
    mm->init_module(); 
    fm->open_file("./output/recodata.root");
    
    std::map<std::pair<double,double> , double  > Xvalues;
    std::map<int, std::vector<int> >::iterator itr;
    int evt=1;
    std::map<int, std::vector<int> > eventdata = mm->get_event_data();
    
    for(itr=eventdata.begin();itr!=eventdata.end();++itr){
      
      if(evt%10==0){
	std::cout << "-------------------" << std::endl;
	std::cout << "Event: " << evt << std::endl;
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
				      ps->CreateSpace());
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
  return 0;  
}

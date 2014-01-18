#include "Viewer.h"


Viewer::Viewer(double gap,
	       std::pair<std::pair<double,double>,std::pair<double,double> > sandy,
	       std::pair<std::pair<std::vector<double>,std::vector<double> >
	       ,std::pair<std::vector<double>,std::vector<double> > > hitpts){ //can't pass sandy was reference because its the return of another function
  fGap = gap;
  SandY = sandy;
  fHitPoints = hitpts;
}


Viewer::~Viewer(){}


void Viewer::setup_geometry(){
  for (int i=0; i<4; ++i){ //loop over four modules
      g[i] = new Geometry(i,fGap); 
  }
}
double Viewer::flip_slope(double& slope){
  return 1.0/slope;
}
double Viewer::flip_yinter(double& slope, double& yinter){
  return (-1.0*yinter)*slope;
}

void Viewer::setup_lines(){
  std::stringstream ss;
  
  ss << SandY.first.first << "*x + " << SandY.second.first;
  tgRecoLines.first = new TF1("fit_function_XZ", (ss.str()).c_str(),0.0,2.0*74.0+fGap);


  ss.str(""); //string stream


  ss << SandY.first.second << "*x + " << SandY.second.second;
  tgRecoLines.second = new TF1("fit_function_YZ", (ss.str()).c_str(),0.0,2.0*74.0+fGap);
  

}



void Viewer::fill_TGs() {
  int n = 0;
  double xx = 0.0;
  double yy = 0.0;

  for (int i=0; i<4; ++i){ //loop over four modules
    tgModules[i] = new TGraphErrors();
    for(int y=0;y<4;++y){
      for(int x=0 ; x<64;++x){
	xx = (g[i]->location(y,x)).first;
	yy = (g[i]->location(y,x)).second;
	//tgModules[i]->SetPoint(n,xx,yy);
	tgModules[i]->SetPoint(n,yy,xx);
	n++;
      }//end loop strips
    }//end loop rows
    n=0;
    xx=0.0;
    yy=0.0;
  }//end loop modules

  //Do hit points, make their TGraphErrors*
  
  for (int k=0; k<fHitPoints.first.first.size();++k)
    tgHitPoints.first->SetPoint(k,fHitPoints.first.second[k],fHitPoints.first.first[k]);
  for (int k=0; k<fHitPoints.second.first.size();++k)
    tgHitPoints.second->SetPoint(k,fHitPoints.second.second[k],fHitPoints.second.first[k]);
			 
}
void Viewer::setup_planes(){
 
  for (int i=0; i<4; ++i){ //loop over four modules
    
    //tgModules[i] = new TGraphErrors()
    
    
    
  }
}
void Viewer::setup(){
  setup_geometry();
  setup_lines();
  fill_TGs(); 
}

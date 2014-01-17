#include "Viewer.h"


Viewer::Viewer(double gap,std::pair<std::pair<double,double>,std::pair<double,double> > sandy){ //can't pass sandy was reference because its the return of another function
  fGap = gap;
  SandY = sandy;
}


Viewer::~Viewer(){}


void Viewer::setup_geometry(){
  printf("aa\n");
  for (int i=0; i<4; ++i){ //loop over four modules
    printf("i\n");
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
  printf("how am I doing");
  std::stringstream ss;
  /*
  ss << flip_slope(SandY.first.first) << "*x + " << flip_yinter(SandY.first.first,SandY.second.first);

  std::cout << "ss: " << ss.str() << "\n";

  tgRecoLines.first = new TF1("fit_function_XZ", (ss.str()).c_str(),0.0,2.0*74.0+fGap);
  ss.str("");


  ss << flip_slope(SandY.first.second) << "*x + " << flip_yinter(SandY.first.second,SandY.second.second);

  tgRecoLines.second = new TF1("fit_function_YZ", (ss.str()).c_str(),0.0,2.0*74.0+fGap);

  std::cout << "ss: " << ss.str() << "\n";
  */



  ss << SandY.first.first << "*x + " << SandY.second.first;

  std::cout << "ss: " << ss.str() << "\n";

  tgRecoLines.first = new TF1("fit_function_XZ", (ss.str()).c_str(),0.0,2.0*74.0+fGap);
  ss.str("");


  ss << SandY.first.second << "*x + " << SandY.second.second;

  tgRecoLines.second = new TF1("fit_function_YZ", (ss.str()).c_str(),0.0,2.0*74.0+fGap);

  std::cout << "ss: " << ss.str() << "\n";
  
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

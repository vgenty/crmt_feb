#include "Geometry.h"

Geometry::Geometry()
{
  fName = "module";
}

Geometry::~Geometry() {}


void Geometry::dump()
{
  std::cout << "Name: " << fName << std::endl;
  
  for(int i=0;i<4;i++){
    for(int j=0;j<64;++j){
      std::cout << " (" << fLocation[i][j].first << "," << fLocation[i][j].second << ") ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

  }
}


void Geometry::set_coordinates()
{
  //std::ofstream myfile;
   //myfile.open ("geometry.dat");
  
   for(int i=0;i<4;i++){
    for(int j=0;j<64;++j){
      if(i==0 || i== 2){
	fLocation[i][j] = std::make_pair(double(j)*fWidth+fOffset,
					 (double(3-i)*2.0 + 1.0)*fHalfHeight
					 + double(3-i)*fStopGap);
      }
      if(i==1){
	fLocation[i][j] = std::make_pair(double(j)*fWidth+2.0*fOffset,
					 (double(3-i)*2.0 + 1.0)*fHalfHeight
					 + double(3-i)*fStopGap);
      }
      if(i==3){
	fLocation[i][j] = std::make_pair(double(j)*fWidth,
					 double(4-i)*fHalfHeight);
      }
      //myfile << fLocation[i][j].first << "," << fLocation[i][j].second << std::endl;
    }
  }
  
  
   // myfile.close();
  

}

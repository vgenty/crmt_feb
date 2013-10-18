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
  
  for(int i=3;i>=0;i--){
    for(int j=0;j<64;++j){
      if(i==3){
	fLocation[i][j] = std::make_pair(double(j)*fWidth,
					 double(4-i)*fHalfHeight 
					 + double(3-i)*fStopGap);
	std::cout << "making i==3";
      }
      if(i==2 || i == 0){
	fLocation[i][j] = std::make_pair(double(j)*fWidth+fOffset,
					 double(4-i)*2.0*fHalfHeight 
					 + double(3-i)*fStopGap);
	std::cout << "making i==2,0";
      }
      if(i==1){
	fLocation[i][j] = std::make_pair(double(j)*fWidth+2.0*fOffset,
					 double(4-i)*2.0*fHalfHeight 
					 + double(3-i)*fStopGap);
      	std::cout << "making i==1";
      }
    }
  }
  
}

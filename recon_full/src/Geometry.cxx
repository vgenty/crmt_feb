#include "Geometry.h"

Geometry::Geometry(const double &id,const double &gap)
{
  fName = "module";
  
  fId  = id; //matches module id
  if(id <= 1.0) 
    fGap = gap;
  else
    fGap = 0.0;
  
  fBottom = 0.64 + fGap + (3.0 - fId)*(3.0*fStopGap + 4.0*1.54*2.0 );
  set_coordinates();
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
   for(int i=0;i<4;i++){
    for(int j=0;j<64;++j){
      if(i==0 || i== 2){
	fLocation[i][j] = std::make_pair(fWidth/2.0 + double(j)*fWidth+fOffset,
					 (double(3-i)*2.0 + 1.0)*fHalfHeight
					 + double(3-i)*fStopGap + fBottom);
      }
      if(i==1){
	fLocation[i][j] = std::make_pair(fWidth/2.0 + double(j)*fWidth+2.0*fOffset,
					 (double(3-i)*2.0 + 1.0)*fHalfHeight
					 + double(3-i)*fStopGap + fBottom);
      }
      if(i==3){
	fLocation[i][j] = std::make_pair(fWidth/2.0 + double(j)*fWidth,
					 double(4-i)*fHalfHeight + fBottom);
      }
   
    }
   }
   
   /*   
   if(fId == 0.0){
     std::ofstream myfile;
     myfile.open ("geometry.dat");
     for(int i=0;i<4;i++)
       for(int j=0;j<64;++j)
	 myfile << fLocation[i][j].first << "," << fLocation[i][j].second << std::endl;
     myfile.close();  
   }
   */
  

}

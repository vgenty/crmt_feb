#include <cmath>
#include "ParameterSpace.h"
#include "Geometry.h"

ParameterSpace::ParameterSpace()
{
  std::cout << "Initialized Parameter Space with geometry" << std::endl;
  g.set_coordinates();
  
  fMaxSlope = ((g.location(0,63)).first-(g.location(3,0)).first)/((g.location(0,63)).second-(g.location(3,0)).second);
  
  fMinSlope = ((g.location(3,63)).first-(g.location(0,0)).first)/((g.location(3,63)).second-(g.location(0,0)).second);
  
  fMinYinter = -1.0 * fMaxSlope * (g.location(0,63)).second + (g.location(0,63)).first;
  fMaxYinter = -1.0 * fMinSlope * (g.location(0,0)).second + (g.location(0,0)).first;

  
  
  
}

ParameterSpace::~ParameterSpace() {}


void ParameterSpace::Fill_Space()
{
 
  
  for(int ystep = 0; ystep <=fNYinterDivisions; ++ystep){
    for(int sstep = 0; sstep <=fNSlopeDivisions; ++sstep){
      
      double slope = fSlopeStep*sstep+fMinSlope;
      double yinter = fYinterStep*ystep+fMinYinter;
      

      std::pair<double,double> pair = std::make_pair(slope,yinter);

      std::cout << "slope: " << slope << " yinter: " << yinter << std::endl;
      for(int yy = 0; yy < 4; ++yy){
	for(int xx = 0; xx < 64; ++xx){
	  if(Intersection(g.location(yy,xx),slope,yinter)){
	    fPSpace[pair].push_back(std::make_pair(g.location(yy,xx).first,
						   g.location(yy,xx).second));

	    std::cout << "pushed back: ( " << g.location(yy,xx).first << "," << g.location(yy,xx).second << " ) " << std::endl; 
	  }
	}
      }
      std::cout << "sstep: " << sstep << std::endl;
    }
    std::cout << "ytep: " << ystep << std::endl;
  }
  
  
  
  
  
}


bool ParameterSpace::Intersection(std::pair<double,double> params, double Slope, double Yint)
{
  //assume this works, it doesn't FYI
  double FibI = params.first;
  double FibJ = params.second;
  
  double           LeftEdge_h = FibJ + g.get_HalfHeight();
  double          RightEdge_h = FibJ - g.get_HalfHeight();
  
  double   LineLeftLocation_v = Slope*LeftEdge_h  + Yint;
  double  LineRightLocation_v = Slope*RightEdge_h + Yint;
  
  double            TopEdge_v = FibI + g.get_Width()/2.0;
  double         BottomEdge_v = FibI - g.get_Width()/2.0;
  
  double           LineXTop_h = (TopEdge_v - Yint)/Slope;
  double           LineXBot_h = (BottomEdge_v - Yint)/Slope;
    
  if ( (LineLeftLocation_v < TopEdge_v) && (LineLeftLocation_v > BottomEdge_v) )
    return true;
  if ( (LineRightLocation_v < TopEdge_v) && (LineRightLocation_v > BottomEdge_v) )
    return true;
  if ( (LineXTop_h < RightEdge_h) && (LineXTop_h > LeftEdge_h) )
    return true;
  if ( (LineXBot_h < RightEdge_h) && (LineXBot_h > LeftEdge_h) )
    return true;

  return false;
}


void ParameterSpace::TrackOpener(std::vector<Track> tracks)
{
  for (auto track : tracks){
    if(track.is_chosen()){
      for (auto fib : track.fibers()){
	fRecoPoints.push_back(fib.coords());
      }//end loop fibers
    }//end yes chosen
  }//end track
}

void ParameterSpace::set_NSlopeDivisions(int a)
{
  fNSlopeDivisions = a;
  fSlopeStep = fabs(fMinSlope - fMaxSlope)/fNSlopeDivisions;
}
void ParameterSpace::set_NYinterDivisions(int a)
{
  fNYinterDivisions = a;
  fYinterStep = fabs(fMinYinter - fMaxYinter)/fNYinterDivisions;
}




std::map<int,std::vector<double> > ParameterSpace::CreateSpace()
{
  
  //only want to look row by row
  
    for(auto key : fPSpace){
      for(auto value : key){
	
	
	for(auto rpoint : fRecoPoints){
	  if (rpoint.second = value.second){ //only look on y slices
	    //ask fede about this multiple on same y-row
	  }
	  
	}
	
	
	
      }
      
      
    }
  
  
  

}

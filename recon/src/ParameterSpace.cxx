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
   
  for(int ystep = 0; ystep <fNYinterDivisions; ++ystep){
    for(int sstep = 0; sstep <fNSlopeDivisions; ++sstep){
      
      double slope = fSlopeStep*sstep+fMinSlope;
      double yinter = fYinterStep*ystep+fMinYinter;
      

      std::pair<double,double> pair = std::make_pair(slope,yinter);

      //std::cout << "slope: " << slope << " yinter: " << yinter << std::endl;
      for(int yy = 0; yy < 4; ++yy){
	for(int xx = 0; xx < 64; ++xx){
	  if(Intersection(g.location(yy,xx),slope,yinter)){
	    fPSpace[pair].push_back(std::make_pair(g.location(yy,xx).first,
						   g.location(yy,xx).second));

	    //std::cout << "pushed back: ( " << g.location(yy,xx).first << "," << g.location(yy,xx).second << " ) " << std::endl; 
	  }
	}
      }
      //std::cout << "sstep: " << sstep << std::endl;
    }
    //std::cout << "ytep: " << ystep << std::endl;
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




std::map<std::pair<double,double> , double  > ParameterSpace::CreateSpace()
{
  std::map<double,std::map<int,std::vector<double> > > difference;
  double sigma_x = 0.57;
  double sigma_y = 1.54;
  
  for(auto key : fPSpace){
    //std::cout << "key in fPSpace" << std::endl;
    for(auto value : key.second){
      //std::cout <<  "vector in key: value.second = " << value.second << std::endl;
      difference[value.second][0].push_back(value.first);
	for(auto rpoint : fRecoPoints){
	  if(rpoint.second == value.second){
	    //std::cout << "matched: rpoint.second: " << rpoint.second << std::endl;
	    difference[value.second][1].push_back(rpoint.first);
	  }
	}
    }
    
    //std::cout << "have difference.size(): " << difference.size() << std::endl;
    if(difference.size() == 4){//must be a simulated fiber in each row
      /*
      std::cout << std::endl;
      std::cout << " ----- " << std::endl;
      std::cout << std::endl;
      */
      std::vector<double> reco_avg;
      std::vector<double> sim_avg;
      
      for (auto yy : difference){
	//yy.first = double physical y
	//yy.second = map
	//yy.second[n] : n is sim: 0 reco:1 ==vector of x's
	reco_avg.push_back(avg(yy.second[1]));
	sim_avg.push_back(avg(yy.second[0]));
      }
      /*
      std::cout << "sim_avg.size(): " <<  sim_avg.size() << std::endl;
      std::cout << "reco_avg.size(): " <<  reco_avg.size() << std::endl;
      */
      for(size_t v = 0 ; v < reco_avg.size(); ++v){
	fZ[key.first] += pow(sim_avg[v]-reco_avg[v],2)/(pow(sigma_x,2)+pow(sigma_y,2));
      }
      // std::cout << "wrote z-value: " << fZ[key.first] << std::endl;
      reco_avg.clear();
      sim_avg.clear();
      
    }
    difference.clear();
      
  

  }
  /*  
  std::cout << "end of createspace" << std::endl;
  */
  return fZ;

}

double ParameterSpace::avg(std::vector<double> xes)
{
  double average = 0.0;
  int len        = xes.size();
  for (size_t num = 0; num < len; ++num)
    average = average + xes[num]/len;
  
  return average;
}

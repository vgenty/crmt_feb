#include "Track.h"
#include "Fiber.h"

Track::Track()
{
  fid = 0;
}

Track::~Track() {}

void Track::add_fiber(Fiber a)
{
  fFibers.push_back(a);
}

void Track::set_id(int a)
{
  fid = a;
}
bool Track::contains(Fiber a)
{
  for(auto fib : fFibers){
    if( fib.id() == a.id() &&
	fib.x() == a.x() &&
	fib.y() == a.y() ) return true;
  }
  return false;
}

void Track::dump()
{
  std::cout << " track id " << id() << std::endl;
  for(auto fib : fFibers){
    fib.dump();
  }
  std::cout << "fSlope: " << fSlope << " fYinter: " << fYinter << " fChi: " << fChi << " fNdf: " << fNdf  << " reduced: " << fChi/fNdf<< std::endl;
}

void Track::fit()
{
  std::cout << "fitting" << std::endl;
  fTG = new TGraphErrors();
  fFit = new TF1("fit","[0]+[1]*x",-1,70); //Not sure yet what range should be
  int cnt=1;
  for(auto j : fFibers){
    fTG->SetPoint(cnt,j.coords().first,j.coords().second);
    fTG->SetPointError(cnt,0.52,1.54);
    cnt++;
  }
  //try and guess some values for fSlope and fYinter, take first and last point

  
  double x0=fFibers[0].coords().first;
  double x1=fFibers[fFibers.size()-1].coords().first;
  double y0=fFibers[0].coords().second;
  double y1=fFibers[fFibers.size()-1].coords().second;
    
  double slope  = (y1-y0)/(x1-x0);
  double yinter = y1-slope*x1;
  
  fFit->SetParameters(yinter,slope);
  
  fTG->Fit(fFit,"q");
  
  
  //fSlope  = std::make_pair(fFit->GetParameter(1),);
  //fYinter = std::make_pair(fFit->GetParameter(0),);
  fSlope  = fFit->GetParameter(1);
  fYinter = fFit->GetParameter(0);
  fChi    = fFit->GetChisquare();
  fNdf    = fFit->GetNDF();
  
  

  
}
void Track::calculate_angle()
{
  fAngle  = atan(1/fSlope);  
}

void Track::reconstruct()
{
  fit();
  calculate_angle();
}

#include "Track.h"
#include "Fiber.h"
#include "TMath.h"

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
  /*for(auto fib : fFibers){
    fib.dump();
    }*/
  std::cout << "fSlope: " << fSlope << " fYinter: " << fYinter << " fChi: " << fChi << " fNdf: " << fNdf  << " reduced: " << fChi/fNdf <<  " pvalue : " << fPvalue << std::endl;
}

void Track::fit()
{
  fTG = new TGraphErrors();
  fFit = new TF1("fit","[0]+[1]*x",-1,47); //Not sure yet what range should be
  int cnt=1;
  for(auto j : fFibers){
    fTG->SetPoint(cnt,j.coords().second,j.coords().first);
    fTG->SetPointError(cnt,1.54,0.52);
    cnt++;
  }
  //try and guess some values for fSlope and fYinter, take first and last point
    
  double slope  = 0; // just guess zero as the slope...
  double yinter = fFibers[fFibers.size()-1].coords().first; //choose last point

  //  double slope  = (y1-y0)/(x1-x0);
  //  double yinter = y1-slope*x1;
  
  fFit->SetParameters(yinter,slope);
  
  fTG->Fit(fFit,"q");
  
  fSlope  = fFit->GetParameter(1);
  fYinter = fFit->GetParameter(0);
  fChi    = fFit->GetChisquare();
  fNdf    = fFit->GetNDF();
  fPvalue = TMath::Prob(fChi,fNdf);
}
void Track::calculate_angle()
{
  fAngle  = atan(fSlope);  
}

void Track::reconstruct()
{
  fit();
  calculate_angle();
}

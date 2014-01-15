#include "Line.h"
#include "Track.h"

Line::Line(bool isXZ){
  fisXZ = isXZ;
}

void Line::do_tracks(std::vector<Track> tracks_top,std::vector<Track> tracks_bot){
  fTracks = std::make_pair(tracks_top,tracks_bot);
  fit_tracks();
  choose_best();
  //dump();

}


void Line::fit_tracks(){
  int top_cnt ;
  int bot_cnt ;
  
  for(auto& top_track  : fTracks.first) { //top tracks
    fTG = new TGraphErrors(); //this may be slow as balls but okay
    top_cnt =1;
	  
    for(auto& j : top_track.fibers()) {
      fTG->SetPoint     (top_cnt,j.coords().second,j.coords().first);
      fTG->SetPointError(top_cnt,1.54,             0.52);
      top_cnt++;
    }
    
    for(auto& bot_track : fTracks.second) { //bottom tracks
      TF1 *ffit = new TF1("fit","[0]+[1]*x",-1,74); //Not sure yet what range should be	
      bot_cnt = top_cnt + 1;
      
      for(auto& j : bot_track.fibers()){
	fTG->SetPoint     (bot_cnt,j.coords().second,j.coords().first);
	fTG->SetPointError(bot_cnt,1.54,             0.52);
	bot_cnt++;
      }
      //try and guess some values for fSlope and fYinter, take first and last point
      double slope  = 0; // just guess zero as the slope, there is probably a cleaner way
      //double yinter = fFibers[fFibers.size()-1].coords().first; //choose last point
      double yinter = bot_track.fibers()[0].coords().first; //choose first point
      //  double slope  = (y1-y0)/(x1-x0);
      //  double yinter = y1-slope*x1;

      ffit -> SetParameters(yinter,slope);  
      fTG  -> Fit(ffit,"q");
      
      fFits.push_back(ffit);

      //remove bottom points before new fill
      for(int k=bot_cnt;k <= top_cnt+1;--k)
	fTG->RemovePoint(k);
      
    }
  }

  //fFits vector should be filled with fits
}

void Line::choose_best(){
  //trying to use a lambda function to sort to line with highest Pvalue
  //  std::sort(fFits.begin(), fFits.end(),
  //[](TF1 const & a, TF1 const &b){return a.GetChisquare() < b.GetChisquare();});
  //ok it didn't work
  
  double _redchi = 100000.;
  double _local_redchi;
  for(auto line : fFits){
    _local_redchi = (line->GetChisquare())/(line->GetNDF()); 
    //std::cout << "_local_redchi: " << _local_redchi << "\n";
    if (_local_redchi < _redchi){
      _redchi = _local_redchi;
      fBestLine = *line;
    }
  }
  
  fSlope  = fBestLine.GetParameter(1);
  fYinter = fBestLine.GetParameter(0);
  fChi    = fBestLine.GetChisquare();
  fNdf    = fBestLine.GetNDF();
  fPvalue = TMath::Prob(fChi,fNdf);
  

}

void Line::clear_lines(){  
  //TGraphErrors *fTG;

  //  fTracks.clear();
  //make sure to clean up fBestLine of you are fugged
  
  fFits.clear();
  
  //WARNING YOU NEED TO CLEAR THE FITS I AM NOT JOKING
  
  fSlope    = 0;
  fYinter   = 0;
  fAngle    = 0;
  fCosAngle = 0;
  fChi      = 0;
  fNdf      = 0;
  fPvalue   = 0;
}

void Line::dump(){
  std::cout << "fSlope: " << fSlope << " fYinter: " << fYinter << " fChi: " << fChi << " fNdf: " << fNdf  << " reduced: " << fChi/fNdf <<  " pvalue : " << fPvalue << std::endl;
}

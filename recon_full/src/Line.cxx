#include "Line.h"

Line::Line(bool isXZ){
  fisXZ = isXZ;
}

Line::~Line(){}

void Line::do_track_reco(std::vector<Track> tracks_top,std::vector<Track> tracks_bot,double& gap){
  
  fGap = gap;
  fTracks = std::make_pair(tracks_top,tracks_bot);
  fit_tracks();
  choose_best();

}


void Line::fit_tracks(){
  int top_cnt ;
  int bot_cnt ;
  
  for(auto& top_track  : fTracks.first) { 
    fTG = new TGraphErrors(); 
    top_cnt =1;
	  
    for(auto& j : top_track.fibers()) {
      fTG->SetPoint     (top_cnt,j.coords().second,j.coords().first);
      fTG->SetPointError(top_cnt,1.54,             0.52);
      top_cnt++;
    }
    
    for(auto& bot_track : fTracks.second) { 
      TF1 *ffit = new TF1("fit","[0]+[1]*x",-1.0,2.0*74.0+fGap);
      bot_cnt = top_cnt + 1;
      
      for(auto& j : bot_track.fibers()){
	fTG->SetPoint     (bot_cnt,j.coords().second,j.coords().first);
	fTG->SetPointError(bot_cnt,1.54,             0.52);
	bot_cnt++;
      }

      double slope  = 0; 
      double yinter = bot_track.fibers()[0].coords().first; 
      
      ffit -> SetParameters(yinter,slope);  
      fTG  -> Fit(ffit,"q");
      
      fFits.push_back(ffit);

      //remove bottom points before new fill
      for(int k=bot_cnt;k <= top_cnt+1;--k)
	fTG->RemovePoint(k);
      
    }
  }

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
    if (_local_redchi < _redchi){
      _redchi = _local_redchi;
      fBestLine = *line;
    }
  }
  
  fSlope    = fBestLine.GetParameter(1);
  fYinter   = fBestLine.GetParameter(0);
  fChi      = fBestLine.GetChisquare();
  fNdf      = fBestLine.GetNDF();
  fPvalue   = TMath::Prob(fChi,fNdf);
  fAngle    = atan(fSlope);  
  fCosAngle = cos(fAngle);
  
}

void Line::clear_lines(){  
  fFits.clear();
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

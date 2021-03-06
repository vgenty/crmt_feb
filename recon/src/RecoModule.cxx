#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "RecoModule.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"

RecoModule::RecoModule() {
  fGoodTrackIndex = -1;
  fAngleThreshold = 1.4;
}

RecoModule::~RecoModule() {}

void RecoModule::getfiles(std::string efile, std::string pfile)
{
  fEventFile = efile;
  fPinFile   = pfile;
}

void RecoModule::initpixels()
{
  for(int pix=0;pix<16;++pix)
    for(int fib=0;fib<8;++fib)
      fPixelsToFibers[pix][fib]=(fib+1)+pix*8;
}

void RecoModule::initpins()
{
  std::ifstream pins;
  pins.open(fPinFile.c_str());
  bool found;
  int out = 0;
  std::string line;

  if ( pins.is_open() ) {
    while( !pins.eof() ){
      std::getline(pins,line);
      std::string::iterator it;
      std::string           str_number;
      found = false;
      for ( it = line.begin();it<=line.end(); ++it) {
        if (*it != ',')
          str_number+=*it;
        else
          found = true;

        if(found){
          fPinsToPixels[out][0] = atoi(str_number.c_str());
          str_number="";
          found = false;
        }

        if (it == line.end() ){
          fPinsToPixels[out][1]=atoi(str_number.c_str());
          out++;
        }

      }//end for                                                                                            
    }
  }

  pins.close();
}


void RecoModule::initfibs()
{
  int seeds[3][8] = {{9  ,33,73,97,  1,41,65,105},
		     {104,80,40,16,112,72,48,  8},
		     {120,96,56,32,128,88,64,24 }};
  
  //Fill identifier row                                                                                     
  for(int kk=0; kk<8;++kk) {
    fFiberLocations[0][kk+0*8] = 25  + kk;
    fFiberLocations[0][kk+1*8] = 49  + kk;
    fFiberLocations[0][kk+2*8] = 89  + kk;
    fFiberLocations[0][kk+3*8] = 113 + kk;
    fFiberLocations[0][kk+4*8] = 17  + kk;
    fFiberLocations[0][kk+5*8] = 57  + kk;
    fFiberLocations[0][kk+6*8] = 81  + kk;
    fFiberLocations[0][kk+7*8] = 121 + kk;
  }
  for (int o=0;o<3;++o)
    for( int e=0;e<8;++e)
      for (int i=0;i<8;++i)
        if(o+1==1)
          fFiberLocations[o+1][8*e+i]=seeds[o][i]+e;
        else
          fFiberLocations[o+1][8*e+i]=seeds[o][i]-e;
}

void RecoModule::initfile()
{
  //  std::cout << "in initfile()" << std::endl;
  
  //Local vars
  static const int LARGE_NUMBER=128;
  int    _nsignals;
  int    _nchannels_per_event[100];
  int    _packet_number[100];
  bool   _type[100];
  int    _event_number[100];
  int    _board_address[100];
  double _time[100];
  int    _channels[100][LARGE_NUMBER];
  
  int evt = 1;
  TFile *input = new TFile(fEventFile.c_str(),"READ"); 
  TTree *data_tree = (TTree*)(input->Get("data_tree"));
  
  //  std::cout << "opened file" << std::endl;
  
  if( !data_tree ){             
    std::cout << " problem: cannot read the tree; quitting " << std::endl;
    exit(0);      
  }
  
  data_tree->SetBranchAddress("nsignals",&_nsignals);
  data_tree->SetBranchAddress("nchannels_per_event",_nchannels_per_event);    
  data_tree->SetBranchAddress("event_number",_event_number);
  data_tree->SetBranchAddress("packet_number",_packet_number);      
  data_tree->SetBranchAddress("type",_type);
  data_tree->SetBranchAddress("board_address",_board_address);
  data_tree->SetBranchAddress("time",_time);
  data_tree->SetBranchAddress("channels",_channels);
  TBranch *b_nsignals = data_tree->GetBranch("nsignals");
  TBranch *b_nchannels_per_event = data_tree->GetBranch("nchannels_per_event");
  TBranch *b_event_number = data_tree->GetBranch("event_number");
  TBranch *b_packet_number = data_tree->GetBranch("packet_number");
  TBranch *b_type = data_tree->GetBranch("type");
  TBranch *b_board_address = data_tree->GetBranch("board_address");
  TBranch *b_time = data_tree->GetBranch("time");
  TBranch *b_channels = data_tree->GetBranch("channels");    

  //  std::cout << "Branches set" << std::endl;

  for ( int ient = 0; ient < data_tree-> GetEntries(); ++ient ) {
    
    //std::cout << "looking at event: " << ient << std::endl;
    
    b_nsignals->GetEvent(ient);
    b_nchannels_per_event->GetEvent(ient);
    b_event_number->GetEvent(ient);
    b_packet_number->GetEvent(ient);        
    b_type->GetEvent(ient);
    b_board_address->GetEvent(ient);
    b_time->GetEvent(ient);    
    b_channels->GetEvent(ient); 
    //std::cout << "got event" << std::endl;
    
    //output_list << _event_number[0]; //event number, many times this is -666
    //std::cout << "found _nsignals: " << _nsignals << std::endl;
    for(size_t is=0; is<_nsignals; is++){
      for(size_t ich=0; ich<_nchannels_per_event[is]; ich++){
	//std::cout << "_channels[is][ich]:" << _channels[is][ich]  << std::endl;
        fEventData[evt].push_back(_channels[is][ich]);
	//output_list << "," << _channels[is][ich];
      }   
    }
    //output_list << "\n";
    // std::cout << "evt: " << evt << std::endl;
    evt++;
  }


  //Goal is to fill fEventData[evt].push_back(atoi(str_number.c_str()));

  /*
  //Used to read text file
  std::ifstream events;
  events.open(fEventFile.c_str());
  
  int evt=1;
  bool found;
  bool channel;
  std::string line;
  
  if ( events.is_open() ) {
    while( !events.eof() ){
      
      std::getline(events,line);
      std::string::iterator it;
      std::string           str_number;
      
      found    = false;
      channel  = false;
      for ( it = line.begin();it<=line.end(); ++it) {
        if (found) {
          if (*it == ',' || it == line.end()) {
            channel=true;
            fEventData[evt].push_back(atoi(str_number.c_str()));
            str_number="";
          }
          if (!channel)
            str_number += *it;
          channel=false;
        }
        if (*it == ',')
          found = true;
      }
      evt++;
    }
  }else{
    std::cout << "Bad file descriptor" << std::endl;
  }
  */
}

bool RecoModule::check_event(std::vector<int>& pin_data)
{						
  bool check      =  false;
  int goodpins[8] =  {22,19,20,23,17,18,16,21};
  if(pin_data.size()>=4){      //Must have at least 4 pins hit  
    for(auto pin : pin_data){ //Loop over pins              
      for(int j=0;j<8;++j){    //check against goodpins (which are identifier rows)
	if(pin == goodpins[j] && 
	   ( (pin >=8  && pin<=23)
	     || (pin<=23 && pin <=55) ) ){ //kill events with noisy channels
	  check=true;
	}//end if good pin
      }//end looping good pins
    }//end pin loop
  }//end >4
  
  return check;
}

void RecoModule::find_hit_fibers(std::vector<int>& hit_pins){
  int pixel;
  int pin;
  std::sort(hit_pins.begin(),hit_pins.end());
  
  for(int j=0;j<32;++j){
    if(binary_search(hit_pins.begin(), hit_pins.end(), fPinsToPixels[j][0])){
      pin   = fPinsToPixels[j][0];
      pixel = fPinsToPixels[j][1];

      if(pin<32) //pins less than 32 are the top PMT
        for(int k=0;k<8;++k){
          fHitFibers[0].push_back(fPixelsToFibers[pixel-1][k]);
	  fFiberPinPixel[0][fPixelsToFibers[pixel-1][k]] = std::make_pair(pin,pixel);
	}
      else //pins greater than this are on the bottom
        for(int k=0;k<8;++k){
          fHitFibers[1].push_back(fPixelsToFibers[pixel-1][k]);
	  fFiberPinPixel[1][fPixelsToFibers[pixel-1][k]] = std::make_pair(pin,pixel);
	}
    }
  }
}

void RecoModule::init_module()
{
 
  std::map<int, std::vector<int> >::iterator itr;
   
  getfiles(fEventFile.c_str(),fPinFile.c_str());
  initfibs();
  initpins();
  initpixels();
  initfile();
  g.set_coordinates();

  itr=fEventData.begin();
  while (itr != fEventData.end()) {
    if(!check_event((*itr).second))
      fEventData.erase(itr++); 
    else
      itr++;
  }  
  //Eventwise loop
  
  //  fNEvents = fEventData.size();
  
}
  //Fill fHitFibers key 0 or 1; 0==top, 1==bottom (top/bot pmt)

void RecoModule::get_location(int id, double *x, double *y, bool top){
  int kkfirst, kklast;

  if( top ){
    kkfirst=0;
    kklast=1;
  }else{
    kkfirst=2;
    kklast=3;
  }
  for(int kk=kkfirst; kk<=kklast;++kk) {
    for(int jj=0; jj<64;++jj) {
      if( fFiberLocations[kk][jj] == id ){
	*x=jj;
	*y=kk;
	return;
      }
    }
  } 
  std::cout << " problem: could not locate fiber " << id << " in top? " << top << std::endl;
  return;
}

void RecoModule::fill_fibers(){
  
  double x, y;
  int pin, pixel;
  bool top = true;
  
   for (auto tb: fHitFibers){
    if( tb.first == 1 ) top = false;
    Fiber f;
    for(int tt=0;tt<(tb.second).size();++tt){
      f.set_id(tb.second.at(tt));

      get_location(f.id(),&x,&y,top);
      f.set_x(x);
      f.set_y(y);
      
      f.set_coords(g.location(y,x));
      
      get_pixel(f.id(),&pixel,top);
      get_pin(f.id(),&pin,top);
      f.set_pin(pin);
      f.set_pixel(pixel);
      
      fFibers.push_back(f);
    }
   }
   
}
void RecoModule::get_pin(int id, int *pin, bool top){ 
  if (top)
    *pin = fFiberPinPixel[0][id].first; 
  else
    *pin = fFiberPinPixel[1][id].first; 
}
void RecoModule::get_pixel(int id, int *pixel, bool top){
  if (top)
    *pixel = fFiberPinPixel[0][id].second; 
  else
    *pixel = fFiberPinPixel[1][id].second; 
}
void RecoModule::print_fibers()
{
  for(auto fib : fFibers){
    fib.dump();
  }
  
}    
void RecoModule::clusterize()
{ 
  bool contains  = false;
  bool contains2 = false;
  
  for(auto fib : fFibers){
    
    if( fib.y() == 0 )
      continue;
    
    contains = false;
    
    for(auto track : fTracks){
      if( track.contains(fib) ){
	contains = true;
	break;
      }
    }
    
    if( contains ) 
      continue;
    
    
    Track newtrack;
    newtrack.add_fiber(fib);
    
    for(size_t index=0; index<newtrack.fibers().size(); index++){
      
      Fiber local_fib=newtrack.fibers()[index];
      
      for(auto fib2 : fFibers){
	
	if( fib2.y() == 0 )
	  continue;
	
	contains2 = newtrack.contains(fib2);
	if( contains2 )
	  continue;
	
	
	if( !fib2.near(local_fib) )
	  continue;
	
	newtrack.add_fiber(fib2);
	
      }
    }
    
    newtrack.set_id(fTracks.size());
    fTracks.push_back(newtrack);
    
  }
}

void RecoModule::attach()
{
  bool found;
  //This is such a BS way to delete track pls fix
  std::vector<Track>::iterator track  = fTracks.begin();
  std::vector<Track>::iterator lasttrack  = fTracks.end();
  size_t counter=0;
  while(track != fTracks.begin() + fTracks.size()){
    if( track - fTracks.begin() == fTracks.size()) 
      break;
    found = false;
    for(auto fibs : track->fibers()){
      for (auto fib : fFibers){
	if (fib.y() == 0){
	  if (fib.near(fibs) && !track->contains(fib)) {
	    track->add_fiber(fib);
	    found = true;
	  }
	}
      }
    }
    if(!found){
      fTracks.erase(track);
    }else{
      counter ++;
      track  = fTracks.begin() +  counter;
    }
    
  }
}
void RecoModule::print_tracks()
{
  for(auto tr : fTracks){
    tr.dump();
  }
}
/*
void RecoModule::fill_root()
{
  fm.make_tree("recodata.root",fLocalAngles.size(),fLocalAngles);
}
*/

void RecoModule::choose_angles()
{
  std::vector<Track>::iterator track;

  for (track = fTracks.begin();track != fTracks.end();++track)
    track->reconstruct();

  int index=0;
  int good_index=-1;
  double value=10000;
  for(auto track : fTracks){ //doing THIS MEANS YOU CAN NOT MODIFY fTRACKS
    if(track.pvalue() < value) {
      value      = track.pvalue();
      good_index = index;
    }
    index++;
  }		
  
  fGoodTrackIndex = good_index;
  if(is_good_track() && one_each_layer(fTracks.at(good_index))){//need something here
    fTracks.at(good_index).chosen(true);
    fTracks.at(good_index).hasoneeach(true);
  }
}


// below this line okay my failure is showing
// require good track index AND an angle less than fAngleTreshold <1.4????

bool RecoModule::is_good_track(){
  if (fGoodTrackIndex >=0)
    return true;
  else
    return false;
}
bool RecoModule::is_good_layers(){
  if (is_good_track()){
    if(fTracks.at(fGoodTrackIndex).is_hasoneeach()){
      return true;
    }
    else
      return false;
  }
  else
    return false;

}
bool RecoModule::is_good_angle(){
  if (is_good_track()){
    if(fabs(fTracks.at(fGoodTrackIndex).angle()) <fAngleThreshold ){
      return true;
    }
    else
      return false;
  }
  else
    return false;
  
}
bool RecoModule::conditions_are_met(){
  if(is_good_angle() && is_good_track() && is_good_layers())
    return true;
  else
    return false;
}

double RecoModule::get_Slope()
{
  return fTracks.at(fGoodTrackIndex).slope();
}
double RecoModule::get_YInter()
{
  return fTracks.at(fGoodTrackIndex).yinter();
}
double RecoModule::get_Chi()
{
  return fTracks.at(fGoodTrackIndex).chi();
}
double RecoModule::get_Ndf()
{
  return fTracks.at(fGoodTrackIndex).ndf();
}
double RecoModule::get_Pvalue()
{
  return fTracks.at(fGoodTrackIndex).pvalue();
}
double RecoModule::get_Angle()
{
  return fTracks.at(fGoodTrackIndex).angle();
}
double RecoModule::get_CosAngle()
{
  return fTracks.at(fGoodTrackIndex).cosangle();
}

bool RecoModule::one_each_layer(Track trakk) 
{
  bool one_each[4] = {false,false,false};

  for (auto fiber : trakk.fibers())
    for(size_t layer=0;layer<4;++layer)
      if(fiber.y()==layer)
	one_each[layer] = true;
  
  bool good = false;
  for(size_t layer=0;layer<4;++layer){
    if(one_each[layer] == true) {
      good = true;
    }else{
      good = false;
      break;
    }
  }
  
  return good;
}


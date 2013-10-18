#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "RecoModule.h"

RecoModule::RecoModule() {}

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

void RecoModule::initpins(int pins_to_pixels[32][2])
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
          pins_to_pixels[out][0] = atoi(str_number.c_str());
          str_number="";
          found = false;
        }

        if (it == line.end() ){
          pins_to_pixels[out][1]=atoi(str_number.c_str());
          out++;
        }

      }//end for                                                                                            
    }
  }

  pins.close();
}


void RecoModule::initfibs(int fiber_locations[4][64])
{
  int seeds[3][8] = {{9  ,33,73,97,  1,41,65,105},
		     {104,80,40,16,112,72,48,  8},
		     {120,96,56,32,128,88,64,24 }};
  
  //Fill identifier row                                                                                     
  for(int kk=0; kk<8;++kk) {
    fiber_locations[0][kk+0*8] = 25  + kk;
    fiber_locations[0][kk+1*8] = 49  + kk;
    fiber_locations[0][kk+2*8] = 89  + kk;
    fiber_locations[0][kk+3*8] = 113 + kk;
    fiber_locations[0][kk+4*8] = 17  + kk;
    fiber_locations[0][kk+5*8] = 57  + kk;
    fiber_locations[0][kk+6*8] = 81  + kk;
    fiber_locations[0][kk+7*8] = 121 + kk;
  }
  for (int o=0;o<3;++o)
    for( int e=0;e<8;++e)
      for (int i=0;i<8;++i)
        if(o+1==1)
          fiber_locations[o+1][8*e+i]=seeds[o][i]+e;
        else
          fiber_locations[o+1][8*e+i]=seeds[o][i]-e;
}

void RecoModule::initfile(std::map<int,std::vector<int> >& event_data)
{
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
            event_data[evt].push_back(atoi(str_number.c_str()));
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
}

bool RecoModule::check_event(std::vector<int>& pin_data)
{
  bool check      =  false;
  int goodpins[8] =  {16,19,20,23,17,18,9,4};
  if(pin_data.size()>=4)      //Must have at least 4 pins hit  
    for(auto pin : pin_data) //Loop over pins                                                              
      for(int j=0;j<8;++j)    //check against goodpins (which are identifier rows)
	if(pin == goodpins[j])
	  check=true;
  
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

      if(pin<32)
        for(int k=0;k<8;++k)
          fHitFibers[0].push_back(fPixelsToFibers[pixel-1][k]);
      else
        for(int k=0;k<8;++k)
          fHitFibers[1].push_back(fPixelsToFibers[pixel-1][k]);
    }
  }
}

void RecoModule::init_module()
{
 
  std::map<int, std::vector<int> >::iterator itr;
   
  getfiles(fEventFile.c_str(),fPinFile.c_str());
  initfibs(fFiberLocations);
  initpins(fPinsToPixels);
  initpixels();
  initfile(fEventData);
  //set up coordinates
  g.set_coordinates();
  //Clear events with not enough pins
  itr=fEventData.begin();
  while (itr != fEventData.end()) {
    if(!check_event((*itr).second))
      fEventData.erase(itr++); 
    else
      itr++;
  }  
  //Eventwise loop
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
      fFibers.push_back(f);
      //  f.dump();
    }
   }
   
   //give them real xyz
   
   
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
	  if (fib.near(fibs)) {
	    track->add_fiber(fib);
	    found = true;
	  }
	}
      }
    }
    if(!found){
      //track->dump();
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
void RecoModule::ini{

}
*/

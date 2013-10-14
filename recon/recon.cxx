#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>


bool check_event(std::vector<int>& pin_data) {
  bool check      =  false;
  int goodpins[8] =  {16,19,20,23,17,18,9,4};
 
  if(pin_data.size()>=3)     //Must have at least 3 pins hit
     for(auto pin : pin_data) //Loop over pins
      for(int j=0;j<8;++j)   //check against goodpins (which are identifier rows)
	if(pin == goodpins[j])
	  check=true;
 
  return check;
}

void initpins(int pins_to_pixels[32][2]) {
  std::ifstream pins;
  pins.open("pins.csv");      
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

void initfibs(int fiber_locations[4][64]) {
  int seeds[3][8] = {{9  ,33,73,97,  1,41,65,105},
		     {104,80,40,16,112,72,48,  8},
		     {120,96,56,32,128,88,64,224 }};
    
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

void initfile(std::map<int,std::vector<int> >& event_data) {
  std::ifstream events;
  events.open("event_list.txt");    
 
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

int main(int argc,const char *argv[]) {
  std::cout << "Initializing Based God Event Reconstruction" << std::endl;
  
  //Get fiber_locations in detector
  int fiber_locations[4][64];
  initfibs(fiber_locations);
  //Get pixels_to_fibers
  /*
    for(int kk=0; kk<4;++kk) {
    for(int jj=0; jj<64;++jj) {
    std::cout << fiber_locations[kk][jj] << " ";
    }
    std::cout << std::endl;
  } 
  */

  //Get pins_to_pixels
  int pins_to_pixels[32][2];
  initpins(pins_to_pixels);
  /*
  for(int p=0;p<32;++p){
    std::cout << pins_to_pixels[p][0] << "," << pins_to_pixels[p][1] << std::endl;
    }*/

  //Get event data
  std::map<int, std::vector<int> > event_data;
  std::map<int, std::vector<int> >::iterator itr;
  initfile(event_data); // Initialize event_data<event_number,vec<hitpins>  
 
   
 
  //Cut out events with no identifier row
  //  for (itr = event_data.begin(); itr != event_data.end(); ++itr)
  itr=event_data.begin();
  while (itr != event_data.end()) {
    if(!check_event((*itr).second))
      event_data.erase(itr++); 
    else
      itr++;
  }  

  for(itr=event_data.begin(); itr!= event_data.end(); ++itr) {
    std::cout << (*itr).first;
    std::vector<int> channel = (*itr).second;
    for ( int g = 0; g < channel.size(); ++g) {
      std::cout << " " <<channel[g] << " ";
    }
    std::cout << std::endl;
    
  }
   
  
  

  return 0;
  
}

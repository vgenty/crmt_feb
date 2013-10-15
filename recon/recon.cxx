#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "recoinit.h"
#include <algorithm>

bool check_event(std::vector<int>& pin_data) {
  bool check      =  false;
  int goodpins[8] =  {16,19,20,23,17,18,9,4};
  if(pin_data.size()>=3)      //Must have at least 3 pins hit
     for(auto pin : pin_data) //Loop over pins
      for(int j=0;j<8;++j)    //check against goodpins (which are identifier rows)
	if(pin == goodpins[j])
	  check=true;
 
 return check;
}

void find_hit_fibers(std::vector<int>& hit_pins,
		     std::map<int, std::vector<int> >& hit_fibers,
		     int pins_to_pixels[32][2], 
		     int pixels_to_fibers[16][8]){
  int pixel;
  int pin;
  std::sort(hit_pins.begin(),hit_pins.end());
  
  for(int j=0;j<32;++j){
    if(binary_search(hit_pins.begin(), hit_pins.end(), pins_to_pixels[j][0])){
      pin   = pins_to_pixels[j][0];
      pixel = pins_to_pixels[j][1];
     
      if(pin<32)
	for(int k=0;k<8;++k)	
	  hit_fibers[0].push_back(pixels_to_fibers[pixel][k]);
      else
	for(int k=0;k<8;++k)	
	  hit_fibers[1].push_back(pixels_to_fibers[pixel][k]);
    }
  }  
  
  

}
		     
		     
		  
int main(int argc,const char *argv[]) {
  std::cout << "Initializing Based God Event Reconstruction" << std::endl;
  int fiber_locations[4][64];
  int pins_to_pixels[32][2];
  int pixels_to_fibers[16][8];
  std::map<int, std::vector<int> > event_data;
  std::map<int, std::vector<int> >::iterator itr;
  std::map<int, std::vector<int> > hit_fibers;
  std::map<int, std::vector<int> >::iterator it;
  
  //Initialize data
  initfibs(fiber_locations);
  initpins(pins_to_pixels);
  initpixels(pixels_to_fibers);
  initfile(event_data); 
  
  //Clear events with not enough pins
  itr=event_data.begin();
  while (itr != event_data.end()) {
    if(!check_event((*itr).second))
      event_data.erase(itr++); 
    else
      itr++;
  }  
  //Eventwise loop
  int evt=1;
  for(itr=event_data.begin();itr!=event_data.end();++itr){
    std::cout << "-------------------" << std::endl;
    std::cout << "Event: " << evt << std::endl;

    
    find_hit_fibers((*itr).second,hit_fibers,pins_to_pixels,pixels_to_fibers);
    for (int oo=0;oo<((*itr).second).size();oo++)
      std::cout << "have: " << ((*itr).second).at(oo) << " ";

    std::cout << std::endl;
    for(auto tb: hit_fibers){
      std::cout << "toporbot: " << tb.first << " size: " << (tb.second).size() << std::endl;
      
      std::cout << std::endl;
      std::cout << "fiber numbers: " << std::endl;
      for(int tt=0;tt<(tb.second).size();++tt)
	std::cout << (tb.second).at(tt) << " ";
      std::cout << std::endl;
    }
    
    hit_fibers.clear();
    evt++;
  }

  //Fill hit_fibers key 0 or 1; 0==top, 1==bottom (top/bot pmt)


  

  return 0;
  
}

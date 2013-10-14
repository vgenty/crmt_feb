#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>


void initfibs(int fiber_locations[4][64]) {
  int seeds[3][8] = {{9  ,33,73,97,  1,41,65,105},
		     {104,80,40,16,112,72,48,8  },
		     {120,96,56,32,128,88,64,24}};
  
  
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
      
      std::string str_number;
      found = false; 
      channel  = false;
      for ( it = line.begin();it<=line.end(); ++it) {
	if (found) {
	  if (*it == ',' || it == line.end()) {
	    channel=true;
	    event_data[evt].push_back(atoi(str_number.c_str()));
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

  int fiber_locations[4][64];
  initfibs(fiber_locations);


  for(int kk=0; kk<4;++kk) {
    for(int jj=0; jj<64;++jj) {
      std::cout << fiber_locations[kk][jj] << " ";
   }
    std::cout << std::endl;
  }  
  std::cout << "Initializing Based God Event Reconstruction" << std::endl;
  std::map<int, std::vector<int> > event_data;
  std::map<int, std::vector<int> >::iterator itr;
  initfile(event_data); // Initialize event_data<event_number,vec<hitpins>
  
 
  /*  
 
   for(itr=event_data.begin(); itr!= event_data.end(); itr++) {
    std::cout << (*itr).first;
    std::vector<int> channel = (*itr).second;
    for ( int g = 0; g < channel.size(); ++g) {
      std::cout << " " <<channel[g] << " ";
    }
    std::cout << std::endl;
    
  }
  */ 
  
    
  
  

  return 0;
  
}

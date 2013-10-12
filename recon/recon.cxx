#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

void initfile(std::map<int,std::vector<int> >& event_data) {
  std::ifstream events;
  events.open("event_list.txt");    
  
  
  int evt=1;
  std::string line;
  
  
  if ( events.is_open() ) {
    while( !events.eof() ){
      std::getline(events,line);
      std::string::iterator it;
  
      std::string str_number;
      bool found     = false;
      bool channel   = false;
      for ( it = line.begin();it<=line.end(); ++it) {
	if (found) {
	  if (*it == ',' || it == line.end()) {
	    channel=true;
	    event_data[evt].push_back(atoi(str_number.c_str()));
	    //	    std::cout << str_number << " ";
	    //str_number = "";
	  }
	  if (!channel)  
	    str_number += *it;
	  channel=false;
	}

	if (*it == ',')
	  found = true;
      }

      evt++;
      //std::cout << std::endl;
    }
  }else{
    std::cout << "Bad file descriptor" << std::endl;
  }
  
  
  
  
}

int main(int argc,const char *argv[]) {
  
  std::cout << "Initializing Based God Event Reconstruction" << std::endl;
  
  std::map<int, std::vector<int> > event_data;
  initfile(event_data);
  
  std::map<int, std::vector<int> >::iterator itr;
    
  for(itr=event_data.begin(); itr!= event_data.end(); itr++) {
    
    std::cout << (*itr).first;
    std::vector<int> channel = (*itr).second;
    for ( int g = 0; g < channel.size(); ++g) {
      std::cout << " " <<channel[g] << " ";
    }
    std::cout << std::endl;
    
  }
    
  
    
  
  

  return 0;

}

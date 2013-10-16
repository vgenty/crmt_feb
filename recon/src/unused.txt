  // Initialize event_data<event_number,vec<hitpins>  
  /*
    for(int kk=0; kk<4;++kk) {
    for(int jj=0; jj<64;++jj) {
    std::cout << fiber_locations[kk][jj] << " ";
    }
    std::cout << std::endl;
  } 
  */

  //Get pins_to_pixels

  
  /*
  for(int p=0;p<32;++p){
    std::cout << pins_to_pixels[p][0] << "," << pins_to_pixels[p][1] << std::endl;
    }*/

  //Get pixels_to_fibers
  

  /*  
    for(int kk=0; kk<16;++kk) {
    for(int jj=0; jj<8;++jj) {
    std::cout << pixels_to_fibers[kk][jj] << " ";
    }
    std::cout << std::endl;
  } 
  */

  //Get event data

 
   
 
  //Cut out events with no identifier row
  //  for (itr = event_data.begin(); itr != event_data.end(); ++itr)


/*
  for(itr=event_data.begin(); itr!= event_data.end(); ++itr) {
    std::cout << (*itr).first;
    std::vector<int> channel = (*itr).second;
    for ( int g = 0; g < channel.size(); ++g) {
      std::cout << " " <<channel[g] << " ";
    }
    std::cout << std::endl;
    
  }
  */
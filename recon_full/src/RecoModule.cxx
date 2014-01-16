#include "RecoModule.h"

RecoModule::RecoModule() {
  fGoodTrackIndex = -1;
}

RecoModule::RecoModule(int id, double gap) {
  fGoodTrackIndex = -1;

  fId = id;
  if(id <= 1)
    fGap = gap;
  else
    fGap = 0;

  getfiles("./input/pins.csv");
  init_module();

}

RecoModule::~RecoModule() {}

void RecoModule::getfiles(std::string pfile)
{
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
		     {120,96,56,32,128,88,64, 24}};
  
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

bool RecoModule::check_event(std::vector<int>& pin_data)
{
  bool check      =  false;
  if(pin_data.size()>=3){      //Must have at least 4 pins hit  
    for(auto pin : pin_data){  //Loop over pins              
      for(int j=0;j<8;++j){    //check against goodpins (which are identifier row pins
	if(pin == goodpins[j]){
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

      if(pin<16) //pins less than 16 are the top PMT. Yes it's true the FEB is rotated.
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
 
  getfiles(fPinFile.c_str());
  initfibs();
  initpins();
  initpixels();
  g = new Geometry(fId,fGap);
  
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
      
      f.set_coords(g->location(y,x));
      
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
  //std::vector<Track>::iterator lasttrack  = fTracks.end();
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
  std::cout << "!!!!!!!!!Module id: " << fId << " status!!!!!!!!!\n";
  for(auto tr : fTracks){
    tr.dump();
  }
  
}

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

bool RecoModule::conditions_are_met(){
  if(is_good_track() && is_good_layers())
    return true;
  else
    return false;
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


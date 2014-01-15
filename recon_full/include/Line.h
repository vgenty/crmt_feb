#ifndef LINE_H 
#define LINE_H


#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "TGraphErrors.h"
#include "TF1.h"
#include "math.h"
#include "Track.h"


class Line {
  
private:
  
  TGraphErrors *fTG;
  TF1 *fFit;
  
  double fSlope;
  double fYinter;
  double fAngle;
  double fCosAngle;
  double fChi;
  double fNdf;
  double fPvalue;
  bool   fChosen;
  
public:
  Line(const Track &top,const Track &bot);
  ~Line();
  
  double slope(){return fSlope;}
  double yinter(){return fYinter;}
  double angle(){return fAngle;}
  double chi(){return fChi;}
  double ndf(){return fNdf;}
  double pvalue(){return fPvalue;}
  double cosangle(){return fCosAngle;}

};

#endif

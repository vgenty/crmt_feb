#include <iostream>
#include <cmath>
#include "TF1.h"
#include "TH1D.h"
#include "TApplication.h"
#include "TCanvas.h"

double my_factorial(int x)
{
  int return_me = 1;
  for (int i = 1; i <= x; i++) {
    return_me = return_me*i;
  }
  return return_me;
}

void fitter(double *x, double *p)
{
  std::cout << "wut" << std::endl;
}

int main(int argc, char *argv[])
{
  TApplication *tapp = new TApplication("tapp",&argc,argv);  
  TCanvas *can = new TCanvas("can","can",500,500);
  can->Draw();
  tapp->Run();  
  return 0;
}

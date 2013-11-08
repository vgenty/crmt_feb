#include <fstream>
#include <cmath>
#include "TF1.h"
#include "TH1D.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TMath.h"
#include "dstyle.h"

std::vector<std::pair<double,double> > read_file(std::string file)
{
  std::ifstream myfile;
  std::vector<std::pair<double,double> > data;
  myfile.open(file.c_str());
  
  double holder1 ,holder2;
  
  while(myfile >> holder1 >> holder2)
    data.push_back(std::make_pair(holder1,holder2));
  
  myfile.close();
  return data;
}



void fitter(double *x, double *p)
{
  
  
}

int main(int argc, char *argv[])
{
  set_style();
  TApplication *tapp = new TApplication("tapp",&argc,argv);  
  TCanvas *can = new TCanvas("can","can");
  
  std::vector<std::pair<double,double> > data = read_file("adc_750VHV_3.07Vled_2.txt");
  int len = data.size();
  TH1D *h1 = new TH1D("Charge",";pVs;Count",len,data[0].first*pow(10,12),data[len-1].first*pow(10,12));
  
  for(int k=1;k<=len;k++)
    h1->SetBinContent(k,data[len-k].second);

  h1->Rebin(2);
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();
  h1->Draw();
  can->SetLogy();

  can->Draw();
  tapp->Run();  
  
  
  
  return 0;
}

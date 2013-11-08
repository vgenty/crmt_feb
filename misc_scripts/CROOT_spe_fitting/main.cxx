#include <fstream>
#include <cmath>
#include "TF1.h"
#include "TH1D.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TMath.h"
#include "dstyle.h"

#define PI 3.14159

double fitter(double *x, double *p)
{
  /*
    p[0] = \lambda
    p[1] = x_ped
    p[2] = \sigma_ped
    p[3] = d_f
    p[4] = x_pe
    p[5] = \sigma_pe
    p[6] = d_s
    p[7] = N
  */
  double ped  = exp(-1.0*p[0])*1/(sqrt(2*PI)*p[2])*exp(-1.0*pow(x[0]-p[1],2)/(2*pow(p[2],2)));
  double loop_factor   = (1-p[3]); 
  double pe=0;
  for (int r=1;r<=12;++r)
    pe = pe + (exp(-1.0*p[0])*pow(p[0],r)/TMath::Factorial(r))*exp(-1.0*pow(x[0]-p[1]-r*p[4],2)/(2.0*(pow(p[2],2)+r*pow(p[5],2))))/(sqrt(2*PI*(pow(p[2],2)+r*pow(p[5],2))));
  
  double last = p[3]*(1-exp(-1.0*p[0]))*exp(pow(x[0]-p[1]-p[4]/p[6],2)/(2.0*(pow(p[1],2)+pow(p[4],2)/pow(p[6],2))))/(sqrt(2*PI*(pow(p[1],2)+pow(p[4],2)/pow(p[6],2))));
  
  return p[7]*(ped + loop_factor*pe + last);

}




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

int main(int argc, char *argv[])
{
  
  
  set_style();
  TApplication *tapp = new TApplication("tapp",&argc,argv);  
  TCanvas *can = new TCanvas("can","can");
  //can->Divide(2);
  std::vector<std::pair<double,double> > raw_data = read_file("adc_750VHV_3.07Vled_2.txt");
  int len = raw_data.size();
  double scale = -1*pow(10.0,12);
  double xlow =raw_data[len-1].first*scale;
  double xhigh =raw_data[0].first*scale;
  int rebin = 10;
  
  //can->cd(1);
  can->SetLogy();
  TH1D *h1 = new TH1D("Charge",";pVs;Count",len,xlow,xhigh);
  TF1 *the_fit = new TF1("the_fit",fitter,xlow,xhigh,8);

    
  
  for (auto entry : raw_data)
     for (int i = 0; i <entry.second;++i)
       h1->Fill(entry.first*scale);
  /*
    p[0] = \lambda
    p[1] = x_ped
    p[2] = \sigma_ped
    p[3] = d_f
    p[4] = x_pe
    p[5] = \sigma_pe
    p[6] = d_s
    p[7] = N
  */
  
  the_fit->SetParameters(1.0,5.0,5.0,0.0,55.0,10.0,0.05);
  the_fit->FixParameter(7,h1->GetEntries());   
  
   
  h1->Rebin(rebin);
  h1->Fit("the_fit","V");
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();
  h1->Draw();
  /*
  can->cd(2);
  can->SetLogy();
  TH1D *h2 = new TH1D("Fake Charge",";pVs;Count",len,xlow,xhigh);
  TF1 *the_fake_fit = new TF1("the_fake_fit",fitter,xlow,xhigh,8);
  the_fake_fit->SetParameters(1.0,5.0,5.0,0.0,55.0,10.0,0.05,h1->GetEntries());
  
  
  for(int t=0; t<h1->GetEntries();t++)
    h2->Fill(the_fake_fit->GetRandom());
  
  h2->GetXaxis()->CenterTitle();
  h2->GetYaxis()->CenterTitle();
  h2->Rebin(rebin);
  h2->Draw();
  

  
  
  
  */
  can->Draw();
  tapp->Run();  
  
  
   
   return 0;
}

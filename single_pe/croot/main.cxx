#include <fstream>
#include <cmath>
#include "TF1.h"
#include "TH1D.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TMath.h"
#include "dstyle.h"
#include "TPaveText.h"
#include "TSpectrum.h"
#include <sstream>
#include <stdlib.h> 

#define PI 3.14159

std::string to_python(TF1 *the_fit)
{
  std::stringstream ss;
  double xpe      =  the_fit->GetParameter(4);
  double xpe_err  =  the_fit->GetParError(4);
  double npe      =  the_fit->GetParameter(0);
  double npe_err  =  the_fit->GetParError(0);
  
  ss << (xpe)*pow(10,-12)/(50*1.6*pow(10,-19));
  ss << std::endl;  
  ss << (xpe_err)*pow(10,-12)/(50*1.6*pow(10,-19));
  ss << std::endl;  
  ss << npe;
  ss << std::endl;  
  ss << npe_err;

  return ss.str();
}

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
  for (int r=1;r<=3;++r)
    pe = pe + (exp(-1.0*p[0])*pow(p[0],r)/TMath::Factorial(r))*exp(-1.0*pow(x[0]-p[1]-r*p[4],2)/(2.0*(pow(p[2],2)+r*pow(p[5],2))))/(sqrt(2*PI*(pow(p[2],2)+r*pow(p[5],2))));
  
  double last = p[3]*(1-exp(-1.0*p[0]))*exp(pow(x[0]-p[1]-p[4]/p[6],2)/(2.0*(pow(p[1],2)+pow(p[4],2)/pow(p[6],2))))/(sqrt(2*PI*(pow(p[1],2)+pow(p[4],2)/pow(p[6],2))));
  
  //return p[7]*(ped + loop_factor*pe + last);
  return p[7]*(ped+ loop_factor*pe);
  //return p[7]*(ped + pe);
  

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
  gStyle->SetOptFit(111);
  gStyle->SetOptStat("e");

  std::string filename = argv[1];
  
  //This is unacceptable, get voltage from filename
  
  char hunds  = filename.at(11);
  char tens   = filename.at(12);
  char ones   = filename.at(13);
  
  /*
  char hunds  = filename.at(15);
  char tens   = filename.at(16);
  char ones   = filename.at(17);
  */
  
  std::stringstream ssp;
  ssp << hunds << tens << ones;
  int voltage = std::stoi(ssp.str());
  //glad thats over

  int par = argc;
  
  std::vector<std::pair<double,double> > raw_data = read_file(filename);
  int len = raw_data.size();
  double scale = -1*pow(10.0,12);
  double xlow =raw_data[len-1].first*scale; //breaks here....sometimes
  double xhigh =raw_data[0].first*scale   ;
  //if(xhigh>200)xhigh = 200;
  //if(xlow <-50) xlow = -50;
  int rebin = 10;
  
  TApplication *tapp = new TApplication("tapp",&argc,argv);
  TCanvas *can = new TCanvas("can","can");    

  TH1D *h1 = new TH1D("Charge",";pVs;Count",len,xlow,xhigh);
  TF1 *the_fit = new TF1("the_fit",fitter,xlow,xhigh,8);

    
  
  for (auto entry : raw_data)
     for (int i = 0; i <entry.second;++i)
       h1->Fill(entry.first*scale);
  
  int n_peaks = 5 ;
  TSpectrum *s = new TSpectrum(n_peaks);
  int nfound = s->Search(h1,20.0,"",0.05);
  float *xpeaks = s->GetPositionX();
  
  double ped_peak  = (double)xpeaks[0];
  double pe_peak   = (double)xpeaks[1];
  if(pe_peak < 1 || voltage > 850) pe_peak = ped_peak+300;
  
  the_fit->SetParameter(0,0.5);                  //\lambda
  the_fit->SetParameter(1,ped_peak);            //x_ped
  the_fit->SetParameter(2,10.0);                  //\sigma_ped
  the_fit->SetParameter(3,0.01);                 //d_f
  the_fit->SetParameter(4,pe_peak);            //x_pe
  if (voltage <= 750) the_fit->SetParameter(5,5.0);                 //\sigma_pe
  if (voltage > 750) the_fit->SetParameter(5,5.0);                 //\sigma_pe
  if (voltage > 800) the_fit->SetParameter(5,30.0);                 //\sigma_pe
  the_fit->SetParameter(6,0.05);                 //d_s
  the_fit->SetParameter(7,h1->GetEntries());  //N
  
  the_fit->SetParLimits(0,0,10);
  the_fit->SetParLimits(1,0,1000);
  the_fit->SetParLimits(2,0,1000);
  the_fit->SetParLimits(3,0,1);
  the_fit->SetParLimits(4,0,1000);
  the_fit->SetParLimits(5,0,1000);
  if(voltage > 875)the_fit->SetParLimits(5,0,300);
  the_fit->SetParLimits(6,0,1000);
  
  
   
  h1->Rebin(rebin);
  h1->Fit("the_fit","Q");
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();


  
  std::cout << to_python(the_fit) << std::endl;
  
  if(par >= 3){
    std::cout << "ped_peak: " << ped_peak << std::endl;
    std::cout << "pe_peak: "  << pe_peak  << std::endl;
    for(int i = 2;i<n_peaks;++i)
      std::cout << "peak : " << (i+1) << ": " << xpeaks[i] << std::endl; 
    
    TPaveText *title = new TPaveText(0.1412338,0.9321267,0.4399351,0.9886878,"brNDC");
    title->SetTextFont(63);
    title->SetTextSize(25);
    title->SetBorderSize(0);
    title->SetFillColor(0);
    title->AddText("Title");  
    h1->Draw();
    title->Draw("SAMES");
    can->SetLogy();
    can->Draw();
    tapp->Run();  
  
  }

  
   
   return 0;
}

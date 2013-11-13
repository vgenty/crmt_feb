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

#define PI 3.14159

std::string to_python(double xpe, double xped, double npe)
{
  std::stringstream ss;
 
  ss << (xpe - xped)*pow(10,-12)/(50*1.6*pow(10,-19));
  ss << std::endl;  
  ss << npe;
  
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
  for (int r=1;r<=4;++r)
    pe = pe + (exp(-1.0*p[0])*pow(p[0],r)/TMath::Factorial(r))*exp(-1.0*pow(x[0]-p[1]-r*p[4],2)/(2.0*(pow(p[2],2)+r*pow(p[5],2))))/(sqrt(2*PI*(pow(p[2],2)+r*pow(p[5],2))));
  
  double last = p[3]*(1-exp(-1.0*p[0]))*exp(pow(x[0]-p[1]-p[4]/p[6],2)/(2.0*(pow(p[1],2)+pow(p[4],2)/pow(p[6],2))))/(sqrt(2*PI*(pow(p[1],2)+pow(p[4],2)/pow(p[6],2))));
  
  //return p[7]*(ped + loop_factor*pe + last);
  //return p[7]*(ped + loop_factor*pe);
  return p[7]*(ped + pe);
  

}

std::vector<std::pair<double,double> > read_file(std::string file)
{
  std::ifstream myfile;
  std::vector<std::pair<double,double> > data;
  //std::cout << "file: " << std::endl;
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
  TPaveText *title = new TPaveText(0.1412338,0.9321267,0.4399351,0.9886878,"brNDC");

  title->SetTextFont(63);
  title->SetTextSize(25);
  title->SetBorderSize(0);
  title->SetFillColor(0);
  title->AddText("Title");
  

  std::string filename = argv[1];
  int par = argc;
  //tapplication sucks up arguments be careful
  TApplication *tapp = new TApplication("tapp",&argc,argv);  
  TCanvas *can = new TCanvas("can","can");
  //can->Divide(2);
  
  std::vector<std::pair<double,double> > raw_data = read_file(filename);
  int len = raw_data.size();
  double scale = -1*pow(10.0,12);
  double xlow =raw_data[len-1].first*scale - 5; //breaks here....
  double xhigh =raw_data[0].first*scale    + 5 ;
  //int rebin = 4;
  //  int rebin = 5;
    int rebin = 10;
  
  
  can->SetLogy();
  TH1D *h1 = new TH1D("Charge",";pVs;Count",len,xlow,xhigh);
  TF1 *the_fit = new TF1("the_fit",fitter,xlow,xhigh,8);

    
  
  for (auto entry : raw_data)
     for (int i = 0; i <entry.second;++i)
       h1->Fill(entry.first*scale);
  
  
  TSpectrum *s = new TSpectrum(2);
  int nfound = s->Search(h1,15,"",0.1);
  float *xpeaks = s->GetPositionX();
  
  
  
  the_fit->SetParameter(0,0.5);                  //\lambda
  the_fit->SetParameter(1,xpeaks[0]);            //x_ped
  the_fit->SetParameter(2,05.0);                  //\sigma_ped
  the_fit->SetParameter(3,0.01);                 //d_f
  the_fit->SetParameter(4,xpeaks[0]+30);            //x_pe
  the_fit->SetParameter(5,5.0);                 //\sigma_pe
  the_fit->SetParameter(6,0.05);                 //d_s
  the_fit->SetParameter(7,h1->GetEntries()/10);  //N
  
  the_fit->SetParLimits(0,0,10);
  the_fit->SetParLimits(1,0,1000);
  the_fit->SetParLimits(2,0,1000);
  the_fit->SetParLimits(3,0,1);
  the_fit->SetParLimits(4,1,1000);
  the_fit->SetParLimits(5,0,1000);
  the_fit->SetParLimits(6,0,1000);

  
   
  h1->Rebin(rebin);
  //h1->Fit("the_fit","V");
  h1->Fit("the_fit","Q");
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();


  
  std::cout << to_python(the_fit->GetParameter(4),
			 the_fit->GetParameter(1),
			 the_fit->GetParameter(0)) << std::endl;
  
  if(par >= 3){
    std::cout << "ped guess: " << xpeaks[0];
    std::cout << " 2nd peak guess: " << xpeaks[1] << std::endl;
    std::cout << "ped + 50 guess" <<  xpeaks[0]+50 << std::endl;
    h1->Draw();
    title->Draw("SAMES");
    can->Draw();
    tapp->Run();  
  
  }

  
   
   return 0;
}

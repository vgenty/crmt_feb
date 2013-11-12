#!/usr/bin/env python

from ROOT import TF1, TH1D, TCanvas, gROOT
from fit_methods import *
import sys
from other_methods import *

def main():
    gROOT.SetStyle("DStyle")
    c1=TCanvas("c1","c1")
    c1.cd()

    content = open_file("adc_750VHV_3.07Vled.txt")    
    bins = 2000
    xlow  = float(content[0][0])*10**12+30
    xhigh = float(content[0][bins-1])*10**12+30

    h1 = TH1D("h1",";pVs;Counts",bins,xlow,xhigh)
    
    h1.GetXaxis().CenterTitle()
    h1.GetYaxis().CenterTitle()
    
    title = GetTitle("Charge Spectrum")

    print 'Got %d values from file' % len(content[1])
    print 'Have %d bins in histogram' % bins
    
    for _bin in xrange(bins):
        h1.SetBinContent(_bin,float(content[1][bins-_bin-1]))

    '''
    w       = par[0]
    sigma_0 = par[1]
    Q_0     = par[2]
    alpha   = par[3]
    mu      = par[4]
    sigma_1 = par[5]
    Q_sh    = par[6]
    Q_1     = par[7]
    char_amp= par[8]
    ped_amp = par[9]
    '''
    

    #fit_func = TF1("fit_func",the_fit,xlow+30,xhigh+30,10)
    #fit_func.SetParameters(0.3,2,7.0,0.035,1.68,15,50,35.05,1.0,1.0)
    #fit_func.SetParameter(8, 1000)
    #fit_func.SetParameter(9, 1200)
    #fit_func.SetParLimits(0,0,1)
    h1.Rebin(2)
    h1.SetLineWidth(1)
    #h1.Fit("fit_func","V")    
    h1.Draw()
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
        
    '''
    c2=TCanvas("c2","c2")
    c2.cd()
    h2 = TH1D("h2",";ADC;Counts",bins/10,0,300)
    function = TF1("function",the_fit,0,300,10)
    #Var Names
    h2.GetXaxis().CenterTitle()
    h2.GetYaxis().CenterTitle()    
    function.SetParameters(0.3,2,23.26,0.035,1.68,11.73,50,35.05,1.0,1)
    
    for j in xrange(10000) :
        h2.Fill(function.GetRandom())    
    
    function.SetParameter(8, 200)
    function.SetParameter(9, 200)
        
    title2=GetTitle("Test Spectrum")
    
    h2.Fit("function","V")
    h2.Draw()
    title2.Draw("SAMES")
    
    c2.Update()
    c2.Modified()
    '''
    sys.stdin.readline()
    
    
if __name__ == '__main__' :
    main()
    


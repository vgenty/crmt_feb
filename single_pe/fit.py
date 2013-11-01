#!/usr/bin/env python

from ROOT import TF1, TH1D, TCanvas
from fit_methods import *
import sys
from FixROOT import OneFix

def main():
    c1=TCanvas("c1","c1")
    c1.cd()

    content = open_file("data_2.dat")    
    bins = 2000
    xlow  = float(content[0][0])*10**12+30
    xhigh = float(content[0][bins-1])*10**12+30

    h1 = TH1D("h1",";ADC;Counts",bins,xlow,xhigh)
    fixer=OneFix()
    title=fixer.fix(h1,"Charge Spectrum")

    print 'Got %d values from file' % len(content[1])
    print 'Have %d bins in histogram' % bins
    
    for _bin in xrange(bins):
        h1.SetBinContent(_bin,float(content[1][bins-_bin-1]))

    h1.Rebin(10)
    h1.Draw()
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
        
    
    c2=TCanvas("c2","c2")
    c2.cd()
    h2 = TH1D("h2",";ADC;Counts",bins/10,0,300)
    function = TF1("function",the_fit,0,300,10)
    #Var Names
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
    
    function.SetParameters(0.3,2,23.26,0.035,1.68,11.73,50,35.05,1.0,1)
    
    for j in xrange(10000) :
        h2.Fill(function.GetRandom())    
    
    function.SetParameter(8, 200)
    function.SetParameter(9, 200)
        
    title2=fixer.fix(h2,"NIM Spectrum")
    
    h2.Fit("function","V")
    h2.Draw()
    title2.Draw("SAMES")
    
    c2.Update()
    c2.Modified()
    sys.stdin.readline()
    
    
if __name__ == '__main__' :
    main()
    


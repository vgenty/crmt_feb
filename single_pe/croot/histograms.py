from ROOT import gStyle, TH1D, TGraphErrors, gROOT, TAxis
from ROOT import TCanvas, TH2D, TF1, TPad, TPaveText
import sys, os
import subprocess
from methods import *


def myfit(x,par):
    a = 0.0
    a = (par[0]*x[0])**(par[1])
    return a

def histogram(pmt_num):
    #Set vicstyle from .rootlogon.C
    gROOT.SetStyle("VStyle")
    evt = pmt_num
    #Git files and threshold
    files  = [f for f in os.listdir("./data/pmt_%s/" % evt)]
    wrange = [float(k[5:9]) for k in files]
    
    #Run fitter (pretty slow)
    data  = [subprocess.check_output(["./fit","data/pmt_%s/%s" % (evt,f)]).rstrip().split('\n') for f in files]
    
    #Data from stdout, write to lists
    volts     =  [float(f[0:3]  ) for f in files   ]
    gain      =  [float(value[0]) for value in data]
    gain_err  =  [float(value[1]) for value in data]
    npe       =  [float(value[2]) for value in data]    
    npe_err   =  [float(value[3]) for value in data]
    
    ################## Gain Plot ##################
    
    
    
    fitter = TF1("fitter",myfit,600,1000,2)
    fitter.SetParameters(0.01,10)
    fitter.SetParLimits(0,0,100)
    fitter.SetParLimits(1,0,20)
    tg = TGraphErrors()
    for x in xrange(len(volts)):
        tg.SetPoint(x,volts[x],gain[x])
        tg.SetPointError(x,1,gain_err[x])
        
    tg.Fit("fitter")
    return fitter    

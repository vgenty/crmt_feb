from ROOT import gStyle, TH1D, TGraphErrors, gROOT, TAxis
from ROOT import TCanvas, TH2D, TF1, TPad, TPaveText
import sys, os
import subprocess
from methods import *


def myfit(x,par):
    a = 0.0
    a = (par[0]*x[0])**(par[1])
    return a

def main():
    #Set vicstyle from .rootlogon.C
    gROOT.SetStyle("VStyle")
    evt = sys.argv[1]
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
    c1 = TCanvas("c1","c1")
    fitter = TF1("fitter",myfit,700,900,2)
    fitter.SetParameters(0.01,10)
    fitter.SetParLimits(0,0,100)
    fitter.SetParLimits(1,0,20)
    gStyle.SetOptFit(1111)
    tg = TGraphErrors()
    for x in xrange(len(volts)):
        tg.SetPoint(x,volts[x],gain[x])
        tg.SetPointError(x,1,gain_err[x])
        
    tg_title = GetTitle("PMT Gain")
    tg.SetTitle(";Voltage;Gain")
    tg.GetXaxis().CenterTitle()
    tg.GetYaxis().CenterTitle()
    tg.Fit("fitter")
    tg.Draw("AP")
    tg_title.Draw("SAMES")
    tg.SetMarkerStyle(8)
    c1.Update()
    c1.Modified()
    ################## Gain Plot ##################

    '''
    ################## npev Plot ##################

    c1_2 = TCanvas("c1_2","c1_2",720,610)
    t3=TPad("t3","t3",0.0,0.0,1.0,0.6)
    t4=TPad("t4","t4",0.0,0.6,1.0,1.0)

    t4.SetBottomMargin(0)
    t3.SetTopMargin(0)
    t3.cd()
    tg_2 = TGraphErrors()
    for x in xrange(len(volts)):
        tg_2.SetPoint(x,npe[x],volts[x])
        tg_2.SetPointError(x,npe_err[x],1)
    tg_2.SetPoint(len(volts),0.92,770)

    tg_2.SetTitle(";NPE;Voltage")
    tg_2.GetXaxis().CenterTitle()
    tg_2.GetYaxis().CenterTitle()

    tg_2.Draw("AP")
    t3.Update()
    t3.Modified()
    tg_2.GetXaxis().SetTitleOffset(1.55)
    tg_2.GetYaxis().SetTitleOffset(1.10)

    tg_2.SetMarkerStyle(8)
    tg_2.Draw("AP")
    t3.Update()
    t3.Modified()
    t4.cd()

    


    c2 = TCanvas("c2","c2")

    h  = TH1D("h",";;Counts/0.04 NPE",25,0,1)
    for value in npe:
        h.Fill(value)
    

    h.GetYaxis().CenterTitle()
    h.GetXaxis().CenterTitle()
    h_title = GetTitle("NPE")    
    

    h.Draw()
    h_title.Draw("SAMES")

    c2.Update()
    c2.Modified()

    h.GetXaxis().SetNdivisions(0)
    h.GetYaxis().SetTitleOffset(1.10)
    h.Draw()
    c1_2.cd()
    t3.Draw()
    t4.Draw()
    c1_2.Update()
    c1_2.Modified()
    ################## npe Plot ##################
    ################## Volts-Gain wRange ##################
    c3 = TCanvas("c3","c3")
    tg_wr = TH2D("tg_wr",";;;",20,700,920,1000,100000,100000000)
    
    for x in xrange(len(wrange)):
        tg_wr.Fill(volts[x],gain[x],wrange[x])

    tg_wr_title = GetTitle("Volts, Gain, Window Range")
    tg_wr.SetTitle(";Voltage (V);Gain)")
    tg_wr.GetXaxis().CenterTitle()
    tg_wr.GetYaxis().CenterTitle()
    
    tg_wr.Draw("COLZ")
    tg_wr_title.Draw("SAMES")
    c3.Update()
    c3.Modified()
    ################## Volts-Gain-wRange ##################
    ################## VoltsGain-npe ##################
    c4 = TCanvas("c4","c4")
    tg_np = TH2D("tg_np",";;;",20,700,920,1000,100000,100000000)
    for x in xrange(len(wrange)):
        tg_np.Fill(volts[x],gain[x],npe[x])

    c4.SetLogy()
    tg_np_title = GetTitle("Volts, Gain, npe")
    tg_np.SetTitle(";Voltage (V);Gain")
    tg_np.GetXaxis().CenterTitle()
    tg_np.GetYaxis().CenterTitle()
    tg_np.Draw("COLZ")
    tg_np_title.Draw("SAMES")
    c3.Update()
    c3.Modified()
    ################## Volts-Gain-npe ##################
    '''
    sys.stdin.readline()

if __name__ == '__main__' :
    main()

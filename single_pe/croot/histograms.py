from ROOT import gStyle, TH1D, TGraphErrors, gROOT
from ROOT import TCanvas, TPaveText
import sys, os
import subprocess

def GetTitle(words):
    fTitle = TPaveText(0.1412338,0.9321267,0.4399351,0.9886878,"brNDC")
    fTitleWords = words
    fTitle.SetTextFont(63)
    fTitle.SetTextSize(24)
    fTitle.SetBorderSize(0)
    fTitle.SetFillColor(0)
    fTitle.AddText(words)
    
    return fTitle

def main():
    gROOT.SetStyle("DStyle")
    


    files = [f for f in os.listdir("./data")]
    
    #running fitter (pretty slow)
    data  = [subprocess.check_output(["./fit","data/%s" % f]).rstrip().split('\n') for f in files]
    
    volts = [float(f[0:3]  ) for f in files   ]
    gain =  [float(value[0]) for value in data]
    npe  =  [float(value[1]) for value in data]
    
    
    ################## Gain Plot ##################
    c1 = TCanvas("c1","c1")
    tg = TGraphErrors()
    for x in xrange(len(volts)):
        tg.SetPoint(x,volts[x],gain[x])
    
    tg.SetTitle(";Voltage;Gain")
    tg.GetXaxis().CenterTitle()
    tg.GetYaxis().CenterTitle()
    tg.Draw("AP")
    tg.SetMarkerStyle(8)
    c1.Update()
    c1.Modified()
    ################## Gain Plot ##################
    ################## npe Plot ##################
    c2 = TCanvas("c2","c2")
    h  = TH1D("h",";Npe;Count",25,0,1)
    for value in npe:
        h.Fill(value)
    h.GetYaxis().CenterTitle()
    h.GetXaxis().CenterTitle()
    h_title = GetTitle("NPE")    
    h.Draw()
    h_title.Draw("SAMES")
    c2.Update()
    c2.Modified()
    ################## npe Plot ##################

    sys.stdin.readline()

if __name__ == '__main__' :
    main()

from ROOT import gStyle, TH1D, TGraphErrors, gROOT
from ROOT import TCanvas, TPaveText, TH2D
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

    files  = [f for f in os.listdir("./data")]
    wrange = [float(k[5:9]) for k in files]
    
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

    tg_title = GetTitle("PMT Gain")
    tg.SetTitle(";Voltage;Gain")
    tg.GetXaxis().CenterTitle()
    tg.GetYaxis().CenterTitle()
    tg.Draw("AP")
    tg_title.Draw("SAMES")
    tg.SetMarkerStyle(8)
    c1.Update()
    c1.Modified()
    ################## Gain Plot ##################
    ################## npe Plot ##################
    c2 = TCanvas("c2","c2")
    h  = TH1D("h",";Npe;Count",50,0,1)
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
    ################## Volts-Gain wRange ##################
    c3 = TCanvas("c3","c3")
    tg_wr = TH2D("tg_wr",";;;",20,700,920,1000,100000,100000000)
    
    for x in xrange(len(wrange)):
        tg_wr.Fill(volts[x],gain[x],wrange[x])
    tg_wr_title = GetTitle("Volts, Gain, Window Range")
    tg_wr.SetTitle(";Voltage (V);Gain)")
    tg_wr.GetXaxis().CenterTitle()
    tg_wr.GetYaxis().CenterTitle()
    #c3.SetLogz()
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

    sys.stdin.readline()

if __name__ == '__main__' :
    main()

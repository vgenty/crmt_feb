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
    print wrange
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
    ################## Volts-wRange-Gain Range ##################
    c3 = TCanvas("c3","c3")
    tg_wr = TH2D("tg_wr",";;;",10,700,910,35,0,35)

    for x in xrange(len(wrange)):
        tg_wr.Fill(volts[x],wrange[x],gain[x])
    tg_wr_title = GetTitle("Volts, WRange, Gain")
    tg_wr.SetTitle(";Voltage (V);Threshold (mV)")
    tg_wr.GetZaxis().SetRangeUser(100000,10000000)
    tg_wr.GetXaxis().CenterTitle()
    tg_wr.GetYaxis().CenterTitle()
    c3.SetLogz()
    tg_wr.Draw("COLZ")
    tg_wr_title.Draw("SAMES")
    c3.Update()
    c3.Modified()
    ################## Volts-wRange-Gain Range ##################
    ################## Volts-wRange-Gain npe ##################
    c4 = TCanvas("c4","c4")
    tg_np = TH2D("tg_np",";;;",10,700,910,100,100000,100000000)
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
    ################## Volts-wRange-Gain npe ##################

    sys.stdin.readline()

if __name__ == '__main__' :
    main()

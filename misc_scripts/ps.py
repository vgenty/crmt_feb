import ROOT
from ROOT import TFile, TCanvas, TH1D, gStyle
from ROOT import gROOT
from other_methods import *
import sys

gROOT.SetStyle("DStyle")
f = TFile("recodata.root","READ")
tree = f.Get("Event Tree")

c1 = TCanvas("c1","c1")
c1.cd()
fangle = TH1D("fx_LowSlope",";Slope_reco - Slope_ps;Count",100,-1.5,1.5)
for event in tree:
    fangle.Fill(event.Slope - event.fx_LowSlope)
title=GetTitle("Slope-Slope")
fangle.GetYaxis().CenterTitle()
fangle.GetXaxis().CenterTitle()
fangle.Draw()
title.Draw("SAMES")


c2 = TCanvas("c2","c2")
c2.cd()
#c1.SetLogy()
fyinter = TH1D("fx_LowYinter",";Yinter_reco - Yinter_ps;Count",100,-10,10)

for event in tree:
    fyinter.Fill(event.Yinter - event.fx_LowYinter)

title_y=GetTitle("Yinter-Yinter")
fyinter.GetYaxis().CenterTitle()
fyinter.GetXaxis().CenterTitle()
fyinter.Draw()
title_y.Draw("SAMES")

c3 = TCanvas("c3","c3")
c3.cd()
c3.SetLogy()
h1_all  = TH1D("pvalue_all",";P-Value;Count",100,0,1)
h1_mine = TH1D("pvalue_mine",";P-Value;Count",100,0,1)
title_all = GetTitle("P-Value All Events")
for event in tree:
    for value in event.fxp_Zvalue:
        h1_all.Fill(value)
    h1_mine.Fill(event.fxp_LowZValue)

h1_all.GetYaxis().CenterTitle()
h1_all.GetXaxis().CenterTitle()
h1_all.Draw()
h1_mine.SetLineColor(ROOT.kRed)
h1_mine.SetFillColor(ROOT.kRed)
h1_mine.Draw("SAMES")
title_all.Draw("SAMES")
c3.Update()
c3.Modified()
sys.stdin.readline()


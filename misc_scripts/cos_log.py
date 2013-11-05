from ROOT import TFile, TCanvas, TH1D, gStyle
from FixROOT import OneFix
import sys

fixer = OneFix()

c1 = TCanvas("c1","c1")
c1.SetLogy()
f = TFile("recodata.root","READ")
tree = f.Get("Event Tree")

fangle = TH1D("fcosangle",";cos#theta;Count",100,0,1)

for event in tree:
    fangle.Fill(event.fCosAngle)
fangle.GetXaxis().SetRangeUser(0.7,1.0)
gStyle.SetStatBorderSize(2)
gStyle.SetStatFont(62)
title=fixer.fix(fangle,"Recon. Cos(Angle)")

fangle.Draw()
title.Draw("SAMES")

sys.stdin.readline()


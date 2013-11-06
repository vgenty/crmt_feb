from ROOT import TFile, TCanvas, TH1D, gStyle
from other_methods import *
from ROOT import gROOT
import sys

gROOT.SetStyle("DStyle")

f = TFile("recodata.root","READ")
tree = f.Get("Event Tree")

fangle = TH1D("fangle",";#theta (rad);Count/0.05 rad",60,-1.5,1.5)

for event in tree:
    fangle.Fill(event.fAngle)
#gStyle.SetStatBorderSize(2)
#gStyle.SetStatFont(62)
#title=fixer.fix(fangle,"Recon. Angle")
title=GetTitle("Recon. Angle")
fangle.Draw()
title.Draw("SAMES")

sys.stdin.readline()


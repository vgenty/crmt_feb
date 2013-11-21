from ROOT import gStyle, TH1D, TGraphErrors, gROOT, TAxis
from ROOT import TCanvas, TH2D, TF1, TPad, TPaveText, TMath
import sys, os, math
import subprocess
import histograms
from methods import *

def main():
    #set up vics style from .rootlogon.C
    gROOT.SetStyle("VStyle")
    gain = 10000000.0
    c1 = TCanvas("c1","c1")
    info=[]
    #for x in xrange(sys.argv[1]):
    for x in xrange(2):
        info.append(histograms.histogram(x+1))

    values = [TF1("gain_%d" % info.index(func), "(1/(%f))*TMath::Exp(TMath::Log(x)/(%f))" % (func.GetParameter(0),func.GetParameter(1)),1000,1000000000) for func in info]
    
    graph = TGraphErrors()
    for index in xrange(len(values)):
        graph.SetPoint(index, values[index](gain), values.index(values[index]) + 1)
        #graph.SetPointError(index, values[index](1), 0) #what should the error in the gain be? fix this
        graph.SetPointError(index, 0, 0) #what should the error in the gain be? fix this
        
    graph.SetTitle(";Voltage (V);PMT ID")
    graph.GetYaxis().CenterTitle()
    graph.GetXaxis().CenterTitle()
    graph.SetMarkerStyle(8)
    title = GetTitle("PMT Gain: %.3e" % gain)
    graph.Draw("AP")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
    

    sys.stdin.readline()
    
if __name__ == '__main__' :
    main()

from ROOT import gStyle, TH1D, TGraphErrors, gROOT, TAxis, TLatex
from ROOT import TCanvas, TH2D, TF1, TPad, TPaveText, TMath
import sys, os, math
import subprocess
import histograms
from methods import *

def function(x,par):
    return par[0]




def main():
    #set up vics style from .rootlogon.C
    gROOT.SetStyle("VStyle")
    gStyle.SetOptFit(111)
    gain = 10000000.0
    c1 = TCanvas("c1","c1")
    info=[]
    #for x in xrange(sys.argv[1]):
    #this is slow
    for x in xrange(8):
        info.append(histograms.histogram(x+1))


    #take fit functions from info, create inverse TF1, give gain, return voltage
    values = [TF1("gain_%d" % info.index(func), "(1/(%f))*TMath::Exp(TMath::Log(x)/(%f))" % (func.GetParameter(0),func.GetParameter(1)),1000,1000000000) for func in info]
    errors = [TF1("error_func","TMath::Sqrt((1/(%f)**2*TMath::Exp(TMath::Log(x)/%f)*%f)**2 + (1/(%f)*TMath::Log(x)/(%f)**2*TMath::Exp(TMath::Log(x)/%f)*%f)**2 + 1)" % (func.GetParameter(0),func.GetParameter(1),func.GetParError(0),func.GetParameter(0),func.GetParameter(1),func.GetParameter(1),func.GetParError(1)),1000,1000000000) for func in info]
    
    
    alphas       = [float(n.GetParameter(1))/12.0 for n in info]
    error_alphas = [float(n.GetParError(1 ))/12.0 for n in info]

    graph = TGraphErrors()
    for index in xrange(len(values)):
        graph.SetPoint(index, values.index(values[index]) + 1, values[index](gain))
        graph.SetPointError(index, 0, errors[index](gain)) 
        
    fit_volts = TF1("fit_volts",function,600,1000,1)
    fit_volts.SetParameter(0,800)
    fit_volts.SetParName(0,"Vf")
    graph.SetTitle(";PMT ID;Voltage (V)")
    graph.GetYaxis().CenterTitle()
    graph.GetXaxis().CenterTitle()
    graph.SetMarkerStyle(8)
    title = GetTitle("PMT Gain: %.2e" % gain)
    graph.Fit("fit_volts")
    graph.Draw("AP")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
    
    c2 = TCanvas("c2","c2")
    alpha_graph = TGraphErrors();
    upper = TF1("upper","0.8",0,8)
    lower = TF1("lower","0.7",0,8)
    for alpha in alphas:
        index = alphas.index(alpha)
        alpha_graph.SetPoint(index,index+1,alpha)
        alpha_graph.SetPointError(index,0,error_alphas[index])

    alpha_graph.SetTitle(";PMT ID;#alpha")
    alpha_graph.GetYaxis().CenterTitle()
    alpha_graph.GetXaxis().CenterTitle()
    alpha_graph.SetMarkerStyle(8)
    alpha_title = GetTitle("#alpha From Fit")

    
    alpha_graph.Draw("AP")
    alpha_title.Draw("SAMES")
    upper.Draw("SAMES")
    lower.Draw("SAMES")
    alpha_graph.GetYaxis().SetRangeUser(0,1)
    c2.Update()
    c2.Modified()
    
    
    sys.stdin.readline()
    
if __name__ == '__main__' :
    main()

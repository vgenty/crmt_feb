from ROOT import TH1D, TCanvas, TF1, TFile,gStyle, TLatex, TPad
from FixROOT import OneFix, TPaveTitle
import sys, math


def main():
    fixer = OneFix()

    c1=TCanvas("c1","c1")#,600,800)
    #tp1 = TPad("tp1","tpad1",0,0.3,1,1.0)
    #tp2 = TPad("tp1","tpad2",0,0.0,1,0.3)

    #tp1.cd()
    filename = TFile("III-result.root","READ")
    h1 = filename.deltatime_between_events_hightimes.Clone("h1")
    h1.SetTitle(";Time (s);Count")
    h1.Rebin(200)
    h1.SetAxisRange(0.0,0.07)
    
    fitter   = TF1("fitter","[0]*exp([1]*x)",0.0,0.7)
    fitter.SetParName(0,"A")
    fitter.SetParName(1,"b")
    fitter.SetParameters(250,-50)

    gStyle.SetOptFit(111)
    binWidth = h1.GetBinWidth(2)
    equation = TPaveTitle("f(x)=Ae^{bx}").GetTitle()
    title=fixer.fix(h1,"Rate")


    
    h1.Draw()
    h1.Fit("fitter")
    title.Draw("SAMES")
    equation.Draw("SAMES")
    c1.Update()
    c1.Modified()
    
    #tp2.cd()

    
    #values=[]
    #for entry in xrange(h1.GetNbinsX()) :
    #    b = float(entry)/float(h1.GetNbinsX())
    #    #print h1.GetBinContent(entry)
    #    values.append(fitter(b)-h1.GetBinContent(entry))

    #for entry in xrange(h1.GetNbinsX()) :
    #    h2.SetBinContent(entry+1,values[entry])

    #print values
    #h2.SetAxisRange(0.1,0.07)
    #h2title=fixer.fix(h2,"Residual")
    #h2.Draw("P")
    #h2.SetMarkerStyle(8)
    #h2title.Draw("SAMES")
    
    #c1.cd()

    #tp1.Draw()
    #tp1.Update()
    #tp1.Modified()

    #tp2.Draw()
    #tp2.Update()
    #tp2.Modified()


    sys.stdin.readline()
if __name__ == '__main__' :
    main()

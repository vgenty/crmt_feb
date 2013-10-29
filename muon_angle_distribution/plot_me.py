import sys
from ROOT import TCanvas, TH1D, TFile, gStyle
from FixROOT import OneFix, TPaveTitle


def main():
    fixer = OneFix()
    c1=TCanvas("c1","c1")
    filename = TFile("recodata.root")
    tree = filename.Get("Recon Tree")
    tree.GetEntry(0) #what the fuck does this do
    h1 = TH1D("angles",";;",100,-1.5,1.5)
    
    
    values = [k for k in tree.ReconAngles]
    for x in values:
        h1.Fill(x)

    h1.Rebin(2)
    h1.Rebin(2)
    h1.SetTitle(";Angle (rad); Count/%.2f rad" % h1.GetBinWidth(0))
    title=fixer.fix(h1,"Reconstructed Angle from Zenith")
    entries = TPaveTitle("Entries: %d" % h1.GetEntries()).GetTitle()
    mean = TPaveTitle("Mean: %f" % h1.GetMean()).GetTitle()
    rms = TPaveTitle("RMS: %f" % h1.GetRMS()).GetTitle()
    labels=[title,mean,entries,rms]
    

    h1.SetFillColor(2)
    h1.SetFillStyle(3002)
    h1.Draw()
    for label in labels:
        label.Draw("SAMES")
    
    
    sys.stdin.readline()



if __name__ == '__main__' :
    main()

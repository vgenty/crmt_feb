import sys
from ROOT import TCanvas, TH1D, TFile, gStyle
from FixROOT import OneFix, TPaveTitle


def main():
    fixer = OneFix()
    c1=TCanvas("c1","c1",500,600)
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
    mean = TPaveTitle("Mean: %.2f" % h1.GetMean()).GetTitle()
    rms = TPaveTitle("RMS: %.2f" % h1.GetRMS()).GetTitle()
    labels=[title,mean,entries,rms]
    
    gStyle.SetPadTopMargin(0.08)
    gStyle.SetPadBottomMargin(0.12)
    gStyle.SetPadLeftMargin(0.14)
    gStyle.SetPadRightMargin(0.1)
    gStyle.SetFrameLineWidth(2)
    h1.GetYaxis().SetTitleOffset(1.2)
    h1.SetFillColor(2)
    h1.SetFillStyle(3002)
    h1.Draw()
    for label in labels:
        label.Draw("SAMES")
    
    
    c1.Update()
    c1.Modified()
        
    sys.stdin.readline()



if __name__ == '__main__' :
    main()

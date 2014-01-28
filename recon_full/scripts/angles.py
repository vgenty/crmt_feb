from ROOT import TCanvas, TStyle, TLatex, TPaveText
from ROOT import TH1D, TFile, gStyle, gROOT
from ROOT import TMultiGraph

import sys

def GetTitle(words):
    
    fTitle = TPaveText(0.3135314,0.9285714,0.6122112,0.9897959,"brNDC")
    fTitleWords = words
    fTitle.SetTextFont(63)
    fTitle.SetTextSize(24)
    fTitle.SetBorderSize(0)
    fTitle.SetFillColor(0)
    fTitle.AddText(words)

    return fTitle

def main():
    gROOT.SetStyle("VStyle")
    gStyle.SetOptStat(0)

    #get files
    f = TFile("../output/recodata.root","READ")
    sim = TFile("../input/bfile.root","READ")
    
    #get trees
    tree = f.Get("Event Tree 3D")
    sim_tree = sim.Get("SimulationTree")
    
    phi_sim   = TH1D("Phi_sim",";#phi (rad);Count",100,0,6.3)
    theta_sim = TH1D("theta_sim",";#theta (rad);Count",100,-2.0,0.5)
    
    phi   = TH1D("Phi",";;",100,0,6.3)
    theta = TH1D("theta",";;",100,-2.0,0.5)
    
    nevents = tree.GetEntries()
    nevents_sim = sim_tree.GetEntries()
    print "nevents_sim: " + str(nevents_sim)

    for x in xrange(nevents_sim):
        sim_tree.GetEntry(int(x))
        theta_sim.Fill(-1.0*(3.14-sim_tree.Theta))
        phi_sim.Fill(sim_tree.Phi)
 
    
    for x in xrange(nevents):
        tree.GetEntry(int(x))
        theta.Fill(tree.fTheta)
        phi.Fill(tree.fPhi)

    c1 = TCanvas()
    c1.cd()
    theta_sim.GetXaxis().CenterTitle()
    theta_sim.GetYaxis().CenterTitle()
    theta_sim.Draw()
    theta.SetLineColor(2)
    theta.SetFillColor(2)
    theta.SetFillStyle(3003)
    theta.Draw("SAMES")
    theta_title = GetTitle("Zenith Angle") 
    theta_title.Draw("SAMES")
    c1.Update()
    c1.Modified()
    
    
    c2 = TCanvas()
    c2.cd()
    phi_sim.GetXaxis().CenterTitle()
    phi_sim.GetYaxis().CenterTitle()
    phi_sim.Draw()
    phi.SetLineColor(2)
    phi.SetFillColor(2)
    phi.SetFillStyle(3003)
    phi.Draw("SAMES")
    phi_title = GetTitle("Polar Angle") 
    phi_title.Draw("SAMES")
    phi_sim.GetYaxis().SetRangeUser(0,1500)
    c2.Update()
    c2.Modified()
    
    sys.stdin.readline()
    
if __name__ == '__main__':
    main()

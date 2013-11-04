#!/usr/bin/env python

from ROOT import TH2D, TCanvas, TLatex, TF1, TFile, TGraphErrors, TMultiGraph
from FixROOT import OneFix
from display_methods import *
from fit_methods import *
import sys
      
def main():
    fixer=OneFix()
    f = TFile("recodata.root","READ")
    event = sys.argv[1]
    
    #this is for the event display
    c1     = TCanvas("c1","c1",800,500)
    module = TH2D("mod",";x #rightarrow;y #rightarrow",64,0,64,4,0,4)
    
    pins_to_pixels   =  pins("pins.csv")
    pixels_to_fibers =  mapit()
    pins_to_count    =  struck(f,event)
    tracks           =  fibsss(f,event)
    rows             =  rowmaker()
    
    print pins_to_count
    print tracks
    
    module = fillhisto(module,
                       pins_to_pixels,
                       pixels_to_fibers,
                       pins_to_count,
                       rows,
                       tracks)
    
    c1.cd()
    title=fixer.fix(module,"Module 1 - Event %d" % int(event))
    module.GetYaxis().SetNdivisions(4)
    module.GetXaxis().SetNdivisions(8)
    module.GetYaxis().SetTitleOffset(0.6)
    
    
    module.Draw("COLZ")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()

    
    
    
    #this is for the line display
    c2     = TCanvas("c2","c2",800,500)
    
    slope  = get_slope(f,event)
    yinter = get_inter(f,event,slope)
    fit_function = TF1("fit_function","%f*x + %f" % (slope,yinter),0,70)

    all_coords = get_allcoords()
    hit_coords = get_hitcoords(f,event)
    tg_all     = TGraphErrors()
    tg_hit     = TGraphErrors()
    tmulti     = TMultiGraph()

    for i in xrange(len(hit_coords)):
        tg_hit.SetPoint(i,hit_coords[i][0],hit_coords[i][1])
    for i in xrange(len(all_coords)):
        tg_all.SetPoint(i,all_coords[i][0],all_coords[i][1])
        
    tg_hit.SetMarkerStyle(8)
    tg_hit.SetMarkerColor(2)
    tg_all.SetMarkerStyle(8)
    
    
    
    
    tmulti.Add(tg_all)
    tmulti.Add(tg_hit)
    tmulti.Draw("AP")
    c2.Update()
    c2.Modified()
    tmulti.GetYaxis().SetRangeUser(0,13)
    title_tg=fixer.fix(tmulti,"Module 1 Fit - Event %d" % int(event))
    tg.SetTitle(";x (cm); y (cm)")
    title_tg.Draw("SAMES")

    fit_function.Draw("SAMES")
    c2.Update()
    c2.Modified()
    #fit_function.Draw()
    

    

    
    
    
    f.Close()
    sys.stdin.readline()
   
   
if __name__ == '__main__' :
    main()
    

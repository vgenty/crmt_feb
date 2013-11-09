#!/usr/bin/env python

from ROOT import TH2D, TCanvas, TLatex, TF1, TFile, TGraphErrors, TMultiGraph
from ROOT import gROOT
from display_methods import *
from fit_methods import *
from fx_methods import *
from other_methods import *
import sys
import math
      
def main():
    gROOT.SetStyle("DStyle")
    f = TFile("recodata.root","READ")
    event = sys.argv[1]
    
    #this is for the event display
    #c1     = TCanvas("c1","c1",800,500)
    c1     = TCanvas("c1","c1")
    module = TH2D("mod",";x #rightarrow;y #rightarrow",64,0,64,4,0,4)
    module.GetXaxis().CenterTitle()
    module.GetYaxis().CenterTitle()
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
    title=GetTitle("Module 1 - Event %d" % int(event))
    module.GetYaxis().SetNdivisions(4)
    module.GetXaxis().SetNdivisions(8)
    
    
    module.Draw("COLZ")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()

    
    
    
    #this is for the line display
    #c2     = TCanvas("c2","c2",800,500)
    c2     = TCanvas("c2","c2")
    c2.cd()
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
    tg_hit.SetMarkerSize(1.2)
    tg_all.SetMarkerStyle(8)
    
    
    
    
    tmulti.Add(tg_all)
    tmulti.Add(tg_hit)
    tmulti.Draw("AP")
    tmulti.SetTitle(";x (cm); y (cm)")
    tmulti.GetXaxis().CenterTitle()
    tmulti.GetYaxis().CenterTitle()
    c2.Update()
    c2.Modified()
    tmulti.GetYaxis().SetRangeUser(0,13)
    title_tg=GetTitle("Module 1 Fit - Event %d" % int(event))
    title_tg.Draw("SAMES")

    fit_function.Draw("SAMES")
    c2.Update()
    c2.Modified()
    #fit_function.Draw()
    

    

    
    c3 = TCanvas("c3","c3")
    c3.cd()
    
    
    fx_slope  = get_fx_slope(f,event) #these are in specific order
    fx_yinter = get_fx_yinter(f,event) #these are in specific order
    fx_zvalue = get_fx_zvalue(f,event) #these are in specific order

    num = len(fx_slope)
    
    
    sorted_fx_slope  = sorted( fx_slope  )
    sorted_fx_yinter = sorted( fx_yinter )
    
    xlow  = sorted_fx_slope[0]
    xhigh = sorted_fx_slope[num - 1]
 
    ylow  = sorted_fx_yinter[0]
    yhigh = sorted_fx_yinter[num - 1]
 
    #look for different values
    #xstat = math.fabs(( sorted_fx_slope[num-1]  - sorted_fx_slope[0]  ))/10
    #ystat = math.fabs(( sorted_fx_yinter[num-1] - sorted_fx_yinter[0] ))/4

    #ok is there going to be a conversion issue here? im scared

    h3 = TH2D("Xvalue",";Slope;Y-inter",
              100,xlow,xhigh,
              40,ylow,yhigh)
    
    
    for x in xrange(num):
     print "(" + str(fx_slope[x]) + "," + str(fx_yinter[x]) + "," + str(fx_zvalue[x])

       
    for x in xrange(num):
        h3.Fill(fx_slope[x],fx_yinter[x],fx_zvalue[x])
    
    print h3.GetMinimum() 
    '''
    minslope= 0
    miny    = 0
    minnull = 0
    
    h3.GetMinimumBin(minslope,miny,minnull)
    
    print h3.GetXaxis().GetBinCenter(minslope)
    print h3.GetYaxis().GetBinCenter(miny)
    '''
    h3.GetXaxis().CenterTitle()
    h3.GetYaxis().CenterTitle()
    h3.Draw("COLZ")

    sys.stdin.readline()
    f.Close()
    
    
   
   
if __name__ == '__main__' :
    main()
    

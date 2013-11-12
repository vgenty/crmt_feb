#!/usr/bin/env python

from ROOT import TH2D, TCanvas, TLatex, TF1, TFile
from ROOT import gROOT, gStyle, TH1D, TGraphErrors, TMultiGraph
from methods.display_methods import *
from methods.fit_methods import *
from methods.fx_methods import *
from methods.other_methods import *
import sys
import math
      
def main():
    gROOT.SetStyle("DStyle")
    gStyle.SetOptStat(0)
    f = TFile("../output/recodata.root","READ")
    event = sys.argv[1]
    
    #this is for the event display
    #c1     = TCanvas("c1","c1",800,500)
    c1     = TCanvas("c1","c1")
    module = TH2D("mod",";x #rightarrow;y #rightarrow",64,0,64,4,0,4)
    module.GetXaxis().CenterTitle()
    module.GetYaxis().CenterTitle()
    pins_to_pixels   =  pins("../input/pins.csv")
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
    title=GetTitle("Reco Event Display - Event %d" % int(event))
    module.GetYaxis().SetNdivisions(4)
    module.GetXaxis().SetNdivisions(8)
    
    
    module.Draw("COLZ")
    #module.Draw("LEGO")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()

        
    
    #this is for the TRUE line display
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
    title_tg=GetTitle("Reco Fit - Event %d" % int(event))
    title_tg.Draw("SAMES")

    fit_function.Draw("SAMES")
    c2.Update()
    c2.Modified()
    #fit_function.Draw()
    


    c3     = TCanvas("c3","c3")
    c3.cd()
    xy_slope  = get_xyslope(f,event)
    xy_yinter = get_xyinter(f,event,slope)
    xy_fit_function = TF1("xy_fit_function","%f*x + %f" % (xy_slope,xy_yinter),0,17)

    
    xy_tg_all     = TGraphErrors()
    xy_tg_hit     = TGraphErrors()
    xy_tmulti     = TMultiGraph()
    
    for i in xrange(len(hit_coords)):
        xy_tg_hit.SetPoint(i,hit_coords[i][1],hit_coords[i][0])
    for i in xrange(len(all_coords)):
        xy_tg_all.SetPoint(i,all_coords[i][1],all_coords[i][0])

    xy_tg_hit.SetMarkerStyle(8)
    xy_tg_hit.SetMarkerColor(2)
    xy_tg_hit.SetMarkerSize(1.2)
    xy_tg_all.SetMarkerStyle(8)
    
    
    
    
    xy_tmulti.Add(xy_tg_all)
    xy_tmulti.Add(xy_tg_hit)
    xy_tmulti.Draw("AP")
    xy_tmulti.SetTitle(";y (cm); x (cm)")
    xy_tmulti.GetXaxis().CenterTitle()
    xy_tmulti.GetYaxis().CenterTitle()
    c3.Update()
    c3.Modified()
    xy_tmulti.GetYaxis().SetRangeUser(0,64)
    xy_title_tg=GetTitle("Reco Fit - Event %d" % int(event))
    xy_title_tg.Draw("SAMES")

    xy_fit_function.Draw("SAMES")
    c3.Update()
    c3.Modified()



    c4 = TCanvas("c4","c4")
    c4.cd()
    
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

    
    #ok is there going to be a conversion issue here? im scared

    #h3 = TH2D("Xvalue",";Slope #Deltax/#Deltay;X-inter",
    h3 = TH2D("Xinter",";tan#theta;X-Intercept",
              500,xlow,xhigh,
              80,ylow,yhigh)
    
    title_ps=GetTitle("Reco PS - Event %d" % int(event))
    #for x in xrange(num):
    # print "(" + str(fx_slope[x]) + "," + str(fx_yinter[x]) + "," + str(fx_zvalue[x])

  
    low_values = get_fx_lowvals(f,event)
    
    for x in xrange(num):
        h3.Fill(fx_slope[x],fx_yinter[x],fx_zvalue[x])
    
        
    h3_low = TH2D("Xinter_low",";tan#theta;X-Intercept",
              500,xlow,xhigh,
              80,ylow,yhigh)

    h3_low.Fill(low_values[1],low_values[2],low_values[0])
    h3_low.SetMarkerStyle(8)

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
    #h3.Draw("CONT4Z")
    h3.Draw("COLZ")
    h3_low.Draw("SAMES")
    title_ps.Draw("SAMES")
    #c4.SetLogz()
    c4.Update()
    c4.Modified()

    sys.stdin.readline()
    f.Close()    
    
    
    
if __name__ == '__main__' :
    main()
    

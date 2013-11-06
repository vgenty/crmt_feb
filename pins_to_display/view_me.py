from ROOT import TH2D, TCanvas, TLatex, gROOT
from viewer_methods import *
from other_methods import *
import sys

def main():
    gROOT.SetStyle("DStyle")
    c1     = TCanvas("c1","c1")
    module = TH2D("mod",";x #rightarrow;y #rightarrow",64,0,64,4,0,4)
    
    pins_to_pixels   =  pins("pins.csv")
    pixels_to_fibers =  mapit()
    pins_to_count    =  struck("vics_list.txt",sys.argv[1])
    rows             =  rowmaker()
    
    
    module = fillhisto(module,
                       pins_to_pixels,
                       pixels_to_fibers,
                       pins_to_count,
                       rows)
    c1.cd()
    title=GetTitle("Module")
    module.GetYaxis().SetNdivisions(4)
    module.GetXaxis().SetNdivisions(8)
    module.GetYaxis().CenterTitle()
    module.GetXaxis().CenterTitle()
    module.Draw("COLZ")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
    sys.stdin.readline()
   
   
if __name__ == '__main__' :
    main()
    

from ROOT import TH2D, TCanvas, TLatex
from FixROOT import OneFix
from viewer_methods import *
import sys
      
def main():
    fixer=OneFix()
    c1     = TCanvas("c1","c1",800,500)
    module = TH2D("mod",";x #rightarrow;y #rightarrow",64,0,64,4,0,4)
    
    pins_to_pixels   =  pins("pins.csv")
    pixels_to_fibers =  mapit()
    pins_to_count    =  struck("recodata.root",sys.argv[1])
    tracks           =  fibsss("recodata.root",sys.argv[1])
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
    title=fixer.fix(module,"Module 1 - Event %d" % int(sys.argv[1]))
    module.GetYaxis().SetNdivisions(4)
    module.GetXaxis().SetNdivisions(8)
    module.GetYaxis().SetTitleOffset(0.6)
    
    
    module.Draw("COLZ")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
    sys.stdin.readline()
   
   
if __name__ == '__main__' :
    main()
    

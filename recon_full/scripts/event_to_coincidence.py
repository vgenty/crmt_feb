import ROOT
from ROOT import TFile, TTree
import sys

def main():
    tfile = TFile(sys.argv[1],"READ")
    event = sys.argv[2]
    
    tree = tfile.Get("SimulationTree")
    
    print "Have NEntries: "+ str(tree.GetEntries())
    
    ccounter = -1;
    for evt in xrange(tree.GetEntries()):
        tree.GetEntry(evt)
        if (tree.Coincidence):
            ccounter+=1
            print "evt: " + str(evt) + " ccounter: " + str(ccounter)
    print "ccounter: " + str(ccounter)

    
    
    
    
if __name__ == '__main__':
    main()

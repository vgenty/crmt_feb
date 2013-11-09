from ROOT import TFile
import csv

def get_slope(tfile,event):
    
    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))
    slope = tree.Slope
    return float(1.0/(float(slope)))

def get_inter(tfile,event,slope) :
    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))
    yinter = tree.Yinter
    
    return float((-1.0 * yinter)*(slope))

def get_xyslope(tfile,event):
    
    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))
    return tree.Slope
    

def get_xyinter(tfile,event,slope) :
    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))
    return tree.Yinter

def get_hitcoords(tfile,event):
    coords=[]

    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))

    for xx, yy in zip(tree.fFibX, tree.fFibY):
        coords.append([xx,yy])
        
    return coords

def get_allcoords() :
    
    filename = 'geometry.dat'
    coords   =[]
    
    with open(filename, 'rb') as f:
        reader = csv.reader(f)
        for row in reader:
            coords.append([float(row[0]),float(row[1])])
            
    return coords

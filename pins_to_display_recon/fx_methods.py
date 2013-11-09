from ROOT import TFile

def get_fx_slope(tfile,event)  :
    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))
    
    local_slope = []

    for entry in tree.fx_Slope:
        local_slope.append(entry)
        
    return local_slope
        
def get_fx_yinter(tfile,event) :
    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))
    
    local_yinter = []
    
    for entry in tree.fx_Yinter:
        local_yinter.append(entry)    

    return local_yinter

def get_fx_zvalue(tfile,event) :

    tree = tfile.Get("Event Tree")
    tree.GetEntry(int(event))
    
    local_zvalue = []
    
    for entry in tree.fx_Zvalue:
        local_zvalue.append(entry)    

    return local_zvalue

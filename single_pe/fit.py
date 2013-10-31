from ROOT import TF1, TH1D, TCanvas
import numpy as np
import sys, csv
from FixROOT import OneFix

def open_file(filename):
    
    print 'opening file: ' + str(filename)
    content=[]

    with open(filename, 'rb') as f:
        reader = csv.reader(f)
        for row in reader:
            content.append(row[1])
            
    return content
            
    
def factorial(n):
    x=1
    for j in reversed(xrange(n+1)):
        if(j != 0): x *= j

    return x

def poisson(l,k):
    return (l**k)*(np.exp(-l))/float(factorial(k))

'''    
def fit(x,p):
    first  = (1-p[0])*(p[1]/(np.sqrt(2*np.pi)*p[2]))*np.exp(-((x-p[3])/p[2])**2)
    second = 0.0
    for n in xrange(2,11):
        second += (poisson(n-1)*p[6]/float(n))*50000.0/(np.sqrt(2.0*np.pi)*np.sqrt(float(n))*p[1])*np.exp(-1.0*(x-p[0]/p[7]-float(n)*(p[5]-p[3])+p[3])**2/(4.0*float(n)*(p[4]**2-p[2]**2)))
    
    third += p[2]*p[0]*p[7]*np.exp(-p[7]*(x-p[3]))
                                 
    return first + second + third
'''    
    
def the_fit(x,par):

    '''
    w       = par[0]
    sigma_0 = par[1]
    Q_0     = par[2]
    alparha   = par[3]
    mu      = par[4]
    sigma_1 = par[5]
    Q_sh    = par[6]
    Q_1     = par[7]
    '''

    first  = ((1-par[0])*(1/(np.sqrt(2*np.pi)*par[1]))*np.exp(-((x[0]-par[2])**2/(2*par[1])**2))+par[3]*np.exp(-1.0*par[3]*(x[0]-par[2])))*np.exp(-par[4])
    
    second=0.0
#    for n in xrange(1,11):
#        second = second + ((par[4]**n)*np.exp(-par[4])/factorial(n))*1/(np.sqrt(2*np.pi*n)*par[5])*np.exp(-1.0*(x[0]-par[2]-par[6]-n*par[7])**2/(2*n*par[5]**2))
    
    
    tot = first + second
    return tot

def main():
    

    #print fit(2,p)
    bins = 2000
    
    c1=TCanvas("c1","c1")
    c1.cd()
    h1 = TH1D("h1",";ADC;Counts",bins,.735,1.264)
    fixer=OneFix()
    title=fixer.fix(h1,"Charge Spectrum")
    
    content = open_file("data.dat")
    print 'Got %d values from file' % len(content)
    print 'Have %d bins in histogram' % bins
    for _bin in xrange(bins):
        h1.SetBinContent(_bin,float(content[bins-_bin-1]))
        
    h1.Rebin(10)
        
#    function = TF1("function",the_fit,0.735,1.264,8)
#    function.SetParameters(1,1,1,1,1,1,1,1)
    
#    h1.Fit(function,'V')

    h1.Draw()
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
    
    
    c2=TCanvas("c2","c2")
    c2.cd()
    h2 = TH1D("h2",";ADC;Counts",bins/10,.735,1.264)

    function = TF1("function",the_fit,0.735,1.264,8)
    
    '''
    w       = par[0]
    sigma_0 = par[1]
    Q_0     = par[2]
    alparha   = par[3]
    mu      = par[4]
    sigma_1 = par[5]
    Q_sh    = par[6]
    Q_1     = par[7]
    '''
    
    function.SetParameters(0,0.01,0.75,0,1,0.5,0,1.1)
    
    for j in xrange(1000) :
        h2.Fill(function.GetRandom())    
    
    title2=fixer.fix(h2,"What?")
    
    h2.Draw()
    title2.Draw("SAMES")
    
    c2.Update()
    c2.Modified()
    
    sys.stdin.readline()
    
    
if __name__ == '__main__' :
    main()
    


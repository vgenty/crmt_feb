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
    
def fit(x,p):

    '''
    w       = p[0]
    sigma_0 = p[1]
    Q_0     = p[2]
    alpha   = p[3]
    mu      = p[4]
    sigma_1 = p[5]
    Q_sh    = p[6]
    Q_1     = p[7]
    '''

    first  = ((1-p[0])*(1/(np.sqrt(2*np.pi)*p[1]))*np.exp(-((x-p[2])**2/2*p[1]**2))+p[3]*np.exp(-1.0*p[3]*(x-p[2])))*np.exp(-p[4])
    
    second=0.0
    for n in xrange(1,11):
        second += ((p[4]**n)*np.exp(-p[4])/factorial(n))*1/(np.sqrt(2*np.pi*n)*p[5])*np.exp(-1.0*(x-p[2]-p[6]-n*p[7])**2/(2*n*p[5]**2))
    
        return first + second

def main():
    
    print factorial(5)
    
    
    #p=[1,1,1,1,1,1,1,1]
    #print fit(2,p)
    
    bins = 2000
    
    h1 = TH1D("h1",";ADC;Counts",bins,.735,1.264)
    fixer=OneFix()
    title=fixer.fix(h1,"Charge Spectrum")
    
    content = open_file("data.dat")
    print 'Got %d values from file' % len(content)
    print 'Have %d bins in histogram' % bins
    for _bin in xrange(bins):
        h1.SetBinContent(_bin,float(content[bins-_bin-1]))
    
    h1.Rebin(10)
    h1.Draw()
    title.Draw("SAMES")
    sys.stdin.readline()
    
    
if __name__ == '__main__' :
    main()
    


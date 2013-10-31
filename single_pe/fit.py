from ROOT import TF1, TH1D, TCanvas, TMath
import sys, csv
from FixROOT import OneFix

def open_file(filename):
    
    print 'opening file: ' + str(filename)
    content=[]
    
    value=[]
    adc=[]

    with open(filename, 'rb') as f:
        reader = csv.reader(f)
        for row in reader:
            adc.append(row[0])
            value.append(row[1])
            
    return [adc,value]
            
def heaviside(x,q):
    if x > q:
        return 0.0
    else:
        return 1.0
    
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
    first    = 0.0
    long_exp = 0.0
    long_exp = ((1.0-par[0])*(1/(TMath.Sqrt(2*TMath.Pi())*par[1]))*TMath.Exp(-((x[0]-par[2])**2/(2*(par[1])**2))))
    outside= 0.0
    outside= TMath.Exp(-par[4]) 
    second = 0.0
    inside = 0.0
    inside = par[0]*heaviside(x,par[2])
    expp   = 0.0
    expp   = par[3]*TMath.Exp(-1.0*par[3]*(x[0]-par[2]))
    first  = (long_exp + inside*expp )*outside*par[9]
    #first  = (long_exp)*outside

    #for n in xrange(1,3):
    #((par[4]**n)*TMath.Exp(-par[4])/TMath.Factorial(n))
    second=0.0
    for n in xrange(1,3):
        second = second + par[8]*TMath.Poisson(n,par[4])*1/(TMath.Sqrt(2*TMath.Pi()*n)*par[5])*TMath.Exp(-1.0*(x[0]-par[2]-par[0]/par[3]-n*par[7])**2/(2*n*par[5]**2))
        #second = second + 1/(TMath.Sqrt(2*TMath.Pi()*n)*par[5])*TMath.Exp(-1.0*(x[0]-par[2]-par[0]/par[3]-n*par[7])**2/(2*n*par[5]**2))
    #second = par[0]*TMath.Gaus(x[0],par[2],TMath.Sqrt(1)*par[5])

    #    tot = first + second
    tot = first + second
    return tot
    #return par[1]*TMath.Poisson(x[0],par[4])

def main():
    c1=TCanvas("c1","c1")
    c1.cd()

    content = open_file("data_2.dat")    
    bins = 2000
    xlow  = float(content[0][0])*10**12+30
    xhigh = float(content[0][bins-1])*10**12+30
    h1 = TH1D("h1",";ADC;Counts",bins,xlow,xhigh)
    fixer=OneFix()
    title=fixer.fix(h1,"Charge Spectrum")

    print 'Got %d values from file' % len(content[1])
    print 'Have %d bins in histogram' % bins
    for _bin in xrange(bins):
        h1.SetBinContent(_bin,float(content[1][bins-_bin-1]))

    h1.Rebin(10)
    #function = TF1("function",the_fit,xlow,xhigh,8)
    #function.SetParameters(0.2,1,7,0.35,1.00,2,1,25.05)        

    #h1.Fit(function,'V')

    h1.Draw()
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
        
    
    c2=TCanvas("c2","c2")
    c2.cd()
    h2 = TH1D("h2",";ADC;Counts",bins/10,0,300)

    function = TF1("function",the_fit,0,300,10)
    
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
    
    function.SetParameters(0.3,5,23.26,0.035,1.68,11.73,50,35.05,1.,1.0)
    
    for j in xrange(10000) :
        h2.Fill(function.GetRandom())    
    
    function.SetParameter(8, 200)
    function.SetParameter(9, 200)
        
    title2=fixer.fix(h2,"What?")
    #for i in xrange(0,8):
    #    function.SetParLimits(i,0,10*function.GetParameter(i))


    h2.Fit("function","V")
    h2.Draw()
    title2.Draw("SAMES")
    
    c2.Update()
    c2.Modified()
    '''
    c3=TCanvas("c3","c3")
    c3.cd()
    h3 = TH1D("h3",";ADC;Counts",bins/10,xlow,xhigh)
    title3=fixer.fix(h3,"Testing")
    
    function2 = TF1("function2",the_fit,xlow,xhigh,8)
    function2.SetParameters(0.2,1,7,0.0,1.00,2,1,25.05)
    #function2.SetParameters(0.2,1.0,6.0,0.035,1.0,2,0,25.05)
    
    for j in xrange(10000) :
        h3.Fill(function2.GetRandom())        
        
    h3.Fit("function2","V")
    h3.Draw()
    title3.Draw("SAMES")
    c3.Update()
    c3.Modified()
    '''
    sys.stdin.readline()
    
    
if __name__ == '__main__' :
    main()
    


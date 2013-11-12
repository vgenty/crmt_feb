from ROOT import TMath
import csv

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
    first  = 0.0
    second = 0.0 
    tot    = 0.0

    #Var Names
    '''
    w       = par[0]
    sigma_0 = par[1]
    Q_0     = par[2]
    alpha   = par[3]
    mu      = par[4]
    sigma_1 = par[5]
    Q_sh    = par[6] <--------currently not in use
    Q_1     = par[7]
    char_amp= par[8]
    ped_amp = par[9]
    '''
    #Wow this is painful to read
    first = par[9]*((1.0-par[0])*(1/(TMath.Sqrt(2*TMath.Pi())*par[1]))*TMath.Exp(-((x[0]-par[2])**2/(2*(par[1])**2))) + par[0]*heaviside(x,par[2])*par[3]*TMath.Exp(-1.0*par[3]*(x[0]-par[2]))) *TMath.Exp(-par[4])

    #range 1->x peaks in pe charge spectrum, this is very slow
    for n in xrange(1,10):
        second = second + par[8]*TMath.Poisson(n,par[4])*1/(TMath.Sqrt(2*TMath.Pi()*n)*par[5])*TMath.Exp(-1.0*(x[0]-par[2]-par[0]/par[3]-n*par[7])**2/(2*n*par[5]**2))
        
    tot = first + second
    return tot

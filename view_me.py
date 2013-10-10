from ROOT import TH2D, TCanvas
from FixROOT import OneFix
import csv
import sys

def pins(filename) :
    print 'opening file: ' + str(filename)
    pinstopixels=[]

    with open(filename, 'rb') as f:
        reader = csv.reader(f)
        for row in reader:
            data=[int(row[0]),int(row[1])]
            pinstopixels.append(data)  
    return pinstopixels


def mapit() :
    mapping=[]
    data=[]
    r=128
    cnt=1
    for j in xrange(r) :
        data.append(j+1)
        if (j+1)%8 == 0 :
            mapping.append(data)        
            data=[]
    return mapping

def struck(filename) :
    print 'opening file: ' + str(filename)
    pinstocounts=[]

    with open(filename, 'rb') as f:
        reader = csv.reader(f)
        for row in reader:
            data=[int(row[0]),int(row[1])]
            pinstocounts.append(data)  
    return pinstocounts
        
def rowmaker() :
#identifier row
   row1=[]
   for x in xrange(25,25+8)   : row1.append(x)
   for x in xrange(49,49+8)   : row1.append(x)
   for x in xrange(89,89+8)   : row1.append(x)
   for x in xrange(113,113+8) : row1.append(x)
   for x in xrange(17,17+8)   : row1.append(x)
   for x in xrange(57,57+8)   : row1.append(x)
   for x in xrange(81,81+8)   : row1.append(x)
   for x in xrange(121,121+8) : row1.append(x)

#Bottom 3 rows   
   seed2=[9,33,73,97,1,41,65,105]
   seed3=[104,80,40,16,112,72,48,8]
   seed4=[120,96,56,32,128,88,64,24]

   row2=[]
   row3=[]
   row4=[]
   
   for x in xrange(8) :
       for y in seed2:
           row2.append(x+y)
       for y in seed3:
           row3.append(y-x)
       for y in seed4:
           row4.append(y-x)

   return [row1,row2,row3,row4]
   print pixels_to_fibers
   
   print pints_to_counts

   


def fillhisto(module,pins_to_pixels,pixels_to_fibers,pins_to_count,rows):
   hitpixels_top = []
   hitpixels_bot = []
   
   for hitpin in pins_to_count :
       hitpinid = hitpin[0]
       hitcount = hitpin[1]
       for pin in pins_to_pixels :
           pinid   = pin[0]
           pixelid = pin[1]
           if hitpinid == pinid and pinid < 32 :
               hitpixels_top.append([pixelid,hitcount])
           if hitpinid == pinid and pinid > 32 :
               hitpixels_bot.append([pixelid,hitcount])
               
   hitfibers_top = []
   hitfibers_bot = []
   
   print pixels_to_fibers
   for pixel in hitpixels_top:
       hitpixel = pixel[0]
       hitcount = pixel[1]
       for fib in pixels_to_fibers[hitpixel-1]:
           hitfibers_top.append(fib)

   for pixel in hitpixels_bot:
       hitpixel = pixel[0]
       hitcount = pixel[1]
       for fib in pixels_to_fibers[hitpixel-1]:
           hitfibers_bot.append(fib)
       
         
   for fib in rows[0] :
       if fib in hitfibers_top :
           module.SetBinContent(rows[0].index(fib)+1,4,1)
   for fib in rows[1] :
       if fib in hitfibers_top :
           module.SetBinContent(rows[1].index(fib)+1,3,1)
   for fib in rows[2] :
       if fib in hitfibers_bot :
           module.SetBinContent(rows[2].index(fib)+1,2,1)
   for fib in rows[3] :
       if fib in hitfibers_bot :
           module.SetBinContent(rows[3].index(fib)+1,1,1)

   return module
      
def main():
    fixer=OneFix()
    c1     = TCanvas("c1","c1",1000,500)
    module = TH2D("mod",";x;y",64,0,64,4,0,4)
    
    pins_to_pixels   =  pins("pins.csv")
    pixels_to_fibers =  mapit()
    pins_to_count    =  struck("struck.csv")
    rows             =  rowmaker()
    
    
    module = fillhisto(module,
                       pins_to_pixels,
                       pixels_to_fibers,
                       pins_to_count,
                       rows)
    c1.cd()
    title=fixer.fix(module,"Module")
    module.Draw("COLZ")
    title.Draw("SAMES")
    c1.Update()
    c1.Modified()
    sys.stdin.readline()
   
   
if __name__ == '__main__' :
    main()
    

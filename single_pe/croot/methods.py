from ROOT import TPaveText

def GetTitle(words):

    fTitle = TPaveText(0.1412338,0.9321267,0.4399351,0.9886878,"brNDC")
    fTitleWords = words
    fTitle.SetTextFont(63)
    fTitle.SetTextSize(24)
    fTitle.SetBorderSize(0)
    fTitle.SetFillColor(0)
    fTitle.AddText(words)
    
    return fTitle

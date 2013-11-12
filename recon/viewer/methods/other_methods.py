from ROOT import TPaveText

def GetTitle(words):
    fTitle = TPaveText(.5,.8,.8,.9,"brNDC")
    fTitleWords = words
    fTitle.SetTextFont(63)
    fTitle.SetTextSize(24)
    fTitle.SetBorderSize(0)
    fTitle.SetFillColor(0)
    fTitle.AddText(words)
    
    return fTitle

#! /usr/bin/env python

import ROOT,copy
from ROOT import gSystem, TCanvas, TH1,  TPad, gStyle, TLegend
from collections import OrderedDict
from optparse import OptionParser
import sys
import math
import operator


def GetTypeofsamples(category):


    signalZZ_qq = [{"sample":'ZZTo2e2mu',"color":ROOT.kAzure-4,"name":'qq/qg #rightarrow ZZ(+jets)'},{"sample":'ZZTo2e2muJJ_SMHContinInterf_H125.6',"color":ROOT.kAzure-4,"name":'qq/qg #rightarrow ZZ(+jets)'},{"sample":'ZZTo4eJJ_SMHContinInterf_H125.6',"color":ROOT.kAzure-4,"name":'qq/qg #rightarrow ZZ(+jets)'},{"sample":'ZZTo4e',"color":ROOT.kAzure-4,"name":'qq/qg #rightarrow ZZ(+jets)'},{"sample":'ZZTo4mu',"color":ROOT.kAzure-4,"name":'qq/qg #rightarrow ZZ(+jets)'},{"sample":'ZZTo4muJJ_SMHContinInterf_H125.6',"color":ROOT.kAzure-4,"name":'qq/qg #rightarrow ZZ(+jets)'}]
    
    signalZZ_gg=[{"sample":'ggTo2e2mu_SMHContinInterf-MCFM67_H125.6',"color":ROOT.kAzure-5,"name":'gg #rightarrow ZZ(+jets)'},{"sample":'ggTo4e_SMHContinInterf-MCFM67_H125.6',"color":ROOT.kAzure-5,"name":'gg #rightarrow ZZ(+jets)'},{"sample":'ggTo4mu_SMHContinInterf-MCFM67_H125.6',"color":ROOT.kAzure-5,"name":'gg #rightarrow ZZ(+jets)'}]

    signalOther = [{"sample":'ZZZJets',"color":ROOT.kAzure-6,"name":'Other ZZ processes'},{"sample":'WZZJets',"color":ROOT.kAzure-6,"name":'Other ZZ processes'},{"sample":'WH126',"color":ROOT.kAzure-6,"name":'Other ZZ processes'},{"sample":'ZH126',"color":ROOT.kAzure-6,"name":'Other ZZ processes'},{"sample":'ttH126',"color":ROOT.kAzure-6,"name":'Other ZZ processes'}] 

    signal_tot = [{"sample":'ZZTo2e2mu',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ZZTo2e2muJJ_SMHContinInterf_H125.6',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ZZTo4eJJ_SMHContinInterf_H125.6',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ZZTo4e',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ZZTo4mu',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ZZTo4muJJ_SMHContinInterf_H125.6',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ggTo2e2mu_SMHContinInterf-MCFM67_H125.6',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ggTo4e_SMHContinInterf-MCFM67_H125.6',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ggTo4mu_SMHContinInterf-MCFM67_H125.6',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ZZZJets',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'WZZJets',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'WH126',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ZH126',"color":ROOT.kAzure-4,"name":'ZZ processes'},{"sample":'ttH126',"color":ROOT.kAzure-4,"name":'ZZ processes'}]


    signal = signalZZ_qq + signalZZ_gg + signalOther

    #background = [{"sample":'WWWJets',"color":ROOT.kOrange,"name":'Triboson'},{"sample":'WZ',"color":ROOT.kMagenta-8,"name":'WZ'},{"sample":'TTTo2L2Nu2B',"color":ROOT.kGreen-5,"name":'tt'},{"sample":'TTZJets',"color":ROOT.kGreen-5,"name":'tt'},{"sample":'TTWJets',"color":ROOT.kGreen-5,"name":'tt'},{"sample":'TTWWJets',"color":ROOT.kGreen-5,"name":'tt'},{"sample":'TTGJets',"color":ROOT.kGreen-5,"name":'tt'},{"sample":'DYJetsToLLTuneZ2M50',"color":ROOT.kOrange+1,"name":'DY'},{"sample":'DYJetsToLLTuneZ2M10',"color":ROOT.kOrange+1,"name":'DY'}]

    background_red = [{"sample":'WWWJets',"color":ROOT.kBlue-9,"name":'Triboson'},{"sample":'WZ',"color":ROOT.kOrange-4,"name":'WZ'},{"sample":'TTTo2L2Nu2B',"color":ROOT.kRed-2,"name":'tt'},{"sample":'TTWJets',"color":ROOT.kRed-2,"name":'tt'},{"sample":'TTGJets',"color":ROOT.kRed-2,"name":'tt'},{"sample":'DYJetsToLLTuneZ2M50',"color":ROOT.kGreen-5,"name":'DY'},{"sample":'DYJetsToLLTuneZ2M10',"color":ROOT.kGreen-5,"name":'DY'}]

    irred_background_tot = [{"sample":'WWZJets',"color":ROOT.kOrange-3,"name":'Irreducible background'},{"sample":'TTWWJets',"color":ROOT.kOrange-3,"name":'Irreducible background'},{"sample":'TTZJets',"color":ROOT.kOrange-3,"name":'Irreducible background'}]

    irred_background_div = [{"sample":'WWZJets',"color":ROOT.kOrange+2,"name":'WWZ'},{"sample":'TTZJets',"color":ROOT.kOrange-4,"name":'TTZ'},{"sample":'TTWWJets',"color":ROOT.kGreen-7,"name":'TTWW'}]

    background = irred_background_div +  background_red

    SigBkg = signal_tot+irred_background_div +  background_red

    SigIrredBkg_tot=irred_background_tot+signal

    SigIrredBkg_div=irred_background_div+signal

    SigIrredBkg_tot_RedBkg = irred_background_tot+signal+background_red

    SigTotIrredBkg_tot_RedBkg = irred_background_tot+signal_tot+background_red
     
    data = [{"sample":'DoubleMu',"color":ROOT.kBlack,"name":'Data'},{"sample":'DoubleEle',"color":ROOT.kBlack,"name":'Data'},{"sample":'MuEG',"color":ROOT.kBlack,"name":'Data'}]
 
    typeofsamples = []
    
    if category=="sig":
        typeofsamples = signal

    if category=="sig_qq":
        typeofsamples = signalZZ_qq

    if category=="sig_gg":
        typeofsamples = signalZZ_gg

    if category=="sig_tot":
        typeofsamples = signal_tot
    
    if category=="bkg":
        typeofsamples = background

    if category=="sig+bkg":
        typeofsamples =SigBkg

    if category=="red_bkg":
        typeofsamples = background_red

    if category=="irred_bkg_tot":
        typeofsamples = irred_background_tot
        
    if category=="irred_bkg_div":
        typeofsamples = irred_background_div

    if category=="sig+irred_bkg_tot":
        typeofsamples = SigIrredBkg_tot
    
    if category=="sig+irred_bkg_div":
        typeofsamples = SigIrredBkg_div

    if category=="sig+irred_bkg_tot+red_bkg":
        typeofsamples = SigIrredBkg_tot_RedBkg

    if category=="sigTot+irred_bkg_tot+red_bkg":
        typeofsamples = SigTotIrredBkg_tot_RedBkg
 
    if category=="data":
        typeofsamples = data
    
    return copy.deepcopy(typeofsamples)

#######################################################

def GetInputdir(region):

    if region=="SR":
        inputdir="../results/ZZjAnalyzer_SR/"
    
    if region=="SR_compare":
        inputdir="../results/ZZjAnalyzer_SR/"
    
    if region=="CR2P2F":
        inputdir="../results/ZZjAnalyzer_CR2P2F/"
 
    if region=="CR3P1F": 
        inputdir="../results/ZZjAnalyzer_CR3P1F/"

    return copy.deepcopy(inputdir)

#######################################################

def GetDataPlot(region):

    datafiles={}
    
    typeofsamples=GetTypeofsamples("data")

    inputdir=GetInputdir(region)

    for d in typeofsamples:
        datafiles[d["sample"]] = ROOT.TFile(inputdir+d["sample"]+".root")
    
    print "\n","######### Data #########\n"

    hdata=ROOT.TH1F()
    hvar=ROOT.TH1F()
    isFirst=1
    for d in typeofsamples:
        h = datafiles[d["sample"]].Get("ZZMass")
        if h==None:
            print d["sample"]," has no enetries or is a zombie" 
            continue
        if isFirst:
            if region=="CR2P2F":
                print d["sample"], "..........................", -h.Integral()
            else:
                print d["sample"], "..........................", h.Integral()
            hdata=copy.deepcopy(h)
            isFirst=0
            continue
        if region=="CR2P2F":
            print d["sample"], "..........................", -h.Integral()
        else:
            print d["sample"], "..........................", h.Integral()
        hdata.Add(h)

    if region=="CR2P2F":
        hdata.Add(hdata,-2)     

    hdata.SetMarkerStyle(21)
    hdata.SetMarkerSize(.5)
    hdata.Rebin(2) #FIX MEEEE
   
    
    return copy.deepcopy(hdata)

######################################################

def SetFakeRate(h31, h22, hvar):

    Nbins= h31.GetNbinsX()
    
    for i in range(1,Nbins):

        stat22 = h22.GetBinError(i)
        stat31 = h31.GetBinError(i)
        stat = (stat22*stat22)+(stat31*stat31)
        err = math.sqrt(hvar.GetBinContent(i)+ stat)
        h31.SetBinContent(i,h31.GetBinContent(i)+h22.GetBinContent(i))
        h31.SetBinError(i,err)      
        
    return 1
#######################################################

def GetFakeRate(region, method): 

    if method=="MC":
        typeofsamples = GetTypeofsamples("sig+bkg")

    if method =="data":
        typeofsamples = GetTypeofsamples("data")

    filesCR2P2F={}
    filesCR3P1F={}

    inputdirCR2P2F=GetInputdir("CR2P2F")
    inputdirCR3P1F=GetInputdir("CR3P1F")

    for s in typeofsamples:
        filesCR2P2F[s["sample"]] = ROOT.TFile(inputdirCR2P2F+s["sample"]+".root")  
    for s in typeofsamples:
        filesCR3P1F[s["sample"]] = ROOT.TFile(inputdirCR3P1F+s["sample"]+".root")

    print "\n","######### Contribution to fake rate #########\n"

    hFakeRate=ROOT.TH1F()
    h22Tot=ROOT.TH1F()
    hvar=ROOT.TH1F()

    isFirst=1
    for s in typeofsamples:
        h31 = filesCR3P1F[s["sample"]].Get("ZZMass")
        hv31 = filesCR3P1F[s["sample"]].Get("ZZMass_FRVar")
        if h31==None:
            print s["sample"]," has no enetries or is a zombie" 
            continue
        if isFirst:
            hFakeRate=copy.deepcopy(h31)
            hvar=copy.deepcopy(hv31)
            h22 = filesCR2P2F[s["sample"]].Get("ZZMass")
            hv22 = filesCR2P2F[s["sample"]].Get("ZZMass_FRVar")
            h22Tot=copy.deepcopy(h22) 
            isFirst=0
            print s["sample"], "..........................",(h31.Integral())+(h22.Integral())
            if h22==None:
                print s["sample"]," has no enetries or is a zombie" 
                continue
            # hFakeRate.Add(h22)
            hvar.Add(hv22)
            continue 
                      
        hFakeRate.Add(h31)
        hvar.Add(hv31)        
        h22 = filesCR2P2F[s["sample"]].Get("ZZMass")
        hv22 = filesCR2P2F[s["sample"]].Get("ZZMass_FRVar")
        if h22==None:
            print s["sample"]," has no enetries or is a zombie" 
            continue
        h22Tot.Add(h22)
        hvar.Add(hv22)
        print s["sample"], "..........................",(h31.Integral())+(h22.Integral())
    
    SetFakeRate(hFakeRate,h22Tot,hvar)
    Err=ROOT.Double(0.)
    Integr= hFakeRate.IntegralAndError(0,-1,Err)
    print "                                __________________ "
    print "\n","Reducible background----------> ",Integr," +- ",Err,"\n"

    if region != "SR_compare":    
        hFakeRate.SetFillColor(ROOT.kGray)
        hFakeRate.SetLineColor(ROOT.kBlack)
        hFakeRate.Rebin(2) #FIX MEEEEE

    else:
       
        hFakeRate.SetMarkerStyle(21)
        hFakeRate.SetMarkerSize(.5)

    return copy.deepcopy(hFakeRate)



#######################################################


def GetStackPlot(region, category):

    leg = TLegend(0.61,0.56,0.85,0.81)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.025)
    files={}
    
    typeofsamples = GetTypeofsamples(category)
    inputdir = GetInputdir(region)

    for s in typeofsamples:
        files[s["sample"]] = ROOT.TFile(inputdir+s["sample"]+".root")

    stack = ROOT.THStack("stack",region+" "+category)

# Comment this if you don't want the irreducible background (FR method) to be shown ------------------
    #if region=="SR":
    #    hfake=GetFakeRate(region,"data")
    #    stack.Add(hfake)
    #    leg.AddEntry(hfake,"Reducible background","f")

##----------------------

    print "\n","######### MonteCarlo samples #########\n"

    LastColor = ROOT.kBlack
    for s in typeofsamples:
        h = files[s["sample"]].Get("ZZMass")
        if h==None:
            print s["sample"]," has no entries or is a zombie" 
            continue
        if region=="CR2P2F":
            hcopy = copy.deepcopy(h) 
            h.Add(hcopy,-2)
        print s["sample"], "..........................", h.Integral()
           

        h.SetLineColor(s["color"])
        h.SetFillColor(s["color"])
        h.Rebin(2) # FIX MEEEEE
        stack.Add(h)
        if LastColor!=s["color"]:
            leg.AddEntry(h,s["name"],"f")
        LastColor=s["color"]
    
        
    return (copy.deepcopy(stack),copy.deepcopy(leg))

#######################################################


def GetStackPlot_fstate(region, category):

    leg = TLegend(0.61,0.56,0.85,0.81)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.025)
    files={}
    filesbkg ={}

    bkgsamples = GetTypeofsamples("irred_bkg_tot")    
    typeofsamples = GetTypeofsamples("sig") 
    inputdir = GetInputdir(region)

    for s in typeofsamples:
        files[s["sample"]] = ROOT.TFile(inputdir+s["sample"]+".root")

    for b in bkgsamples:
        filesbkg[b["sample"]] = ROOT.TFile(inputdir+b["sample"]+".root")


    hsum2e2mu = ROOT.TH1F()
    hsum4e    = ROOT.TH1F()
    hsum4mu   = ROOT.TH1F()
    hIrredBkg = ROOT.TH1F()

    isFirst=1
    for s in typeofsamples:
        h2e2mu = files[s["sample"]].Get("ZZTo2e2m_Mass")        
        if h2e2mu==None:
            print "For sample ", s["sample"], "h2e2mu has no enetries or is a zombie"       
            continue
        if isFirst:
            hsum2e2mu=copy.deepcopy(h2e2mu)            
            isFirst=0
            continue 
        hsum2e2mu.Add(h2e2mu)        

    isFirst=1
    for s in typeofsamples:
        h4e    = files[s["sample"]].Get("ZZTo4e_Mass")
        if h4e==None:
            print "For sample ", s["sample"],"h4e has no enetries or is a zombie"       
            continue
        if isFirst:
            hsum4e=copy.deepcopy(h4e)
            isFirst=0
            continue
        hsum4e.Add(h4e)
    
    isFirst=1
    for s in typeofsamples:
        h4mu   = files[s["sample"]].Get("ZZTo4m_Mass")
        if h4mu==None:
            print "For sample ", s["sample"],"h4mu has no enetries or is a zombie"       
            continue
        if isFirst:
            hsum4mu=copy.deepcopy(h4mu)
            isFirst=0
            continue
        hsum4mu.Add(h4mu)
        
        
    print "\n","Total integral 2e2mu contribution ----------> ",hsum2e2mu.Integral()
    print "\n","Total integral 4e    contribution ----------> ",hsum4e.Integral()
    print "\n","Total integral 4mu   contribution ----------> ",hsum4mu.Integral(),"\n"
    #hsum2e2mu.Rebin(2) #FIX MEEEEE
    #hsum4e.Rebin(2) #FIX MEEEEE
    #hsum4mu.Rebin(2) #FIX MEEEEE

    isFirst=1
    for b in bkgsamples:
        hbkg = filesbkg[b["sample"]].Get("ZZMass")        
        if hbkg==None:
            print b["sample"], "has no enetries or is a zombie"       
            continue
        if isFirst:
            hIrredBkg=copy.deepcopy(hbkg)
            isFirst=0
            continue
        hIrredBkg.Add(hbkg)
        print b["sample"],"..........................", hbkg.Integral()
        
    hIrredBkg.SetLineColor(b["color"])
    hIrredBkg.SetFillColor(b["color"])           
    hIrredBkg.Rebin(2)
    stack = ROOT.THStack("stack",region+" "+"Final states")
   

    if region=="SR":
        hfake=GetFakeRate(region,"data")
        stack.Add(hfake)

    stack.Add(hIrredBkg)
    
    leg.AddEntry(hfake,"Reducible background","f")
    leg.AddEntry(hIrredBkg, "Irreducible background","f")

    hsum = [{"state":hsum2e2mu,"color":ROOT.kAzure-4,"name":'2e2mu'},{"state":hsum4e,"color":ROOT.kAzure-5,"name":'4e'},{"state":hsum4mu,"color":ROOT.kAzure-6,"name":'4mu'}]

    print "\n","######### Signal samples for every final state #########\n"

    LastColor = ROOT.kBlack
    for i in hsum:
        if i["state"]==None:
            print i["state"]," has no enetries" 
            continue
    
        i["state"].SetLineColor(i["color"])
        i["state"].SetFillColor(i["color"])
        i["state"].Rebin(2) # FIX MEEEEE
       
        stack.Add(i["state"])
       
        if LastColor!=i["color"]:
            leg.AddEntry(i["state"],i["name"],"f")
        LastColor=i["color"]
    
        
    return (copy.deepcopy(stack),copy.deepcopy(leg))



#######################################################

def GetSignalDefPlot(category):

    leg = TLegend(0.61,0.56,0.85,0.81)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.025)

    typeofsamples=GetTypeofsamples(category)
    inputdir = GetInputdir("SR")
    files = {}

    for s in typeofsamples:
        files[s["sample"]] = ROOT.TFile(inputdir+s["sample"]+".root")
    
    stack = ROOT.THStack("stack","SR: signal definition for "+category)
    hSig = ROOT.TH1F()
    hNoSig = ROOT.TH1F()

    isFirst =1
    for s in typeofsamples:
        hs = files[s["sample"]].Get("PassDef")
        if hs==None:
            print "For sample ", s["sample"],"PassDef has no entries"
            continue
        if isFirst:
            hSig = copy.deepcopy(hs)
            isFirst=0
            continue
        
        hSig.Add(hs)
    print "Total events passing signal defition ", hSig.Integral()

    isFirst =1
    for s in typeofsamples:
        hn = files[s["sample"]].Get("NoPassDef")
        if hn==None:
            print "For sample ", s["sample"],"NoPassDef has no entries"
            continue
        if isFirst:
            hNoSig = copy.deepcopy(hn)
            isFirst=0
            continue
        
        hNoSig.Add(hn)
    print "Total events not passing signal defition ", hNoSig.Integral()
       
    hSig.SetFillColor(ROOT.kAzure-4)
    hSig.SetLineColor(ROOT.kAzure-4)
    hNoSig.SetFillColor(ROOT.kAzure-6)
    hNoSig.SetLineColor(ROOT.kAzure-6)      
    hSig.Rebin(2)
    hNoSig.Rebin(2)
    stack.Add(hSig)
    stack.Add(hNoSig)
    leg.AddEntry(hSig,"Pass S def","f")
    leg.AddEntry(hNoSig,"No Pass S def","f")

    return (copy.deepcopy(stack),copy.deepcopy(leg))

#######################################################


   

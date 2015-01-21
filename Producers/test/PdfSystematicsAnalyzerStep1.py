### NOTE: This is prepared to run on the newest PDFs with LHAPDF >=3.8.4
### so it requires local installation of LHAPDF libraries in order to run 
### out of the box. Otherwise, substitute the PDF sets by older sets

import FWCore.ParameterSet.Config as cms

### ----------------------------------------------------------------------
### Based on ZZ->4l strategy.
###----------------------------------------------------------------------
LEPTON_SETUP = 2012
JET_SETUP = 2012
#PD = ""
#MCFILTER = ""
ELECORRTYPE = "Paper" # "None", "Moriond", or "Paper"
ELEREGRESSION = "Paper" # "None", "Moriond", "PaperNoComb", or "Paper"
APPLYMUCORR = True
#MCFILTER = "signaldefinition"
SIGNALDEFINITION = int('1',2) # -1 means get everything, 1 means the request of having a ZZ pair with the mass in the choosedn windows. For other topology see the README under VVXAnalysis/Commons.
try:
    IsMC
except NameError:
    IsMC = True

try:
    LEPTON_SETUP
except NameError:
    LEPTON_SETUP = 2012 # define the set of effective areas, rho corrections, etc.

try:
    JET_SETUP
except NameError:
    JET_SETUP = 2012 # define the MVA for the jet PU Id

try:
    PD
except NameError:
    PD = "" # "" for MC, "DoubleEle", "DoubleMu", or "MuEG" for data

try:
    MCFILTER
except NameError:
    MCFILTER = ""

try:
    XSEC
except NameError:
    XSEC = -1



#Process name
process = cms.Process("PDFWEIGHT")
#process = cms.Process("Sequence")


# Max events
process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(-1)
    input = cms.untracked.int32(-1)
)

process.load("FWCore.MessageService.MessageLogger_cfi")

#process.TFileService=cms.Service('TFileService',
#fileName=cms.string('PDFStudies.root')
#)

infile1='/store/cmst3/user/cmgtools/CMG/ZZTo4mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_100_1_UR6.root'
infile2= '/store/cmst3/user/cmgtools/CMG/ZZTo2e2mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_100_1_irQ.root'
infile3='/store/cmst3/group/cmgtools/CMG/ZZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_10_1_UV1.root'
# Input files (on disk)
process.source = cms.Source("PoolSource",
      
                            fileNames = cms.untracked.vstring(infile2)
                            
)

process.MessageLogger.cerr.FwkReport.reportEvery = 10000

# Produce PDF weights (maximum is 3)
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
      # Fix POWHEG if buggy (this PDF set will also appear on output, 
      # so only two more PDF sets can be added in PdfSetNames if not "")
      FixPOWHEG = cms.untracked.string("CT10.LHgrid"),
      GenTag = cms.untracked.InputTag("genParticlesPruned"),
      PdfInfoTag = cms.untracked.InputTag("generator"),
      PdfSetNames = cms.untracked.vstring(
              "MSTW2008nlo68cl.LHgrid"
            , "NNPDF20_100.LHgrid"
      )
)


process.weightout = cms.OutputModule("PoolOutputModule",
                                     outputCommands = cms.untracked.vstring('drop *',
                                                                            'keep *_pdfWeights_*_*'),
                                     fileName = cms.untracked.string('PdfWeight.root')
                                     )

process.zzGenCategory = cms.EDFilter("ZZGenFilterCategory",
                                     Topology = cms.int32(1), # -1 means get everything
                                     ParticleStatus = cms.int32(1), 
                                     src            = cms.InputTag("genParticlesPruned"),
                                     GenJets        = cms.InputTag("genJetSel")
                                     )


# Collect uncertainties for rate and acceptance
#process.pdfSystematics = cms.EDFilter("PdfSystematicsAnalyzer",
      #SelectorPath = cms.untracked.string('preselection'),
    # SelectorPath = cms.untracked.string('pdfana'), 
      #PdfWeightTags = cms.untracked.VInputTag(
       #       "pdfWeights:CT10"
        #    , "pdfWeights:MSTW2008nlo68cl"
         #   , "pdfWeights:NNPDF20"
     # )
#)


# Main path
process.pdfana = cms.Sequence(process.zzGenCategory*process.pdfWeights)
#process.pdfana = cms.Sequence(process.pdfWeights)

process.weightpath = cms.Path(process.pdfana)
process.outpath = cms.EndPath(process.weightout)

#process.end = cms.EndPath(process.pdfSystematics)

### ----------------------------------------------------------------------
###
### HCP Synchronization file for 533.
###
### FIXME: the path includes the rochester correction (which is a passtrough for 53X right now);
### the syncronization is supposed to be done with no escale applied
###
###----------------------------------------------------------------------

LEPTON_SETUP = 2012
PD = ""
MCFILTER = ""
ELECORRTYPE   = "Paper" # "None", "Moriond", or "Paper"
ELEREGRESSION = "Paper" # "None", "Moriond", "PaperNoComb", or "Paper" 
APPLYMUCORR = True

#For DATA: 
#IsMC = False
#PD = "DoubleEle"

# Get absolute path
import os
PyFilePath = os.environ['CMSSW_BASE'] + "/src/ZZAnalysis/AnalysisStep/test/"

### ----------------------------------------------------------------------
### Standard sequence
### ----------------------------------------------------------------------

execfile(PyFilePath + "analyzer.py")


### ----------------------------------------------------------------------
  ### Replace parameters
### ----------------------------------------------------------------------
process.source.fileNames = cms.untracked.vstring(
    'root://lxcms00//data3/2013/HZZ_cmgTuple/BE539_H1258TeV.root' #533 V5_15_0 version
    )


### FIXME: DEBUGGING ONLY, turns off smearing if used with special tag
process.calibratedPatElectrons.synchronization = True
##process.calibratedPatElectrons.smearingRatio = 1
process.calibratedMuons.fakeSmearing = cms.bool(True)
#process.calibratedMuons.fakeSmearing = cms.untracked.bool(True)

#process.appendPhotons.debug = cms.untracked.bool(True)

process.maxEvents.input = 100

# Silence output
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


### ----------------------------------------------------------------------
### Analyzer for Plots
### ----------------------------------------------------------------------


# jet-jet pairs
process.disambiguatedJets = cms.EDProducer("JetsWithLeptonsRemover",
                                           Jets  = cms.InputTag("cmgPFJetSel"),
                                           Muons = cms.InputTag("appendPhotons:muons"),
                                           Electrons = cms.InputTag("appendPhotons:electrons"))

process.bareWCand = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string('cmgPFJetSel cmgPFJetSel'),
                                   cut = cms.string('mass > 0'), # protect against ghosts
                                   checkCharge = cms.bool(True))

process.WCand = cms.EDProducer("WCandidateFiller",
                               src = cms.InputTag("bareWCand"))


### Simple plot for test

process.wPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
                            src = cms.InputTag("WCand"),
                            histograms = cms.VPSet(cms.PSet(min = cms.untracked.double(0.0),
                                                            max = cms.untracked.double(1000.0),
                                                            nbins = cms.untracked.int32(100),   
                                                            name = cms.untracked.string("W_pT"),
                                                            description = cms.untracked.string("W pT [GeV/c]"),
                                                            plotquantity = cms.untracked.string("pt"))))

# Build W->jj candidate
process.WjjSequence = cms.Sequence(process.disambiguatedJets * process.bareWCand * process.WCand  * process.wPt)
process.Candidates *= process.WjjSequence


# Fill the tree for the analysis
process.treePlanter = cms.EDAnalyzer("TreePlanter",
                                     muons     = cms.InputTag("appendPhotons:muons"),
                                     electrons = cms.InputTag("appendPhotons:electrons"),
                                     jets      = cms.InputTag("disambiguatedJets"),
                                     Zmm       = cms.InputTag("MMCand"),
                                     Zee       = cms.InputTag("EECand"),
                                     Wjj       = cms.InputTag("WCand"),
                                     MET       = cms.InputTag("cmgPFMET"),
                                     Vertices  = cms.InputTag("goodPrimaryVertices"),                                    
                                     isMC      = cms.untracked.bool(False),
                                     PUInfo    = cms.untracked.InputTag("addPileupInfo")
                                     )

process.fillTrees = cms.Path(process.treePlanter)



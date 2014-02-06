### ----------------------------------------------------------------------
### Based on ZZ->4l strategy.
###----------------------------------------------------------------------


LEPTON_SETUP = 2012
PD = ""
MCFILTER = ""
ELECORRTYPE   = "Paper" # "None", "Moriond", or "Paper"
ELEREGRESSION = "Paper" # "None", "Moriond", "PaperNoComb", or "Paper" 
APPLYMUCORR = True


try:
    IsMC
except NameError:
    IsMC = True

try:
    LEPTON_SETUP
except NameError:
    LEPTON_SETUP = 2012 # define the set of effective areas, rho corrections, etc.

try:
    PD
except NameError:
    PD = ""             # "" for MC, "DoubleEle", "DoubleMu", or "MuEG" for data 

try:
    MCFILTER
except NameError:
    MCFILTER = ""



# Get absolute path
import os
PyFilePath = os.environ['CMSSW_BASE'] + "/src/ZZAnalysis/AnalysisStep/test/"

### ----------------------------------------------------------------------
### Standard sequence
### ----------------------------------------------------------------------

execfile(PyFilePath + "MasterPy/ZZ4lAnalysis.py")         # 2012 reference analysis


### ----------------------------------------------------------------------
### Replace parameters
### ----------------------------------------------------------------------

### ----------------------------------------------------------------------
  ### Replace parameters
### ----------------------------------------------------------------------
process.source.fileNames = cms.untracked.vstring(
    #'root://lxcms00//data3/2013/HZZ_cmgTuple/BE539_H1258TeV.root' #533 V5_15_0 version
    '/store/cmst3/group/cmgtools/CMG/WZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_10_1_nLP.root'
    #'/store/cmst3/group/cmgtools/CMG/WZZ_8TeV-aMCatNLO-herwig/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_100_1_GEb.root'
    #'/store/cmst3/group/cmgtools/CMG/ZZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_10_1_UV1.root'
    )


### FIXME: DEBUGGING ONLY, turns off smearing if used with special tag
process.calibratedPatElectrons.synchronization = True
##process.calibratedPatElectrons.smearingRatio = 1
process.calibratedMuons.fakeSmearing = cms.bool(True)
#process.calibratedMuons.fakeSmearing = cms.untracked.bool(True)

#process.appendPhotons.debug = cms.untracked.bool(True)

process.maxEvents.input = -1

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
                                           Electrons = cms.InputTag("appendPhotons:electrons"),
                                           EnergyFractionAllowed = cms.double(0)) # maximum energy fraction carried by the lepton in the jet, to accept a jet as non from lepton

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


process.Candidates = cms.Path(process.muons             +
                              process.electrons         + process.cleanSoftElectrons +
                              process.appendPhotons     +
                              process.softLeptons       +
                              # Build Boson candidates
                              process.bareEECand        + process.EECand +  
                              process.bareMMCand        + process.MMCand +
                              process.WjjSequence
                              )

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
                                     isMC      = cms.untracked.bool(True),
                                     PUInfo    = cms.untracked.InputTag("addPileupInfo")
                                     )


process.genCategory =  cms.EDFilter("GenFilterCategory",
                                    src = cms.InputTag("genParticlesPruned"),
                                    Category = cms.int32(-1),
                                    SignalDefinition = cms.int32(1))

process.filltrees = cms.Path(process.printTree + process.genCategory * process.treePlanter)


### ----------------------------------------------------------------------
### Output root file (monitoring histograms)
### ----------------------------------------------------------------------
process.TFileService=cms.Service('TFileService',
                                fileName=cms.string('ZZ4lAnalysis.root')
                                )

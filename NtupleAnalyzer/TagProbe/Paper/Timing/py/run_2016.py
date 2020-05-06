import os,ROOT

filename  = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/'
filename += 'DQM_V0001_R000283407__HLT__FastTimerService__All.root'

f = ROOT.TFile(filename)

double_trig = "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7"
single_trig = "HLT_IsoMu24_v4"

basedir = "DQMData/Run 283407/HLT/Run summary/TimerService/Running 1 processes/process TIMING/Paths"
TDir = f.Get(basedir)

single_h = TDir.Get(single_trig+"_module_average")
single_xaxis = single_h.GetXaxis()
single_Nbin = single_xaxis.GetNbins()

PathsToCheck = []

print "@@@@ From "+single_trig
for i in range(0,single_Nbin):
  modulename = single_xaxis.GetBinLabel(i+1)
  avgtime = single_h.GetBinContent(i+1)

  if avgtime>0.1:
    print modulename+'\t'+str(avgtime)
    PathsToCheck.append(modulename)

double_h = TDir.Get(double_trig+"_module_average")
double_xaxis = double_h.GetXaxis()
double_Nbin = double_xaxis.GetNbins()

print "@@@@ From "+double_trig
for i in range(0,double_Nbin):
  modulename = double_xaxis.GetBinLabel(i+1)
  avgtime = double_h.GetBinContent(i+1)

  if modulename in PathsToCheck:
    print modulename+'\t'+str(avgtime)

print '@@@@ From 2018'
PathsToCheck_2018 = [
"hltGtStage2ObjectMap",
"hltSiPixelDigis",
"hltSiPixelClusters",
"hltSiPixelRecHits",
"hltIterL3OITrackCandidates",
"hltIter3IterL3MuonCkfTrackCandidates",
"hltIter2IterL3FromL1MuonCkfTrackCandidates",
"hltIter3IterL3FromL1MuonCkfTrackCandidates",
"hltIterL3MuonsNoID",
]

print "@@@@ From "+single_trig
for i in range(0,single_Nbin):
  modulename = single_xaxis.GetBinLabel(i+1)
  avgtime = single_h.GetBinContent(i+1)

  if modulename in PathsToCheck_2018:
    print modulename+'\t'+str(avgtime)
print "@@@@ From "+double_trig
for i in range(0,double_Nbin):
  modulename = double_xaxis.GetBinLabel(i+1)
  avgtime = double_h.GetBinContent(i+1)

  if modulename in PathsToCheck_2018:
    print modulename+'\t'+str(avgtime)


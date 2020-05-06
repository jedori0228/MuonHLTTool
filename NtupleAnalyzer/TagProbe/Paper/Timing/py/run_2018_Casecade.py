import os,ROOT

filename = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/'
filename += 'DQM_V0001_R000319941__HLT__FastTimerService__All.root'

f = ROOT.TFile(filename)

iterL3_trig = "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5"
casecade_trig = "HLT_OldMu17_TrkIsoVVL_OldMu8_TrkIsoVVL_DZ_v1"

basedir = "DQMData/Run 319941/HLT/Run summary/TimerService/process TIMING paths/path "

casecade_TDir = f.Get(basedir+casecade_trig)
casecade_h = casecade_TDir.Get("module_time_real_average")
casecade_xaxis = casecade_h.GetXaxis()
casecade_Nbin = casecade_xaxis.GetNbins()

PathsToCheck = []
AvgTimings = []

print "@@@@ From "+casecade_trig
for i in range(0,casecade_Nbin):
  modulename = casecade_xaxis.GetBinLabel(i+1)
  avgtime = casecade_h.GetBinContent(i+1)

  print modulename+'\t'+str(avgtime)
  PathsToCheck.append(modulename)
  AvgTimings.append(avgtime)


iterL3_TDir = f.Get(basedir+iterL3_trig)
iterL3_h = iterL3_TDir.Get("module_time_real_average")
iterL3_xaxis = iterL3_h.GetXaxis()
iterL3_Nbin = iterL3_xaxis.GetNbins()

print "@@@@ From "+iterL3_trig
for i in range(0,iterL3_Nbin):
  modulename = iterL3_xaxis.GetBinLabel(i+1)
  avgtime = iterL3_h.GetBinContent(i+1)

  if modulename in PathsToCheck:
    print modulename+'\t'+str(AvgTimings[i])+'\t'+str(avgtime)

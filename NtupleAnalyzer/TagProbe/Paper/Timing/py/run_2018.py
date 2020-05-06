import os,ROOT

filename = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/'
filename += 'DQM_V0001_R000319941__HLT__FastTimerService__All.root'

f = ROOT.TFile(filename)

double_trig = "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5"
single_trig = "HLT_IsoMu24_v13"

basedir = "DQMData/Run 319941/HLT/Run summary/TimerService/process TIMING paths/path "

single_TDir = f.Get(basedir+single_trig)
single_h = single_TDir.Get("module_time_real_average")
single_xaxis = single_h.GetXaxis()
single_Nbin = single_xaxis.GetNbins()

PathsToCheck = []

print "@@@@ From "+single_trig
for i in range(0,single_Nbin):
  modulename = single_xaxis.GetBinLabel(i+1)
  avgtime = single_h.GetBinContent(i+1)

  if avgtime>0.5:
    print modulename+'\t'+str(avgtime)
    PathsToCheck.append(modulename)


double_TDir = f.Get(basedir+double_trig)
double_h = double_TDir.Get("module_time_real_average")
double_xaxis = double_h.GetXaxis()
double_Nbin = double_xaxis.GetNbins()

print "@@@@ From "+double_trig
for i in range(0,double_Nbin):
  modulename = double_xaxis.GetBinLabel(i+1)
  avgtime = double_h.GetBinContent(i+1)

  if modulename in PathsToCheck:
    print modulename+'\t'+str(avgtime)

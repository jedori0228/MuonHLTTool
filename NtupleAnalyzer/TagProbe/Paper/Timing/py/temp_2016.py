import os,ROOT

filename  = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/'
filename += 'DQM_V0001_R000283407__HLT__FastTimerService__All.root'

f = ROOT.TFile(filename)

double_trig = "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7"

basedir = "DQMData/Run 283407/HLT/Run summary/TimerService/Running 1 processes/process TIMING/Paths"
TDir = f.Get(basedir)

double_h = TDir.Get(double_trig+"_module_average")
double_xaxis = double_h.GetXaxis()
double_Nbin = double_xaxis.GetNbins()


for i in range(0,double_Nbin):
  modulename = double_xaxis.GetBinLabel(i+1)
  avgtime = double_h.GetBinContent(i+1)

  print modulename+'\t'+str(avgtime)

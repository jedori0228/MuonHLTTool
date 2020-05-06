import os,ROOT

PUs = [
"23-27",
"33-37",
"45-50",
]
Runs = [
283270,
283453,
283353,
]

for i_pu in range(0,len(PUs)):

  #print '@@@@'+str(PUs[i_pu])

  filename = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/PUStudy/2016/job_PU'+PUs[i_pu]+'/'
  filename += 'DQM_V0001_R000'+str(Runs[i_pu])+'__HLT__FastTimerService__All.root'
  basedir = "DQMData/Run "+str(Runs[i_pu])+"/HLT/Run summary/TimerService/Running 1 processes/process TIMING/Paths"

  #### Cascade

  detail_lines = open('2016_HLT_Mu50_v5_detail.txt').readlines()

  f = ROOT.TFile(filename)
  double_trig = "HLT_Mu50_v5"
  TDir = f.Get(basedir)

  double_h = TDir.Get(double_trig+"_module_average")
  double_xaxis = double_h.GetXaxis()
  double_Nbin = double_xaxis.GetNbins()

  ModulesCounted = []
  TimingsCounted = []
  for i in range(0,double_Nbin):
    modulename = double_xaxis.GetBinLabel(i+1)
    avgtime = double_h.GetBinContent(i+1)

    ModulesCounted.append(modulename)
    TimingsCounted.append(avgtime)

  #### Tracker

  detail_lines = open('2016_HLT_TkMu50_v3_detail.txt').readlines()

  f = ROOT.TFile(filename)
  double_trig = "HLT_TkMu50_v3"
  TDir = f.Get(basedir)

  double_h = TDir.Get(double_trig+"_module_average")
  double_xaxis = double_h.GetXaxis()
  double_Nbin = double_xaxis.GetNbins()

  for i in range(0,double_Nbin):
    modulename = double_xaxis.GetBinLabel(i+1)
    avgtime = double_h.GetBinContent(i+1)

    for z in range(0,len(ModulesCounted)):
      this_module = ModulesCounted[z]
      if this_module==modulename:
        foundindex = z
        break
    #### if found in ModulesCounted, use the larger timing
    if foundindex>=0:
      if avgtime>TimingsCounted[foundindex]:
        TimingsCounted[foundindex] = avgtime
    #### if not found, append
    else:
      ModulesCounted.append(modulename)
      TimingsCounted.append(avgtime)

  SumTiming = 0.
  for t in TimingsCounted:
    SumTiming += t

  print str(SumTiming)+','

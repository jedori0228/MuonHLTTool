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

  detail_lines = open('2016_HLT_IsoTkMu24_v4_detail.txt').readlines()

  Modules = []
  ModuleDetails = []
  ModuleTime = []
  for i_line in range(0,len(detail_lines)):

    line = detail_lines[i_line]
    if '#' in line:
      ModuleName = line.split()[1]
      Detail = detail_lines[i_line+1].strip('\n')
      words = Detail.split('+')
      #print '#### '+ModuleName
      #print words

      Modules.append(ModuleName)
      ModuleDetails.append(words)
      ModuleTime.append(0)


  f = ROOT.TFile(filename)
  double_trig = "HLT_IsoTkMu24_v4"
  TDir = f.Get(basedir)

  double_h = TDir.Get(double_trig+"_module_average")
  double_xaxis = double_h.GetXaxis()
  double_Nbin = double_xaxis.GetNbins()
      

  for i in range(0,double_Nbin):
    modulename = double_xaxis.GetBinLabel(i+1)
    avgtime = double_h.GetBinContent(i+1)


    for j in range(0,len(ModuleDetails)):
      if modulename in ModuleDetails[j]:
        ModuleTime[j] += avgtime
        #print '['+modulename+'] in '+Modules[j]+' : '+str(avgtime)

    #print modulename+'\t'+str(avgtime)

  this_HLTMuonLocalRecoSequence = 0
  this_HLTHighPt24TrackerMuonSequence = 0
  for i in range(0,len(Modules)):
    if "HLTMuonLocalRecoSequence" in Modules[i]:
      this_HLTMuonLocalRecoSequence = ModuleTime[i]
      #print '    %1.2E,'%ModuleTime[i]
    if "HLTHighPt24TrackerMuonSequence" in Modules[i]:
      this_HLTHighPt24TrackerMuonSequence = ModuleTime[i]
      #print '    %1.2E,'%ModuleTime[i]
  #print 'this_HLTMuonLocalRecoSequence = '+str(this_HLTMuonLocalRecoSequence)
  #print 'this_HLTHighPt24TrackerMuonSequence = '+str(this_HLTHighPt24TrackerMuonSequence)
  print '    %1.2E,'%(this_HLTMuonLocalRecoSequence+this_HLTHighPt24TrackerMuonSequence)

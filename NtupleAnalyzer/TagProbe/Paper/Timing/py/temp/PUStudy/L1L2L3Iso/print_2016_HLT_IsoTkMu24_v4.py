import os,ROOT

PUs = [
"45-50",
]
Runs = [
283353,
]

toChecks = [
['hltTriggerType', 'hltL1fL1sMu22L1Filtered0'],
['', ''],
['hltMuonDTDigis', 'hltL3fL1sMu22f0TkFiltered24Q'],
['hltEcalDigis', 'hltBoolEnd'],
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
      
  for toCheck in toChecks:

    #print '@@@@ Checking '+toCheck[0]+', '+toCheck[1]

    sumtime = 0.

    firstModuleFound = False
    lastModuleFound = False

    for i in range(0,double_Nbin):
      modulename = double_xaxis.GetBinLabel(i+1)
      avgtime = double_h.GetBinContent(i+1)

      if toCheck[0] == modulename:
        firstModuleFound = True
        #print 'First found'

      if firstModuleFound and not lastModuleFound:
        #print modulename
        sumtime += avgtime

      if toCheck[1] == modulename:
        lastModuleFound = True
        #print 'Last found'

    #print toCheck
    print str(sumtime)+','


import os,ROOT

PUs = [
"36-38",
"38-40",
"40-42",
"42-44",
"44-46",
"46-48",
"48-50",
"50-52",
]
Runs = [
324878,
325022,
324878,
325022,
325022,
325022,
325022,
325022,
]

for i_pu in range(0,len(PUs)):

  filename = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/PUStudy/2018/job_PU'+PUs[i_pu]+'/'
  filename += 'DQM_V0001_R000'+str(Runs[i_pu])+'__HLT__FastTimerService__All.root'

  detail_lines = open('2018_HLT_IsoMu24_v13_detail.txt').readlines()

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

  double_trig = "HLT_IsoMu24_v13"
  basedir = "DQMData/Run "+str(Runs[i_pu])+"/HLT/Run summary/TimerService/process TIMING paths/path "

  double_TDir = f.Get(basedir+double_trig)
  double_h = double_TDir.Get("module_time_real_average")
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

  for i in range(0,len(Modules)):
    if 'HLTL3muonrecoSequence' in Modules[i]:
      print '    %1.2E,'%ModuleTime[i]

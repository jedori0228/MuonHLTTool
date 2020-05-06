import os,ROOT

detail_lines = open('2016_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7_detail.txt').readlines()

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


  for j in range(0,len(ModuleDetails)):
    if modulename in ModuleDetails[j]:
      ModuleTime[j] += avgtime
      print '['+modulename+'] in '+Modules[j]+' : '+str(avgtime)

  #print modulename+'\t'+str(avgtime)

for i in range(0,len(Modules)):
  print Modules[i]+' (%1.2E ms) +'%ModuleTime[i]



#### Coutter
print '@@@@@@@@@@@@@@@@@@@@ counter @@@@@@@@@@@@@@@@@@@@@@@@@2'

double_h_counter = TDir.Get(double_trig+"_module_counter")
double_xaxis = double_h_counter.GetXaxis()
double_Nbin = double_xaxis.GetNbins()
for i in range(0,double_Nbin):
  modulename = double_xaxis.GetBinLabel(i+1)
  count = double_h_counter.GetBinContent(i+1)

  print modulename+'\t'+str(count)

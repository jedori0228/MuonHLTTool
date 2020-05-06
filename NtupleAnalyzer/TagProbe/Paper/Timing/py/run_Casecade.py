import os,ROOT

Modules2016 = []
Times2016 = []
lines = open('2016_CascadeModule.txt').readlines()
for line in lines:
  words = line.split('\t')
  Modules2016.append(words[0])
  Times2016.append(float(words[1]))

#### now get 2018 numbers

filename = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/'
filename += 'BACKUP_DQM_V0001_R000319941__HLT__FastTimerService__All_RunCascade.root'

f = ROOT.TFile(filename)

#### cascade trigger in 2018
double_trig = "HLT_OldMu17_TrkIsoVVL_OldMu8_TrkIsoVVL_DZ_v1"

basedir = "DQMData/Run 319941/HLT/Run summary/TimerService/process TIMING paths/path "

double_TDir = f.Get(basedir+double_trig)
double_h = double_TDir.Get("module_time_real_average")
double_xaxis = double_h.GetXaxis()
double_Nbin = double_xaxis.GetNbins()

for i in range(0,double_Nbin):
  modulename = double_xaxis.GetBinLabel(i+1)
  avgtime = double_h.GetBinContent(i+1)

  for j in range(0,len(Modules2016)):
    Module2016 = Modules2016[j]
    Time2016 = Times2016[j]

    if modulename==Module2016:
      print modulename+'\t'+str(Time2016)+'\t'+str(avgtime)
      break


print '\n\n\n'
print '@@@@ Printing all 2016 modules'
print '\n\n\n'
for j in range(0,len(Modules2016)):
  Module2016 = Modules2016[j]
  Time2016 = Times2016[j]

  Found = False
  for i in range(0,double_Nbin):
    Module2018 = double_xaxis.GetBinLabel(i+1)
    Time2018 = double_h.GetBinContent(i+1)

    if Module2016==Module2018:
      print Module2016+'\t'+str(Time2016)+'\t'+str(Time2018)
      Found = True
      break

  if not Found:
    print Module2016+'\t'+str(Time2016)+'\t'+'-'

print '\n\n\n'
print '@@@@ Printing all 2018 modules'
print '\n\n\n'

for i in range(0,double_Nbin):
  Module2018 = double_xaxis.GetBinLabel(i+1)
  Time2018 = double_h.GetBinContent(i+1)

  Found = False
  for j in range(0,len(Modules2016)):
    Module2016 = Modules2016[j]
    Time2016 = Times2016[j]


    if Module2016==Module2018:
      print Module2018+'\t'+str(Time2016)+'\t'+str(Time2018)
      Found = True
      break

  if not Found:
    print Module2018+'\t'+'-'+'\t'+str(Time2018)


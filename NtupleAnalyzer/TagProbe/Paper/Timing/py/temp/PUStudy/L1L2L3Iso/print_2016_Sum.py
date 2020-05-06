import os,ROOT

PUs = [
"45-50",
]
Runs = [
283353,
]

toChecks_Cas = [
#['hltTriggerType', 'hltL1fL1sMu22L1Filtered0'],
['hltMuonDTDigis', 'hltL2fL1sMu22L1f0L2Filtered10Q'],
['hltSiPixelDigis', 'hltL3fL1sMu22L1f0L2f10QL3Filtered24Q'],
['hltEcalDigis', 'hltBoolEnd'],
]

toChecks_Trk = [
#['hltTriggerType', 'hltL1fL1sMu22L1Filtered0'],
['', ''],
['hltMuonDTDigis', 'hltL3fL1sMu22f0TkFiltered24Q'],
['hltEcalDigis', 'hltBoolEnd'],
]

for i_pu in range(0,len(PUs)):

  #print '@@@@'+str(PUs[i_pu])

  filename = '/Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/TagProbe/Paper/Timing/rootfiles/PUStudy/2016/job_PU'+PUs[i_pu]+'/'
  filename += 'DQM_V0001_R000'+str(Runs[i_pu])+'__HLT__FastTimerService__All.root'
  basedir = "DQMData/Run "+str(Runs[i_pu])+"/HLT/Run summary/TimerService/Running 1 processes/process TIMING/Paths"

  detail_lines = open('2016_HLT_IsoMu24_v4_detail.txt').readlines()

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
  TDir = f.Get(basedir)

  double_h_Cas = TDir.Get("HLT_IsoMu24_v4_module_average")
  double_xaxis_Cas = double_h_Cas.GetXaxis()
  double_Nbin_Cas = double_xaxis_Cas.GetNbins()

  double_h_Trk = TDir.Get("HLT_IsoTkMu24_v4_module_average")
  double_xaxis_Trk = double_h_Trk.GetXaxis()
  double_Nbin_Trk = double_xaxis_Trk.GetNbins()

  modules_counted = []
  timings_counted = []
  CASoTRK_counted = []


  for i_step in range(0, len(toChecks_Cas)):

    toCheck_Cas = toChecks_Cas[i_step]
    toCheck_Trk = toChecks_Trk[i_step]

    this_modules_counted = []
    this_timings_counted = []
    this_CASoTRK_counted = []

    #### Cadcade

    firstModuleFound = False
    lastModuleFound = False
    for i in range(0,double_Nbin_Cas):
      modulename = double_xaxis_Cas.GetBinLabel(i+1)
      avgtime = double_h_Cas.GetBinContent(i+1)

      if toCheck_Cas[0] == modulename:
        firstModuleFound = True
        #print 'First found'

      if firstModuleFound and not lastModuleFound:
        #print modulename
        this_modules_counted.append( modulename )
        this_timings_counted.append( avgtime )
        this_CASoTRK_counted.append( "Cascade" )

        #print modulename+'\t'+str(avgtime)

      if toCheck_Cas[1] == modulename:
        lastModuleFound = True
        #print 'Last found'

    #### Tracker

    firstModuleFound = False
    lastModuleFound = False
    for i in range(0,double_Nbin_Trk):
      modulename = double_xaxis_Trk.GetBinLabel(i+1)
      avgtime = double_h_Trk.GetBinContent(i+1)

      if toCheck_Trk[0] == modulename:
        firstModuleFound = True
        #print 'First found'

      if firstModuleFound and not lastModuleFound:
        #print modulename
        #print modulename+'\t'+str(avgtime)

        foundindex = -1
        for z in range(0,len(this_modules_counted)):
          if this_modules_counted[z]==modulename:
            foundindex = z
            break
        #### if module is already in this_modules_counted, use the larger
        if foundindex>=0:
          #if i_step==1:
          #  print modulename+'\t'+str(this_timings_counted[z])+'\t'+str(avgtime)
          if avgtime>=this_timings_counted[z]:
            this_timings_counted[z] = avgtime
            this_CASoTRK_counted[z] = "Tracker"
        #### if not found, append
        else:
          this_modules_counted.append( modulename )
          this_timings_counted.append( avgtime )
          this_CASoTRK_counted.append( "Tracker" )

      if toCheck_Trk[1] == modulename:
        lastModuleFound = True
        #print 'Last found'

    modules_counted.append(this_modules_counted)
    timings_counted.append(this_timings_counted)
    CASoTRK_counted.append(this_CASoTRK_counted)

for i in range(0,len(modules_counted)):

  this_modules_counted = modules_counted[i]
  this_timings_counted = timings_counted[i]
  this_CASoTRK_counted = CASoTRK_counted[i]

  sumtime = 0.

  print '@@@@ Checking '+str(i)

  for j in range(0,len(this_modules_counted)):

    module = this_modules_counted[j]
    timing = this_timings_counted[j]
    CASoTrk = this_CASoTRK_counted[j]

    #### check if module exists in the previous step
    if i==1:
      #print '#### Checking '+module
      Found = False
      for ii in range(0,i):
        #print '####   Looping '+str(ii)+'th step'
        if Found:
          break
        prev_modules_counted = modules_counted[ii]
        for m in prev_modules_counted:
          if m==module:
            Found = True
            break
      if not Found:
        sumtime += timing
      else:
        print module+'\t'+str(timing)+'\t'+str(CASoTrk)

    #### if first step, just add
    else:
      sumtime += timing

  print str(sumtime)+','

'''
    sumtime = 0.
    for t in this_timings_counted:
      sumtime += t

    #print toCheck_Cas
    print str(sumtime)+','
'''

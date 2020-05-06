from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'MuonHLTNtuple_SimMatchingInfo_2018_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8'
config.General.workArea = 'crab_projects'

config.JobType.psetName = 'Run_ntupler.py'
config.JobType.pluginName = 'Analysis'
#config.JobType.numCores = 4
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000


config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
# config.Data.useParent = True

config.Data.inputDBS = 'global'

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50

config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Site.storageSite = 'T2_KR_KNU'



config.JobType.allowUndistributedCMSSW = True

from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = ''
config.General.workArea = 'crab_projects'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = ''
config.JobType.numCores = 4
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

config.Data.inputDBS = 'global'

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 5

config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Site.storageSite = 'T2_KR_KNU'

config.Data.lumiMask = '/afs/cern.ch/user/k/kplee/public/MuonTriggerPaper/JSON/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_Last5invfb.txt'

version = '_'

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # -- for efficiency: useParent = True
    config.General.requestName = 'MuonHLTNtuple'+version+'SingleMuon_Run2017F_RunAfter305821'
    config.Data.inputDataset = '/SingleMuon/Run2017F-17Nov2017-v1/AOD'
    config.JobType.psetName = 'HLTCfg_2017_Data.py'
    config.Data.useParent = True
    config.JobType.allowUndistributedCMSSW = True
    crabCommand('submit', config = config)


#!/usr/bin/env python

import os,sys
import argparse
import datetime

CMSSW_BASE = os.environ['CMSSW_BASE']
UID = str(os.getuid())

PWD = os.getcwd()

parser = argparse.ArgumentParser(description='tamsa1 submit')
parser.add_argument('--name', dest='name')
parser.add_argument('--skeleton', dest='skeleton')
parser.add_argument('--inputFile', dest='inputFile')
parser.add_argument('-n', dest='njobs', type=int)
parser.add_argument('--year', dest='year', type=int, default=-1)
args = parser.parse_args()

## year

if args.year<0:

  tmp_Year = args.year
  if "2016" in args.inputFile:
    tmp_Year = 2016
  elif "2018" in args.inputFile:
    tmp_Year = 2018
  else:
    print "Specify year with --year"
    sys.exit()
  args.year = tmp_Year
  print "Year is not set. Automatically set to "+str(args.year)

## timestamp

JobStartTime = datetime.datetime.now()
timestamp =  JobStartTime.strftime('%Y_%m_%d_%H%M%S')

## job directory name

JobDir = "Run"+str(args.year)+"__"+timestamp+"__"+args.name
fullJobDir = PWD+'/'+JobDir+'/'
os.system('mkdir -p '+JobDir+'/outputs/')

## read inputfile list

Files = open(args.inputFile).readlines()

## make filerange

FileRanges = []
temp_end_largerjob = 0
nfile_checksum = 0

NTotalFiles = len(Files)
nfilepjob = int(NTotalFiles/args.njobs)
nfilepjob_remainder = NTotalFiles-(args.njobs)*(nfilepjob)

SubmitOutput = open(JobDir+'/SubmitOutput.log','w')

if nfilepjob_remainder>=(args.njobs):
  SubmitOutput.write('nfilepjob_remainder = '+str(nfilepjob_remainder)+'\n')
  SubmitOutput.write('while, args.njob = '+str(args.njobs)+'\n')
  SubmitOutput.write('--> exit'+'\n')
  sys.exit()

## First nfilepjob_remainder jobs will have (nfilepjob+1) files per job

for it_job in range(0,nfilepjob_remainder):
  FileRanges.append(range(it_job*(nfilepjob+1),(it_job+1)*(nfilepjob+1)))
  temp_end_largerjob = (it_job+1)*(nfilepjob+1)
  nfile_checksum += len(range(it_job*(nfilepjob+1),(it_job+1)*(nfilepjob+1)))

## Remaining NJobs-nfilepjob_remainder jobs will have (nfilepjob) files per job

for it_job in range(0,args.njobs-nfilepjob_remainder):
  FileRanges.append(range(temp_end_largerjob+(it_job*nfilepjob),temp_end_largerjob+((it_job+1)*nfilepjob) ))
  nfile_checksum += len(range(temp_end_largerjob+(it_job*nfilepjob),temp_end_largerjob+((it_job+1)*nfilepjob) ))
SubmitOutput.write('nfile_checksum = '+str(nfile_checksum)+'\n')
SubmitOutput.write('NTotalFiles = '+str(NTotalFiles)+'\n')
SubmitOutput.close()

## write job

sklines = open(args.skeleton).readlines()

for it_FileRange in range(0,len(FileRanges)):
  FileRange = FileRanges[it_FileRange]

  this_job_dir = fullJobDir+'/job_'+str(it_FileRange)+'/'
  os.system('mkdir -p '+this_job_dir)

  rootrunfile = open(this_job_dir+'/run.cxx','w')

  for skline in sklines:
    if "OUTPUTSAMPLE" in skline:
      rootrunfile.write('  histProducer->Set_OutputFileName("'+fullJobDir+'/outputs/ROOTFile_TnPHist_'+str(it_FileRange)+'.root");\n')
    elif "INPUTSAMPLE" in skline:
      for fileindex in FileRange:
        rootrunfile.write('  histProducer->AddDataPath("'+Files[fileindex].strip('\n')+'");\n')
    else:
      rootrunfile.write(skline)
  rootrunfile.close()

  file_commands = open(this_job_dir+'/commands.sh','w')
  print>>file_commands,'''cd {0}
echo "Okay, let's run the analysis"
root -l -b -q run.cxx
'''.format(this_job_dir)
  file_commands.close()

  jobname = str(args.name)+'_'+str(it_FileRange)
  cmd = 'qsub -V -q fastq -N '+jobname+' -wd '+this_job_dir+' commands.sh '

  cwd = os.getcwd()
  os.chdir(this_job_dir)
  os.system(cmd+' > submitlog.log')
  os.chdir(cwd)

#submitScript = open(JobDir+'/submit.sh','w')


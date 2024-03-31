# ProdSys system
   The ProdSys system is used for official MC productions.
## Prerequisites
   - Get a grid certificate, register your certificate in VOMS and in DIRAC, more details can refer to the section "2. Prerequisites" 
   - Check if you can access the DIRAC web page: https://dirac.ihep.ac.cn/, more details can refer to the section "3.4 Monitoring your jobs"
## Procedures
   - Initialize DIRAC environment
    > source /cvmfs/dcomputing.ihep.ac.cn/dirac/IHEPDIRAC/bashrc 
   - Initialize your certificate to get a vaild proxy to access DIRAC system
    > dirac-proxy-init -g juno_production 
   - Prepare ini file to define your production parameters. Get ini template files, and write your own ini file:  myprod.ini ( 6.4. Write your own ini file )  
    > ihepdirac-juno-make-productions --example > prod.ini
   - Check your production parameters with "--dryrun"
    > ihepdirac-juno-make-productions --ini myprod.ini --dryrun 
   - Submit your production
    > ihepdirac-juno-make-productions --ini myprod.ini
   - Check production task status with DIRAC transformation monitoring page ( 6.5 Check production task status ) 
   - Check job status with DIRAC job monitoring page ( 3.4 Monitoring your jobs )
   - Check data from DFC ( Dirac File Catalogue ) ( 6.6 Check the data produced in DFC)
   - Deal with failed jobs ( 6.7 Deal with failed jobs ) 
   - Finalize and Clean ( 6.8 Finalize and clean )
## Available sites and its related SE

| GRID.IHEP.cn | GRID.JINR-CONDOR.ru |  GRID.INFN-CNAF.it | GRID.IN2P3.fr| CLUSTER.CSNS.cn  | CLUSTER.SDU.cn |CLOUD.INFN-PADOVANA.it |
| --------     |    --------         |     --------       |   --------         |   --------  | -------- | -------- |
| IHEP-STORM (Lustre), IHEP-JUNOEOS (EOS)   | JINR-EOS    |  CNAF-STORM | IN2P3-DCACHE|IHEP-JUNOEOS|IHEP-STORM|CNAF-STORM|

- You can choose or ban sites to choose the site suitable for your tasks
- The stable sites: GRID.IHEP.cn, GRID.JINR-CONDOR.ru, GRID.INFN-CNAF.it, GRID.IN2P3.fr, CLUSTER.CSNS.cn, CLOUD.INFN-PADOVANA.it
- The location of data depends on the choice of SE (Storage Element)
## Write your own ini file
### Parameter sections
  There are two parameter sections.
#### Common parameters [all]
```
; Optional: Define dryrun mode, this mode only test the configuration and do not really submit any jobs
; This can also be passed in the command argument
;dryrun = true

; Define user parameter section [Chain], task-specific parameters can be defined the [Chain] section
process = Chain

; Define JUNO offline software version
softwareVersion = centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0 

; Define production name, the name is used for current production
prodName = JUNOProdTest

; Optional: The prodNameSuffix will be added to the prodName to be sure that each submission has a unique name
;prodNameSuffix = _new

; Define the transformation group name, which is used for identifying monitoring cells 
transGroup = JUNO_prod_test

; Optional: If you just want to test a single tag, not all of them
; With this line enabled, the "tags" parameter will be ignored
;tag = e+_0.0MeV

; Define root directory in Dirac File Catalogue
; If outputType is "production", the root directory will be /juno/production
; If outputType is "user" or something else, the root directory will be under your user directory /juno/user/x/xxx
outputType = production 

; Define sub directory relative to the root directory
outputSubDir = muon/test001

; Define the output Mode, "closest" means uploading the job output data to the closest SE to the site
; If no closest SE is found for this site, then upload to the SE defined by "outputSE"
outputMode = closest
;outputSE = IHEP-STORM

; If no site specified, all available sites will be chosen unless the sites was put in "bannedsite"
;site = GRID.INFN-CNAF.it GRID.IHEP.cn GRID.IN2P3.fr GRID.JINR-CONDOR.ru
;bannedsite = GRID.IHEP.cn

; How many files to move in a single request
;moveGroupSize = 10

; Define the destination SE where data arrive
;moveTargetSE = IHEP-JUNOEOS CNAF-HTTPSTORM
moveTargetSE = IHEP-JUNOEOS

; If ignoreWorkflow is true, the workflow will not be created
;ignoreWorkflow = false 

; If ignoreMove is true, the dataflow will not be processed
;ignoreMove = false

```
#### Process-specific parameters
```
; The parameters in this section will overwrite what's in [all]
[Chain]
; Define the initial seed, "seed" + "jobid" is used for the final seed for each job
seed = 42

; Define maximum event in a job and the number of job, the total job volume is "evtmax" * "njobs"
evtmax = 2
njobs = 10

; Define the number of files to be held in a directory
max2dir = 10000

; Define regular expression for parsing the tags. https://docs.python.org/3/howto/regex.html
; Use group numbers "{0}", "{1}" in "detsim-mode"
tagParser = (.*)_(.*)MeV

; Used to define multi-tasks in one submission, single task can be set to the particle name eg. muon 
tags = e+_0.0MeV e+_1.398MeV e+_4.460MeV

; The final directory will be /<outputType>/<outputSubDir>/<softwareVersion>/<workDir>/<position>/<tags>
; The default is the process name, eg Chain 
; workDir = Chain 

; Define a subdirectory to identify particle positions
; The default is null
; position =  

; Define steps in the work flow
; workflow = detsim elecsim calib rec elecsim_rec
workflow = detsim elecsim_rec

; Define steps in the data flow
;moveType = detsim elecsim calib rec elecsim_rec
moveType = detsim

; Define if user root file need to be generated, the default is 1 (needed)
userOutput = 0 

; Define detsim gentools mode for the detsim step, {0} and {1} are defined by tags
; the default: 
; detsim-mode = --no-gdml --evtmax $evtmax --seed $seed --output detsim-${seed}.root --user-output detsim_user-${seed}.root
detsim-mode = gun --particles {0} --momentums {1} --positions 0 0 0

; Define additional parameters for each step
; The default:
; elecsim-mode = --evtmax -1 --seed $num --input $inputfile --output elecsim-$num.root --rate 1.0
; elecsim_rec-mode = --evtmax -1 --seed $num --input $inputfile --output elecsim_rec-$num.root
; cal-mode = --evtmax -1 --input $inputfile --output calib-$num.root
; rec-mode = --evtmax $evtmax --input $inputfile --output rec-$num.root 
elecsim_rec-mode = --rate 0.001 --enableWP --enableWPDarkPulse --no-evtrec

```
### Tags and TagParser

To allow multi tasks in one submission, the ProdSys system use Tags and TagParser. It can be used in the case that more than one particles with different momentums need to be produced. You can describe particle names, momentums or other parameters in Tags with a format which can be parsed by TagParser into a parameter array. This array will be used in detsim-mode
- Here are some examples: 

#### Generate gamma, e+ with  energy from 1-2Mev
```
 tagParser = (.*)_(.*)momentums
 tag = e+_1.0momentums e+_2.0momentums gamma_1.0momentums gamma_2.0momentums
```
- This will produce an parameter arraye {(e+, 1.0), {e+, 2.0}, {gamma, 1.0}, {gamma, 2.0}
- This array will be arranged into detsim options as {0}, {1}：
```
   detsim-mode = gun --particles {0} --momentums {1} --positions 0 0 0 
```

#### Generate radioactivity background
```
 tagParser = (.*)_(.*)Time_(.*)Volume_(.*)Material
 tag = U238_3e5Time_pTargetVolume_LSMaterial
 detsim-mode = gendecay --nuclear {0} -t {1} --volume {2} --material {3}
```

#### Another type of tagParser
```
 tagParser = (?P\<particle>.*)_(?P\<momentum>.*)MeV
 tags = e+_0.0MeV e+_1.398MeV
 detsim-mode = gun --particles {particle} --momentums {momentum} --positions 0 0 0
```   
     
### More examples

#### Muon

```
[Muon_01]
seed = 42
evtmax = 5
njobs = 2

workflow = detsim
moveType = detsim

; Define the script to be run before detsim with <step>-preScripts, $seed and $evtmax will be replaced with the one generated in each job
detsim-preScripts = $JUNOTOP/offline/InstallArea/${CMTCONFIG}/bin/Muon.exe -seed $seed -o Muon.txt -n $evtmax -v Rock -mult 1 -music_dir ${JUNOTOP}/data/Generator/Muon/data

detsim-mode = --pmtsd-v2 --pmtsd-merge-twindow 1.0 --pmt-hit-type 2 --anamgr-config-file <cvmfspath>/load_Muon_config.py --detoption Acrylic hepevt --exe Muon --file Muon.txt

```
Note: the input file eg. "load_Muon_config.py" should be available in CVMFS

#### Atmospheric neutrino 

```
[Chain]
seed = 1
evtmax = 10
njobs =  40000
max2dir = 10000


workflow = detsim
moveType = detsim
userOutput = 1
detsim-mode = --pmtsd-merge-twindow 1.0 --pmt-hit-type 2 --no-anamgr-optical-parameter --event-split hepevt --file Atm.txt --material LS --volume pTarget
detsim-preScripts = AtmNuGen.exe -seed $seed -n 10 -o Atm.txt -noDeex -tran_ene_max 1.5 -tran_ene_min 0.6
```

## Check production task status
After submission, task status can be checked from the DIRAC "Transformation Monitoring" web page.
- In the monitoring page, you can select the productions with group name (the "transGroup" in ini file）， time range, or request ID ( find in output of the submission) 
- In the production line, right click and use "show jobs" to go the job monitoring page where you can get more details of job status  
- Operations can be done using buttons in the monitoring page
  * Extend  -- Add more number of jobs to produce more events
  * Stop   -- Stop the producton task
  * Complete  -- When the task is finished, complete it 
  * Flush -- When the number of transfer jobs is less than 10, use flush to trigger the transfer

## Check the data produced in DFC
   The usage of DFC can refer to the section "5. Data Usage"
- Use DFC interactive console: dirac-dms-filecatalog-cli
- Use "help" to find commands
- Use /\<outputType>/\<outputSubDir>/\<softwareVersion>/\<workDir>/\<position> defined in your ini file to locate your data
   You also can get it from the output log of submission, eg.
```
-bash-4.2$ ihepdirac-juno-make-productions.py --ini prod.ini --dryrun
Owner: zhangxm
OwnerGroup: juno_production
VO: juno
ProdRoot: /juno/user/z/zhangxm/positron/prd_2021/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/Muon_2021
```    
- If data is in IHEP-STORM, check the availability of data in lustre
   The location can be found at /junofs/grid/production/\<outputSubDir>\<softwareVersion>/\<workDir>/\<position>
    
## Deal with failed jobs

### Check the reason of failed jobs ( in 3.4. Monitoring your jobs )
   - Go to the job monitoring page
   - Right click on the line of failed jobs and find "Logging info"  
   - Enter "Logging info" and check "Application Status" to find errors
   - Or you can get jobid of failed jobs, use the command "dirac-wms-job-get-output \<jobid\>" to get detailed logs  
     * app.out, app.err: the logs for running juno offline software
     * Script3_CodeOutput.log: the whole logs also include environment set-up, upload data, etc
   - Notes: If the jobs are in "stalled" status, there is no chance to get back job logs. In this case, the only way to do is to resubmit the job to find the reason.
   
### Rechedule failed jobs with temporary reasons
 Choose the failed jobs in the job monitoring page, use reschedule (only 3 times) or reset (no limited reschedule) button to reschedule the failed jobs
### Resubmit few jobs because of software problems
 Job submission methods and tools refer to the section "4. Job Submission", or you want to resubmit to local farm.

## Finalize and clean 
- Check and fix to be sure that all the data have correct replicas (just in case of some unknown errors in transfers) 
``` 
> ihepdirac-dms-check-replicas --fix DFCDir SEs
DFCDir: /\<outputType>/\<outputSubDir>/\<softwareVersion>/\<workDir>/\<position> defined in ini file
SEs: <moveTargetSE> defined in ini file 
```
- Clean temporary space used in non-target sites
```
> ihepdirac-dms-rm-dir-replicas DFCDir SEs
DFCDir: /\<outputType>/\<outputSubDir>/\<softwareVersion>/\<workDir>/\<position> defined in ini file
SEs: other SEs except <moveTargetSE> 
```
- Put the transformation into "Complete" to release DFC checking
   Go to the transformation page, select the transformation task, use "Complete" button on the left top
   


## Advanced features
### Reuse existing detsim data to do reconstructions 
#### Prepare ini file to define your production parameters
   To get ini template files:  
> ihepdirac-juno-make-reconstruction-sim --example > rec.ini

#### Specify the detsim data directory in DFC by setting metadata:
```
-bash-4.2$ dirac-dms-filecatalog-cli 
Starting FileCatalog client
FC:/> meta set <path> <metaname> <metavalue> [<metaname> <metavalue> ...]- set metadata values for directory or file
```

For example, 
```
meta set /juno/production/ML/prd02_gamma_i dirName /juno/production/ML/prd02_gamma_i
```
#### Define proper tags and inputQuery to identify the input data

The tags and inputQuery will be used to define metadata to get data from DFC (Dirac File Catalogue). You need to define proper inputQuery to specifiy the detsim data to be used.
For example, the following definition in ini file will be convert into meta query (userdata = 0 means no user data):
     tag = gamma_1.0momentums
     dirName = /juno/production/ML/prd02_gamma_i
     application = detsim
     userdata = 0
```
; Depends on which existing tag to be used, format need to be consistent with the existing detsim
tags = gamma_1.0momentums gamma_2.0momentums

; Define which existing detsim data to be used, need to define metadata in DFC first
inputQuery = {"dirName":"/juno/production/ML/prd02_gamma_i","application":"detsim","userdata":"0"}

```
#### Check your production parameters with "--dryrun"
> ihepdirac-juno-make-reconstruction-sim --ini rec.ini --dryrun 
   
#### Submit your production
> ihepdirac-juno-make-reconstruction-sim --ini rec.ini

Note: To avoid conflicts with the metadata defined in existing DFC directory, use a seperated "outputSubDir" from the existing detsim directory

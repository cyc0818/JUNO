# Job Submission



There're two ways of job management with distributed computing. One is to directly use DIRAC, and the other is to use the help of the JSUB tool.



## <font color=green>Submission with vanilla DIRAC</font>



Using DIRAC alone for distributed computing is supported, and here's a brief guide. Please check user guide in DIRAC documentation for more details (https://dirac.readthedocs.io/en/latest/UserGuide/index.html).



Prepare a JDL, define attributes such as executable, arguments, the input sandbox (small files like scripts), input data (large files), output sandbox, output data, and so on.

```
Executable = "/bin/cp";
Arguments = "my.file my.copy";
InputSandbox = {"my.file"};
StdOutput = "std.out";
StdError = "std.err";
OutputSandbox = {"std.out","std.err","my.copy"};
CPUTime = 10;
```



There're command line tools for job submission, status checking, and output retrieval.

```
> dirac-wms-job-submit job.jdl
JobID = 11758

> dirac-wms-job-status 11758
JobID=11758 Status=Waiting; MinorStatus=Pilot Agent Submission; Site=CREAM.CNAF.it;

>  dirac-wms-job-get-output 11702
Job output sandbox retrieved in 11702/
```



A python API is also allowed. In such case, the jobs are described in python:

```
# setup DIRAC
from DIRAC.Core.Base import Script
Script.parseCommandLine(ignoreErrors=False)

from DIRAC.Interfaces.API.Job import Job
from DIRAC.Interfaces.API.Dirac import Dirac

dirac = Dirac()
j = Job()

j.setCPUTime(500)
j.setExecutable('/bin/echo hello')
j.setExecutable('/bin/hostname')
j.setExecutable('/bin/echo hello again')
j.setName('API')

jobID = dirac.submitJob(j)
print 'Submission Result: ',jobID
```

Submission with python scripts:

```
$ python testAPI-Submission.py
2010-10-20 12:05:49 UTC testAPI-Submission.py/DiracAPI  INFO: <=====DIRAC v5r10-pre2=====>
2010-10-20 12:05:49 UTC testAPI-Submission.py/DiracAPI  INFO: Will submit job to WMS
{'OK': True, 'Value': 196}
```



There're also corresponding python script for job monitoring and getting output.



## <font color=green>Job Submission with JSUB</font>



Compared with using vanilla DIRAC, using the JSUB tool can help with manage your jobs.	

The quick start guide has already provided a simple going-through, and you can check it for details. Here we just provide a simple reminder of the procedure:  

* The software can be enabled by sourcing a script to activate a pythonic virtual environment.
*  JSUB needs to be configured by copying the example configuring file to the home directory and modify information such as location of `taskDir`.
* DIRAC environment needs to be sourced after entering the JSUB pythonic virtual environment.
* Use `create` command to parse a TDF, and then use `submit` command to submit it to backend. Use `ls`  and `status` commands as well as IHEP-DIRAC web app to check the status of your jobs. After the jobs has finished, use `getlog` command to retrieve the log files.
* The output files are stored on SEs, they can be found with `dirac-dms-filecatalog-cli` or in corresponding PFN folders.
* Note that it's easy to accumulate too many obsolete files when using distributed computing. Please remove unwanted output files for your tasks frequently.



In the next parts of this section, we will showcase more possibilities with JSUB. A more detailed guide can be found on the website https://jsubpy.github.io/.

### Writing Task Description File 



To create a JSUB task, users need to firstly provide a task description file telling the software what this task is about. The file can be in the format of YAML, TOML ,and JSON, or as a python dictionary.

Some example task description file can be found under the following path:

```
/cvmfs/dcomputing.ihep.ac.cn/frontend/jsub/1.2/install/jsub/examples/juno
```



A TDF should consist of four parts: general setting, backend setting, splitter setting, and workflow setting.

Attributes in general settings include `taskName`,  `experiment`, and `softVersion`. In the software version attribute, you should specify the architecture upon which the JUNO Offline Software is compiled. There're two ways to specify the software version, you can check `101_detsim.yaml` for example.

In the backend setting, you need to specify attributes like the backend `type`, where the data should be put to (`outputDir`/`outputSubDir`), and other settings like `site`, `bannedSites`, and backend `jobGroup`, `jobName`. Check `101_detsim.yaml` for example. By default, the output files would be put under user home directory in Dirac File Catalog (DFC), so you can simply specify the sub-path with `outputSubDir`; alternatively you can use `outputDir` to set the whole path. Aside from `DIRAC` backend, you can also use `local` backend to submit jobs as backend sub-processes on your login node. The backend can be used for quick test of the correctness of your tasks with just a few jobs before mass submitting to backend.

In the splitter settings, you can define how many jobs you want and how the subjob parameters should be. Two splitter mode has been provided by JSUB. One is `splitByEvent`, which is used for the cases like when you are generating a large data sample with the same settings, where the only differences between subjobs are the suffixes of their filenames and the random seeds they use. 

Here is an example with the `splitByEvent` mode: 

```
splitter:
    mode: splitByEvent
    evtMaxPerJob: 10  
    njobs: 5

workflow:
	...

	detsim:
		seed: 1000
		...
```

In this example, a task of generating 50 events has been divided into 5 jobs. In the workflow part, you need to set the random seed of the first job. For the `splitByEvent`  splitter mode, the `seed` is increment for the following subjobs. In the example above, it's 1000 for the first subjob, 1001 for the second one, so on and so forth. The output filenames are also automatically set with suffixing indexes in this splitter mode.



The other splitter mode is `splitByJobvar`. This splitter uses several jobvar lists to describe the parameters of subjobs, giving you more control over your tasks. A jobvar list is a list of subjob-specific parameter generated by various JSUB jobvar extensions, and the splitter allow users to group jobvar lists and combine them accordingly to generate iterative job parameters. 

```
splitter:
    mode: splitByJobvars
    maxSubjobs: 500 
    evtMaxPerJob: 5000 
    jobvar_lists:
        nuclear:
            type: enumerate
            list: ['U-238','Th-232','K-40','Pb-210','C-14','Kr-85']
            group: nuclear  
        subjob:   
            type: range
#            first: 1    ## 1 by default
#            step: 1     ## 1 by default
            length: 20  ## 100000 by default
            group: same_nuclear 
        seed:
            type: range
            first: 10001
#            step: 1


workflow:   
    steps: [detsim]

    detsim:
        seed: '$(seed)'    
        output: '$(nuclear).$(subjob).detsim.root'
        userOutput: '$(nuclear).$(subjob).user.detsim.root'

```

In this example, 20 subjobs are generated for each of 6 nuclears, resulting in 120 jobs in total (below the limit of `maxSubjobs: 500`). One unique seed is given to each of these 120 jobs. The value of these jobvars can be referenced in the workflow setting below. Different from the case with `splitByEvent` mode, the `seed` in workflow setting here is not the starting value for all jobs, but instead the value for the specified job.



Workflow setting defines the chain of action steps to run on the working node. Each action step can range from running the JUNO Offline Software for `detsim`, `elecsim`, `calib`, `rec`, `calib-woelec`, `rec-woelec`, algorithm (`juno_alg`) to running general commandl line (`cmd`), running general scripts (`exe`). You can check the example TDFs for the list of available attributes in the settings of these steps. 



### TDF examples

A simple example of running JUNO application for simulation.

```
taskName: juno_sim
experiment: juno
softVersion: 'centos7_amd64_gcc830/Pre-Release/J20v1r0-Pre2'


backend: 
    type: dirac
    outputSubDir: 'jsub_tests'

splitter:
    mode: splitByEvent 
    evtMaxPerJob: 1000 
    njobs: 50

workflow:
    steps: [detsim] 

    detsim:  
        seed: 1 

```

----



An example of running multiple steps in the same workflow, using the output of the previous steps as the input for the next ones. It is worth noting that each step is running in an independent subprocess, so the modification of environmental variables in one step may not persist into the next ones.

```
taskName: juno_simrec
experiment: juno
softVersion: 'centos7_amd64_gcc830/Pre-Release/J20v1r0-Pre2'

backend: 
    type: dirac
    outputSubDir: 'jsub_tests'

splitter:
    mode: splitByEvent
    evtMaxPerJob: 1000 
    njobs: 50

workflow:
    steps: [detsim, elecsim, calib, rec]  

    detsim: 
        seed: 1 
        particles: e+
        momentums: 1.398    #MeV
        additionalArgs: ''

    elecsim:
        seed: 10
        rate: 1
        additionalArgs: ''
    
    #calib:        # if a step only uses default settings, it's description can be skipped
    #    additionalArgs: ''  
```

----



An example with `splitByJobvar` mode. Combining the jobvars, JSUB would generate 20 subjobs for each the 6 nuclears for this task, and assigning a unique seed for each job. 

The combining rule: 

- Form a sub-set list of the shortest common length within each jobvar group.
- Then, get the Caucasian product of the sub-set list of all different groups.
- Finally, combine this product with the group-less jobvars through within-group combining rule.

```
taskName: juno_sim_jobvar_splitter
experiment: juno
softVersion: 'centos7_amd64_gcc830/Pre-Release/J20v1r0-Pre2'

backend: 
    type: dirac
    outputSubDir: 'jsub_tests'

splitter:
    ## A splitByJobvars generate job variable lists and combine them into sets. For each variable set, the splitter generates one subjob accordingly.
    mode: splitByJobvars 
    maxSubJobs: 500     ## the resulted number of subjobs won't exceed this number
    evtMaxPerJob: 5000
    jobvarLists:
        ## The jobvar lists are grouped. 
        ## For jobvars in the same group, the length of their common var-set list is decided by the shortest jobvar list. 
        ## For jobvar sets in different groups, the combining result is their Cartesian product.
        ## Jobvars without group attribute would make a final common var-set list with the combining result of all jobvar groups.

        ## In this example, there shall be 6*20=120 jobs, each with a unique seed.
        nuclear:
            type: enumerate
            list: ['U-238','Th-232','K-40','Pb-210','C-14','Kr-85']
            group: nuclear  
        subjob:   
            type: range
#            first: 1    ## 1 by default
#            step: 1     ## 1 by default
            length: 20  ## 100000 by default
            group: same_nuclear 
        seed:
            type: range
            first: 10001
#            step: 1

workflow:
    steps: [detsim]

    detsim:
        ## The values of jobvars can be referenced in workflow setting.
        seed: '$(seed)'
        output: '$(nuclear).$(subjob).detsim.root'
        userOutput: '$(nuclear).$(subjob).user.detsim.root'
        additionalArgs: 'gendecay --nuclear $(nuclear) --volume pTarget --material LS'

        ## Alternatively, use fullArgs to define all settings above. FullArgs is what is directly served to JUNO scripts as parameters,
        #fullArgs: '--evtmax 5000 --seed $(seed) --output $(nuclear).$(subjob).detsim.root --user-output $(nuclear).$(subjob).user.detsim.root gendecay --nuclear $(nuclear) --volume pTarget --material LS'


        ## By default, any output files matching the wildcard *STEP_NAME* will be uploaded to DFC. You may also tag additional files for uploads here.
        #outputUpload: '*detsim*, *detsim_user*'    

```

----

An example of doing `elecsim` step alone, with downloaded data. `splitByJobvars`  splitter mode is required to assign the name of input files.

```
taskName: juno_elecsim
experiment: juno
softVersion: 'centos7_amd64_gcc830/Pre-Release/J20v1r0-Pre2'

backend: 
    type: dirac
    outputSubDir: 'jsub_tests/'

splitter:
    ## For jobs with input, splitByJobvars splitter is necessary so that the input filenames can be referenced in workflow setting
    mode: splitByJobvars 
    maxSubJobs: 500  
    evtMaxPerJob: 5000
    jobvarLists:
        ## One way to assign input file is to list the logical filenames (LFN) in a text file.
#       input_filename:
#           type: lines_in_file 
#           file: './lfnlist.example'   

        ## Another way is to filter LFN list in a given folder, using dirac-dms-find-lfns command
        input_filename:
           type: find_lfns
           path: '/juno/user/.../test'
           metaspec:   ' "Size>1000" "CreationDate>2010-01-01" ' # metadata index specification
        seed:
            type: range
            
workflow:
    steps: [elecsim]

    elecsim: 
        seed: '$(seed)'  # jobvars can be referenced in some workflow settings
        input: '$(input_filename)'                  # if a path is given, would omit dirname
        output: 'elecsim.$(seed).root'              # same
        userOutput: 'elecsim.user.$(seed).root'     # same
        rate: 10
        additionalArgs: ''

        ## It's also possible to use fullArgs here. But it's noteworthy that the input/output filenames should be set under './', because that's where the driver scripts look for files on the DIRAC working nod
es.        
		## If input/output/user_output attributes are designated with

```

----

Another example with `elecsim` to showcase `splitByJobvars` splitter mode. Jobvars can work like variables with pythonic `eval`, and they're resolved in the order of their priorities. 

```
experiment: juno
softVersion: 'centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre2'

backend: 
    type: dirac
    outputSubDir: 'jsub_tests/'

splitter:
    ## For jobs with input, splitByJobvars splitter is necessary so that the input filenames can be referenced in workflow setting
    mode: splitByJobvars 
    maxSubJobs: 500  
    jobvarLists:
        input_LFN:
           ## can use 'jsub jobvar' command to check the values of this jobvar list
           type: find_lfns
           path: '/juno/production/ML/prd03_r'
           metaspec: ' "application=detsim" "tag=e+_0.0momentums" "softwareVersion=centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre2"' 
        seed:
            type: range
        elecsim_LFN:
            ## pythonic eval,  .../detsim/.../*detsim*root --> .../elecsim/.../*elecsim*root
            type: eval
            value: 'input_LFN.replace("detsim","elecsim")' 
            priority: 2  ## from priority 9->0, the values of the eval/string jobvars are resolved in order.

workflow:
    steps: [elecsim]

    elecsim: 
        evtMax: 100 
        rate: 1
        seed: "$(seed)"
        input: "$(input_LFN)"
        ## Can use outputLFN instead of output to assign full path, putting files into complex folder structures.
        outputLFN: "/juno/user/.../$(elecsim_LFN)"    # output to this LFN
        userOutput: "elecsim_user-$(seed).root"      # user output put under the DIRAC output dir

```

----

An example for running JUNO analysis.

The compiled `.so` file of the algorithm and the job configuration file shall be uploaded. To allow users to put subjob-varying parameters into their configuration, a `textReplace` method is provided. Also, users can define what output files they want to upload onto SEs, wildcards for filenames are supported.

```
## ----------------------------------------
## demo for juno analysis
## ----------------------------------------
taskName: juno_custom_Alg
experiment: juno
softVersion:
    arch: 'centos7_amd64_gcc830'
    release: 'J20v1r0-Pre2'

backend:
    type: dirac
    outputSubDir: 'jsub_tests'

splitter:
    mode: splitByJobvars
    maxSubJobs: 20
    evtMaxPerJob: 1000

    jobvarLists:
        idx:
            type: range
            length: 10

workflow:
    steps: [myAlg]

    myAlg:
        type: 'juno_alg'
        # Users shall provide a job configuration file template for the algorithm and the referenced DLLs.
        # These files would be put into input sandbox. 
        # The folders of Sniper.LoadDll() in the config would be auto-redirected.
        soFile: 
            - './JsubDummyAlg/amd64_linux26/libJsubDummyAlg.so'    
            - './JsubHelloWorld/amd64_linux26/libJsubHelloWorld.so'
#       jobConfig: './JsubDummyAlg/share/run.py'   # local position
        jobConfig: './JsubHelloWorld/share/run.py'   # local position
        
        # Users may use case-sensitive text replacement to set subjob-dependent parameters.
        textReplace: 
            # keyword: replacement
            OUTPUT1: 'a/output.$(idx).1.root'
            OUTPUT2: 'b/output.$(idx).2.root'
        # what files to be uploaded as output data, for (dirac backend)
        outputUpload:
            - '*root'
        

```





### Job Management Commands



Help command:

```
jsub --help
jsub create --help
...
```

----



Task creation:

```
jsub create <task-description-filename> 
```

When successful, the command would return the information of the created JSUB task, including its ID.



```
Task created successfully
- ID         : 3
- Name       : juno_sim
- Job Number : 5
```



----





Task submission:

```
jsub submit <task-id> [-r]
```

The `-r` option is used for enforcing resubmit (same as `jsub resubmit <task-id>`), it would delete all the current running jobs of the task on backend and resubmit new jobs, while doing nothing for the output data of the previous jobs that has finished running. 

When successful, the command would print the succeeding message.

```
Submitting task 3
[2021-07-15 07:18:04.282 +0800 CST][JSUB|INFO]: 5 jobs successfully submitted to backend.
```



----



Command to list all your JSUB tasks:

```
jsub ls [-u/-n]
```

The command would list all tasks and their information of task name, task ID, experiment,  backend, backend status, creation time, and info-update time.

```
Task ID  Name         Experiment  Backend  Status (D|F|R|W|O)  Creation Time (UTC)  Info Updated (UTC)   
-------- ------------ ----------- -------- ------------------- -------------------- -------------------- 
1        my_juno_sim  juno        dirac    0|0|50|0|0          2021-07-12 21:18:22  2021-07-12 21:41:13  
2        my_juno_sim  juno        dirac    0|0|50|0|0          2021-07-12 21:28:26  2021-07-12 21:41:15  
3        juno_sim     juno        dirac    0|0|0|5|0           2021-07-14 23:15:06  2021-07-14 23:20:06
```

By default, the command would let JSUB spend some time communicating with backends to fetch the updated status of your tasks one by one. It can be runned with `-n` option to disable auto-update to accelerate.



----



To check whether the setting of a task is right, there are two commands to use:

```
jsub show <task-id> [-d DUMP_PATH]
jsub jobvar <task-id> -i <sub_id>
jsub jobvar <task-id> -j <jobvar_name>
```

The `jsub show` command prints the TDF file of the task after parsing, to allow you to check if the settings are right. It can run with `-d` option to dump the TDF to the given path, for modification and re-creation of the task. The `jsub jobvar` command can be used to test if the job variable list of have correct values.



----



Task monitoring commands:

```
jsub status -s <status_name>
jsub reschedule [-d/-f/-r/-w] [-i sub_id] [-b backend_id]
```

The commands can be used to get the statuses of selected jobs, and to reschedule the selected jobs. The reschedule command support the selection of jobs with Done/Failed/Running/Waiting status, or the specification of JSUB subjob IDs or the backend subjob IDs, separated with commas. 



----



Command to fetch log files:

```
jsub getlog <task-id> 
jsub getlog <task-id> -s Done [-n njobs]  # njobs = max number of subjobs for log fetching
jsub getlog <task-id> -s Failed
jsub getlog <task-id> -i 1,2,3,4
```

The log files are stored in the path `$(jsub-taskDIR)/$(task-id)/logfiles/`. 



----



Other commands:

```
jsub remove [-f]  # removing a JSUB task; -f to enforce removing a submit task
jsub version  	  # show the version of JSUB
jsub package      # show what package is loaded
```


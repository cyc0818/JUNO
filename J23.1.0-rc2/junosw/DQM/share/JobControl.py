#!/usr/bin/env python

'''
Input file name must be "run num _ files num _ events num _ id"
./JobControl.py -i test.root -m /public/data/zhangshu/DQM/client/test -o /public/data/zhangshu/DQM/client/test/output -v J19v1r1-Pre4 
'''
global WAIT_TIME
global REC_RATE
REC_RATE=1 # 0.1 is 10%
WAIT_TIME=99999 # per 30s

import os, sys, time

def configure(argv=[]):
    from optparse import OptionParser
    parser = OptionParser()
    
    parser.add_option("-d","--debug",type=int,default=1,
                    help="Print debug information.")
    parser.add_option("-i","--input-file",type=str,default="test.root",
                    help="Input files name.")
    parser.add_option("-m","--monitor-path",type=str,default="/junofs/users/zhangshu/DQM/test",
                    help="Monitor path.")
    parser.add_option("-o","--output-path",type=str,default="/junofs/users/zhangshu/DQM/output",
                    help="Output path.")
    # parser.add_option("-r","--run-num",type=int,default=0,
    #                 help="Run number.")
    # parser.add_option("-n","--files-num",type=int,default=1,
    #                 help="file numbers of this run")
    # parser.add_option('-e',"--event-num",type=int,default=1,
    #                 help='Event numbers of this input file')
    parser.add_option('-g',"--gdml-file",type=str,default="/junofs/users/zhangshu/DQM/offline/geo_J20v1r0-Pre2_no3inch.gdml",
                    help="")
    parser.add_option('-v',"--offline",type=str,default="J20v1r0-Pre2",
                    help="Offline version")
    parser.add_option('',"--dqmclient-path",type=str,default="/junofs/users/zhangshu/DQM/script",
                    help="DQM client path")
    # parser.add_option('-m',"--recMap",type=str,default="/junofs/users/zhangshu/DQM/RecMap")
    parser.add_option("--QTMLEAlg",type=str,default="/junofs/users/zhangshu/DQM/QTMLEAlg")
    
    (options,args) = parser.parse_args(args=argv)
    return options


        

class Job:
    
    def __init__(self,options):

        self.options = options
        print "--------- Start a new job for : %s ----------" %self.options.input_file
        
        self.input_name = self.options.monitor_path + '/' + self.options.input_file
        if os.path.exists(self.input_name) is None:
            print "Can not find file : %s" %self.options.input_file
            sys.exit(1)
    
    def GetInputInfo(self):
        
        inputnamelist = self.options.input_file.split('_',4)
        self.eventType = inputnamelist[0]
        self.run_num = inputnamelist[1]
        self.files_num = inputnamelist[2]
        self.event_num = inputnamelist[3] * REC_RATE
        self.file_id = inputnamelist[4]
        
        # get sim_user_rootfile for energy rec toy mode
        # self.simUSER_OUTPUT = '/junofs/users/zhangshu/DQM/offline/user_' + self.run_num + '_' + self.files_num + '_root'
        
    def SetOutputName(self):
        
        self.job_path = self.options.output_path + '/' + str(self.run_num) + '/job'
        self.rec_path = self.options.output_path + '/' + str(self.run_num) + '/rec'
        self.cail_path = self.options.output_path + '/' + str(self.run_num) + '/cail'
        self.elec_path = self.options.output_path + '/' + str(self.run_num) + '/elec'
        self.log_path = self.options.output_path + '/' + str(self.run_num) + '/log'
        self.hist_path = self.options.output_path + '/' + str(self.run_num) + '/hist'
        
        if os.path.exists(self.job_path) is False :
            os.makedirs(self.job_path,493)
        if os.path.exists(self.rec_path) is False :
            os.makedirs(self.rec_path,493)
        if os.path.exists(self.cail_path) is False :
            os.makedirs(self.cail_path,493)
        if os.path.exists(self.elec_path) is False :
            os.makedirs(self.elec_path,493)
        if os.path.exists(self.log_path) is False :
            os.makedirs(self.log_path,493)
        if os.path.exists(self.hist_path) is False :
            os.makedirs(self.hist_path,493)
            
        self.job_name = self.job_path + '/' + self.eventType + "_" + self.file_id + '.sh'
        self.subjob_name = self.job_path + '/sub_' + self.eventType + "_" + self.file_id

        self.log_job = self.job_path + '/' + self.eventType + "_" + self.file_id + '.log'
        self.log_hist_name = self.log_path + '/hist_' + self.eventType + "_" + self.file_id + '.log'
        self.log_rec_V_name = self.log_path + '/rec_V_' + self.eventType + "_" + self.file_id + '.log'
        self.log_rec_E_name = self.log_path + '/rec_E_' + self.eventType + "_" + self.file_id + '.log'
        self.log_cail_name = self.log_path + '/cail_' + self.eventType + "_" + self.file_id + '.log'
        self.log_elec_name = self.log_path + '/elec_' + self.eventType + "_" + self.file_id + '.log'

        self.rec_V_name = self.rec_path + '/rec_V_' + self.eventType + "_" + self.file_id
        self.rec_V_user_name = self.rec_path + '/rec_V_user_' + self.eventType + "_" + self.file_id
        self.rec_E_name = self.rec_path + '/rec_E_' + self.eventType + "_" + self.file_id
        self.rec_E_user_name = self.rec_path + '/rec_E_user_' + self.eventType + "_" + self.file_id
        self.cail_name = self.cail_path + '/cail_' + self.eventType + "_" + self.file_id
        self.cail_user_name = self.cail_path + '/cail_user_' + self.eventType + "_" + self.file_id
        self.elec_name = self.elec_path + '/elec_' + self.eventType + "_" + self.file_id
        self.elec_user_name = self.elec_path + '/elec_user_' + self.eventType + "_" + self.file_id
        self.hist_name = self.hist_path + '/hist_' + self.eventType + "_" + self.file_id
        
        self.finish_name = self.log_path + '/' + self.eventType + "_" + 'finish.log' 
        self.log_eventrate = self.log_path + '/' + self.eventType + "_" + 'eventrate.log'
        self.log_todisplay = self.options.output_path + '/' + self.eventType + "_" + 'todisplay.log'
        
        
        if os.path.exists(self.finish_name) is False:
            f = open(self.finish_name,'w')
            f.close()
            os.system('chmod 755 ' + self.finish_name)
        
        if os.path.exists(self.log_eventrate) is False:
            f = open(self.log_eventrate,'w')
            f.close()
            os.system('chmod 755 ' + self.log_eventrate)
            
        if os.path.exists(self.log_todisplay) is False:
            f = open(self.log_todisplay,'w')
            f.close()
            os.system('chmod 755 ' + self.log_todisplay)
    
    def GetEventRate(self):
        
        f = open(self.log_eventrate,'a')
        f.write(self.event_num + '\n')
        f.close()
        
        
    def SubmitJob(self):
        
        fjob = open(self.job_name,'w')
        fjob.write('#!/bin/bash -' + '\n')
        
        if self.eventType == 'cd':
            fjob.write('export CMTPATH=/junofs/users/zhangshu:$CMTPATH' + '\n')
            fjob.write('setup=/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/' + self.options.offline + '/setup.sh' + '\n')
            fjob.write('script=/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/' + self.options.offline + '/offline/Examples/Tutorial/share' + '\n')
            # fjob.write('setup=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/' + self.options.offline + '/setup.sh' + '\n')
            # fjob.write('script=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/' + self.options.offline + '/offline/Examples/Tutorial/share' + '\n')
            fjob.write('source ${setup}' + '\n')

            # for radioactivity background
            fjob.write('(time python ${script}/tut_det2elec.py --evtmax ' + str(int(self.event_num)*2) + ' --input ' + self.input_name + ' --output ' + self.elec_name + ' --user-output ' + self.elec_user_name + ') >& ' + self.log_elec_name + '\n')
            fjob.write('(time python ${script}/tut_elec2calib.py --evtmax ' + str(int(self.event_num)*2) + ' --input ' + self.elec_name + ' --output ' + self.cail_name + ' --user-output ' + self.cail_user_name + ') >& ' + self.log_cail_name + '\n')
            fjob.write('(time python ${script}/tut_calib2rec.py --evtmax -1 --input ' + self.cail_name + ' --output ' + self.rec_V_name + ' --user-output ' + self.rec_V_user_name + ' --elec yes --SwitchAlg 1 --gdml --gdml-file ' + self.options.gdml_file + ') >& ' + self.log_rec_V_name + '\n')
        
            fjob.write('export CMTPATH=/junofs/users/zhangshu/DQM:$CMTPATH' + '\n')
            fjob.write('source ' + self.options.QTMLEAlg + '/OMILREC/cmt/setup.sh' + '\n')
            #use QMLE
            fjob.write('(time python ' + self.options.QTMLEAlg + '/OMILREC/share/tut_calib2rec.py --evtmax -1 --input ' + self.cail_name + ' --output ' + self.rec_E_name + ' --user-output ' + self.rec_E_user_name + ' --method energy-point --enableUpdatePMTPara --enableTimeOffSetCorr --enableLTSPEs --SignalWindow 280  --LPMTCalibEnergy 2.223 --SPMTCalibEnergy 2.223 --RecMapPath ' + self.options.QTMLEAlg + '/RecMap --enableUseTimeRecVertex --elec yes --gdml --gdml-file ' + self.options.gdml_file + ') >& ' + self.log_rec_E_name + '\n')
            #use QTMLE
            # fjob.write('(time python ' + self.options.QTMLEAlg + '/OMILREC/share/tut_calib2rec.py --evtmax -1 --input ' + self.cail_name + ' --output ' + self.rec_E_name + ' --user-output ' + self.rec_E_user_name + ' --method energy-point --enableUpdatePMTPara --enableTimeOffSetCorr --enableLTSPEs --SignalWindow 280  --LPMTCalibEnergy 2.223 --SPMTCalibEnergy 2.223 --RecMapPath ' + self.options.QTMLEAlg + '/RecMap  --enableTimeInfo --elec yes --gdml --gdml-file ' + self.options.gdml_file + ') >& ' + self.log_rec_E_name + '\n')
            
            #for e+
            # fjob.write('(time python ${script}/tut_det2calib.py --evtmax $EVTMAX --input $INPUT_NAME --output $CAIL_NAME --detoption Acrylic) >& $LOG_CAIL' + '\n')
            # fjob.write('(time python ${script}/tut_calib2rec.py --evtmax $EVTMAX --input $CAIL_NAME --output $REC_NAME --user-output $REC_USER_NAME --gdml --SwitchAlg 1 --detoption Acrylic) >& $LOG_REC' + '\n')
        
        elif self.eventType == 'wp':
            
        elif self.eventType == 'tt':
            fjob.write('setup=/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J19v1r1-Pre4/setup.sh' + '\n')
            fjob.write('script=/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J19v1r1-Pre4/offline/Examples/Tutorial/share' + '\n')
            fjob.write('source ${setup}' + '\n')
            fjob.write('(time python ${script}/tut_det2calib.py --evtmax -1 --input ' + self.input_name + ' --output ' + self.cail_name + ') >& ' + self.log_cail_name + '\n')
            fjob.write('(time python ${script}/tut_calib2rec.py --evtmax -1 --input ' + self.cail_name + ' --output ' + self.rec_V_name + ' --user-output ' + self.rec_V_user_name + ' --detoption TT --gdml --gdml-file /junofs/users/zhangshu/DQM/offline/geo_J19v1r1-Pre4_no3inch.gdml --method tt-track ) >& ' + self.log_rec_V_name + '\n' )
            
        else:
            print 'unkown event type !!!!!!'
            sys.exit(1)   
        
        fjob.write('echo "' + self.cail_name + ' ' + self.rec_V_name + '" > ' + self.log_todisplay + '\n')
        fjob.write('(echo "-------- start draw hist -------"' + '\n')
        fjob.write('source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre2/setup.sh')
        # fjob.write('mergefile=${HIST_NAME%\'/\'*}/${RUN_NUM}.root' + '\n')
        fjob.write('mergefile=' + self.hist_path + '/' + str(self.eventType) + '_' + str(self.run_num) + '.root' + '\n')
        # fjob.write(r'root -l -b -q "${DQMCLIENT_PATH}/histo.c(0,\"${REC_NAME}\",\"${mergefile}\",\"${HIST_NAME}\")"')
        fjob.write('time ' + self.options.dqmclient_path + '/histo 0 ' + self.eventType + ' ' + self.rec_V_user_name + ' ' + self.rec_E_user_name + ' ' + self.hist_name + '\n')
        fjob.write('\n')
        fjob.write('histfilesize=$(wc -c ' + self.hist_name + ')' + '\n')
        fjob.write('if [ ! -f "' + self.hist_name + '" ] || [ ${histfilesize%%\' \'*} -lt 600 ]; then' + '\n')
        fjob.write('    echo "draw ' + self.hist_name + ' failed !!!!!!!"' + '\n')
        fjob.write('    if [ -f "' + self.hist_name + '" ]; then' + '\n')
        fjob.write('        rm -f ' + self.hist_name + '\n')
        fjob.write('    fi' + '\n')
        fjob.write('    exit -1' + '\n')
        fjob.write('else' + '\n')
        fjob.write('    echo "' + self.hist_name + '" >> ' + self.finish_name + '\n')
        fjob.write('fi' + '\n')
        
        fjob.write('\n\n')
        fjob.write('done_num=$(wc -l ' + self.finish_name + ')' + '\n')
        fjob.write('if [ ${done_num%%\' \'*} = 1 ]; then' + '\n')
        fjob.write('    echo "-------- this is the first finished job of this run, will wait other jobs --------"' + '\n')
        fjob.write('    loop=0' + '\n')
        fjob.write('    while [ ${done_num%%\' \'*} != ' + str(self.files_num) + ' ]' + '\n')
        fjob.write('    do' + '\n')
        fjob.write('        echo " only ${done_num%%\' \'*} files are done, wait 60s "' + '\n')
        fjob.write('        loop=$(($loop+1))' + '\n')
        fjob.write('        if [ ${loop} -eq ' + str(WAIT_TIME) +' ]; then' + '\n')
        fjob.write('            echo " too many time to wait, force to merge !!!!!!!!! "' + '\n')
        fjob.write('            break' + '\n')
        fjob.write('        fi' + '\n')
        fjob.write('        sleep 30' + '\n')
        fjob.write('        done_num=$(wc -l ' + self.finish_name + ')' + '\n')
        fjob.write('    done' + '\n')
        fjob.write('    echo "-----start merge-----"' + '\n')
        fjob.write('    echo "${done_num%%\' \'*} files are merged"' + '\n')
        fjob.write('    time ' + self.options.dqmclient_path + '/histo 1 ' + self.eventType + ' ${mergefile} ' + self.finish_name + ' ' + self.log_path + '\n')
        # fjob.write(r'    root -l -b -q "${DQMCLIENT_PATH}/histo.c(1,\"${REC_NAME}\",\"${mergefile}\",\"${FINISH_NAME}\")"')
        fjob.write('\n')
        fjob.write('    mergefilesize=$(wc -c ${mergefile})' + '\n')
        fjob.write('    if [ ! -f "${mergefile}" ] || [ ${mergefilesize%%\' \'*} -lt 600 ]; then' + '\n')
        fjob.write('        echo "merge hist ${mergefile} failed !!!!!!!"' + '\n')
        fjob.write('        exit -1' + '\n')
        fjob.write('    else' + '\n')
        fjob.write('        echo "merge successfully"' + '\n')
        fjob.write('    fi' + '\n')
        fjob.write('else' + '\n')
        fjob.write('    echo "there is already merging job"' + '\n')
        fjob.write('fi' + '\n')
        fjob.write('\n')
        fjob.write('echo "Job is done") >& ' + self.log_hist_name + '\n')
        fjob.write('exit 0' + '\n')
        fjob.close()
        os.system('chmod 755 ' + self.job_name)
        
        # jobsum=os.popen('condor_q zhangshu -format "%s" Owner -format " %s " Args -format " ProcId = %d\n" ProcId | wc -l')
        # while int(jobsum.read()) >= 100:
        #     print "submit job failed!! number of running jobs is full , wait 30s"
        #     time.sleep(15)
        #     jobsum=os.popen('condor_q zhangshu -format "%s" Owner -format " %s " Args -format " ProcId = %d\n" ProcId | wc -l')
        
        # os.system('condor_submit zhangshu -name job@schedd04.ihep.ac.cn ' + self.subjob_name)
        if self.eventType == 'cd':
            os.system('export PATH=/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin:$PATH && hep_sub -os SL6 -wt mid ' + self.job_name)
        if self.eventType == 'tt':
            os.system('export PATH=/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin:$PATH && hep_sub -os SL6 -wt short ' + self.job_name)

        
        
if __name__ == "__main__":
    options = configure(sys.argv)
    job = Job(options)
    job.GetInputInfo()
    job.SetOutputName()
    job.GetEventRate()
    job.SubmitJob()
        
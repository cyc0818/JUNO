import multiprocessing as mp


def monitoring(q):
    import pyinotify

    class MyEvent(pyinotify.ProcessEvent):
        def process_IN_CREATE(self,event):
            file = "%s" %event.name
            print "new file : %s" %file
            q.put(file)
            # log = '/junofs/users/zhangshu/DQM/output/' + file + '.log'
            # args = './run.sh'+' '+file
            # args = '(python JobControl.py -m /junofs/users/zhangshu/DQM/test -o /junofs/users/zhangshu/DQM/output -i ' + file + ') >& ' + log
            # args = 'python JobControl.py -m /junofs/users/zhangshu/DQM/test -o /junofs/users/zhangshu/DQM/output -i ' + file
            # child = subprocess.Popen(args,shell=True)
            
            
    def moni(path):
        wm = pyinotify.WatchManager()
        wm.add_watch(path,pyinotify.IN_CREATE,rec = True)
        ev=MyEvent()
        notifier = pyinotify.Notifier(wm,ev)
        notifier.loop()

    path = '/public/data/zhangshu/DQM/client_0224/test'
    print "start monitoring %s" %path
    moni(path)


def jobsubmit(q):
    import subprocess
    import os, time
    
    while True:
        file = q.get()
        args = 'python JobControl.py -m /public/data/zhangshu/DQM/client_0224/test -o /public/data/zhangshu/DQM/client_0224/output -i ' + file
        jobsum=os.popen('condor_q zhangshu -format "%s" Owner -format " %s " Args -format " ProcId = %d\n" ProcId | wc -l')
        while int(jobsum.read()) >= 100:
            print "submit job failed!! number of running jobs is full , wait 30s"
            time.sleep(15)
            jobsum=os.popen('condor_q zhangshu -format "%s" Owner -format " %s " Args -format " ProcId = %d\n" ProcId | wc -l')
        child = subprocess.Popen(args,shell=True)
        print "file :%s is submited" %file

if __name__ == '__main__':
    q = mp.Queue()
    monitor = mp.Process(target=monitoring,args=(q,))
    jobsub = mp.Process(target=jobsubmit,args=(q,))
    monitor.start()
    jobsub.start()
    monitor.join()
    jobsub.join()
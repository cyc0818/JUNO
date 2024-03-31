import pyinotify
import subprocess

class MyEvent(pyinotify.ProcessEvent):
        def process_IN_CREATE(self,event):
                file = "%s" %event.name
                log = '/junofs/users/zhangshu/DQM/output/' + file + '.log'
                # args = './run.sh'+' '+file
                # args = '(python JobControl.py -m /junofs/users/zhangshu/DQM/test -o /junofs/users/zhangshu/DQM/output -i ' + file + ') >& ' + log
                args = 'python JobControl.py -m /junofs/users/zhangshu/DQM/test -o /junofs/users/zhangshu/DQM/output -i ' + file
                child = subprocess.Popen(args,shell=True)
                
def monitor(path):
    wm = pyinotify.WatchManager()
    wm.add_watch(path,pyinotify.IN_CREATE,rec = True)
    ev=MyEvent()
    notifier = pyinotify.Notifier(wm,ev)
    notifier.loop()

path = '/junofs/users/zhangshu/DQM/test'
print "start monitoring %s" %path
monitor(path)

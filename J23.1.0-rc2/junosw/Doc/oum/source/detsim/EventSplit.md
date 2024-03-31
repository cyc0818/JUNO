# Event Split Method
In this part, we will introduce the method of event split.
## Background of Event Split
When we do Muon simulation to do some research, we find the job of Muon simulation always crash at both local platform and distributed computing sites(2.5GB~4GB). After a lots of memory tests of Muon simulation, we find the reason why Muon simulation crash is that ROOTIO double the memory need . If you want to know more about that, please look up Doc 6717-v4.
## How to use this method

We use dateModelWriterWithSplit to perform both split and unsplit IO tasks in the (since the version of J21). If you use the unsplit output, you don't need to add extra arguments, as in the previous version.For example,
```
python  $TUTORIALROOT/share/tut_detsim.py --evtmax 10  gun
```

If you want to use split output, just add the option `--event-split`. For example: 
```
python  $TUTORIALROOT/share/tut_detsim.py --evtmax 10  --event-split  gun
```
Then, IO task will execute when the number of hits of one event reaches 10000 (this is the default split hits number). However you can also specify the split hits number by adding the option `--event-split-maxhits`.

## Reducing memory effectively in combination with other methods
However, we have to say that just use the event split method can't reduce memory effectively. You'd better use the merge of time window and the format of muon-hit-type. Ok, when you use these 3 methods, Congratulation to you that you will have no memory worries in the detector simulation. Now I will give you an example of how to combine these 3 methods to solve the memory problem:
```
python  $TUTORIALROOT/share/tut_detsim.py  --pmt-hit-type 2 --pmtsd-merge-twindow 1.0 --event-split --evtmax 10 gun
```
`--pmt-hit-type 2` this arguement means you choose the muon hit type which is simplified. The default value is 1, and it means the normal hit type.
`--pmtsd-merge-twindow 1.0` this arguement means that you will merge the pmt hits which are in the 1 ns timewindow. 

  

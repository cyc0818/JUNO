
##############################################################################
# Module definition
##############################################################################

from .JUNOModule import JUNOModule
from .JUNOUtils import mh

class JUNOPSDPreSelectModule(JUNOModule):
    def __init__(self):
        pass

    def register_options(self, parser):
        tool_args = parser.add_argument_group(mh("classifier tools"), mh("classifiers"))
        tool_args.add_argument("--classifier-afterpulse", action="store_true", dest="classifier_afterpulse", help=mh("enable afterpulse tool to classifier afterpulse like event (Default is enabled)"))
        tool_args.add_argument("--no-classifier-afterpulse", action="store_false", dest="classifier_afterpulse", help=mh("disable afterpulse tool(Default is enabled)"))
        tool_args.set_defaults(classifier_afterpulse=True)
        
        tool_args.add_argument("--afterpulse-interval-time", default=None, type=float)
        tool_args.add_argument("--afterpulse-energy-ratio",default =None, type = float)

        # energy
        tool_args.add_argument("--classifier-energy", action="store_true", dest="classifier_energy", help=mh("enable energy select tool to classifier event (Default is disabled)"))
        tool_args.add_argument("--no-classifier-energy", action="store_false", dest="classifier_energy", help=mh("disable the tool"))
        tool_args.set_defaults(classifier_energy=False)
        
        tool_args.add_argument("--energy-range-max", default = None, type = float)
        tool_args.add_argument("--energy-range-min", default = None, type = float)

        #fiducial volume
        tool_args.add_argument("--classifier-radius", action="store_true", dest="classifier_radius", help=mh("enable fiducial volume tool to classifier  event (Default is disabled)"))
        tool_args.add_argument("--no-classifier-radius", action="store_false", dest="classifier_radius", help=mh("disable (Default is enabled)"))
        tool_args.set_defaults(classifier_radius = False)
        
        tool_args.add_argument("--radius-max", default = None,  type = float) # mm
        tool_args.add_argument("--radius-min", default = None, type = float) # mm
        
        #prompt signal
        
        tool_args.add_argument("--classifier-prompt-signal", action="store_true", dest="classifier_prompt", help=mh("select the prompt signal from the prompt-signal correlated sample (Default is disabled)"))
        tool_args.add_argument("--no-classifier-prompt-signal", action="store_false", dest="classifier_prompt", help=mh("disable this tool"))
        tool_args.set_defaults(classifier_prompt=False)
        
        tool_args.add_argument("--classifier-delayed-signal", action="store_true", dest="classifier_delayed", help=mh("select the delayed signal from the delayed-signal correlated sample (Default is disabled)"))
        tool_args.add_argument("--no-classifier-delayed-signal", action="store_false", dest="classifier_delayed", help=mh("disable this tool"))
        tool_args.set_defaults(classifier_delayed=False)
        # single
        tool_args.add_argument("--classifier-single", action="store_true", dest="classifier_single", help=mh("select the single signal (Default is disabled)"))
        tool_args.add_argument("--no-classifier-single", action="store_false", dest="classifier_single", help=mh("disable this tool"))
        tool_args.set_defaults(classifier_single=False)
        
        pass

    def init_psd_steering_alg(self,task, args):
         
        import PSDPreSelectAlg
        import PSDClassification
        psd_preselect_alg = task.createAlg("PSDPreSelectAlg")
        
        if args.classifier_afterpulse:
            psd_preselect_alg.property("ClassifierList").append("AfterPulseLikeClassifier")
            tool = psd_preselect_alg.createTool("AfterPulseLikeClassifier")
            if args.afterpulse_interval_time:
                tool.property("IntervalTime").set(args.afterpulse_interval_time)
            if args.afterpulse_energy_ratio:
                tool.property("EnergyRatio").set(args.afterpulse_energy_ratio)
        else:
            if (args.afterpulse_interval_time != None ) or (args.afterpulse_energy_ratio != None ):
                assert 0 , "please add the option --classifier-afterpulse "
        
        if args.classifier_energy:
            print("args.classifier_energy has register.")
            psd_preselect_alg.property("ClassifierList").append("EnergyRangeClassifier")
            tool = psd_preselect_alg.createTool("EnergyRangeClassifier")
	
            print("EnergyRangeMax = {}".format(args.energy_range_max))
            print("EnergyRangeMin = {}".format(args.energy_range_min))

            if args.energy_range_max != None :
                print("EnergyRangeMax has register.")
                tool.property("EnergyRangeMax").set(args.energy_range_max)
            if args.energy_range_min != None:
                tool.property("EnergyRangeMin").set(args.energy_range_min)
        else:
            if( ( args.energy_range_max != None ) or (args.energy_range_min != None)):
                assert 0  , "please add the option --classifier-energy "            

        if args.classifier_radius:
            psd_preselect_alg.property("ClassifierList").append("FiducialVolumeClassifier")
            tool = psd_preselect_alg.createTool("FiducialVolumeClassifier")
            if args.radius_max != None:
                tool.property("RadiusMax").set(args.radius_max)
            if args.radius_min != None:
                tool.property("RadiusMin").set(args.radius_min)
        else:
            if( ( args.radius_max != None ) or (args.radius_min != None)):
                assert 0  , "please add the option --classifier-radius "            


        if args.classifier_prompt:
            psd_preselect_alg.property("ClassifierList").append("PromptSignalClassifier")
            psd_preselect_alg.createTool("PromptSignalClassifier")

        if args.classifier_delayed:
            psd_preselect_alg.property("ClassifierList").append("DelayedSignalClassifier")		
            psd_preselect_alg.createTool("DelayedSignalClassifier")

        if args.classifier_single:
            psd_preselect_alg.property("ClassifierList").append("SingleClassifier")		
            psd_preselect_alg.createTool("SingleClassifier")
        
		
    def init(self, toptask, args):
        # Create a python store
        import SniperPython
        toptask.createSvc("PyDataStoreSvc/DataStore")
        import BufferMemMgr
        bufMgr = toptask.createSvc("BufferMemMgr")
        bufMgr.property("TimeWindow").set([-0.1, 0.1]);
        import EventRecordSvc
        toptask.createSvc("EventRecordSvc")
        self.init_psd_steering_alg(toptask, args)

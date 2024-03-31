#!/usr/bin/env python

import CondDB
import base64
class Py_CD_LS_MatProp_Cnv(CondDB.ICnvFromPayload):
    
    def __init__(self, self_):
        CondDB.ICnvFromPayload.__init__(self)
        
        self.self_ = self_

        self.tag_begin = "-----"
        self.tag_end = "....."

    def cnv(self, payload):
        # print "got %s"%payload.data()
        decoded = base64.b64decode(payload.data())
        # print "decoded: %s"%decoded

        return self.load(decoded)

    def load(self, decoded):
        curkey = ""

        isfirst = {}

        for line in decoded.splitlines():
            line = line.strip()
            if len(line) == 0:
                continue

            if line.find(self.tag_begin) != -1:
                curkey = self.get_key(line, self.tag_begin)
                # print "Begin curkey: %s"%curkey
                continue
            if line.find(self.tag_end) != -1:
                key = self.get_key(line, self.tag_end)
                if curkey != key:
                    print "mismatch key %s/%s"%(curkey, key)
                curkey = ""
                continue
            if len(curkey) == 0:
                continue
            
            # print "Loading: %s"%line

            if curkey == "light_yield":
                self.parse_light_yield(line)
            elif curkey == "refractive":
                if not isfirst.has_key(curkey):
                    # reset
                    del self.self_.refractive_energy[:]
                    del self.self_.refractive_index[:]
                    isfirst[curkey] = False

                self.parse_refractive(line)
            else:
                print "Unknown key '%s'" %curkey
        return True

    def get_key(self, line, tag):
        key = ""
        i1 = line.find(tag)
        if i1 == -1:
            return key
        i1 += len(tag)
        i2 = line.find(tag, i1)
        if i2 == -1:
            return key

        key = line[i1:i2].strip()
        return key

    def parse_light_yield(self, line):
        self.self_.light_yield = float(line)

    def parse_refractive(self, line):
        # print "parse refractive: %s" %line
        ene, idx = line.split("*eV")
        # print " -> (%s, %s)" %(ene, idx)

        self.self_.refractive_energy.append(float(ene.strip()))
        self.self_.refractive_index.append(float(idx.strip()))

class Py_CD_LS_MatProp(CondDB.IConditionObject):

    def __init__(self):
        CondDB.IConditionObject.__init__(self)

        self.cnv = Py_CD_LS_MatProp_Cnv(self)

        # own object
        self.light_yield = 0
        self.refractive_energy = []
        self.refractive_index = []


    def getCnv(self, streamer):

        return self.cnv

##############################################################################
# CD.PMT.Prop
##############################################################################
class Py_CD_PMT_Prop_Cnv(CondDB.ICnvFromPayload):
    
    def __init__(self, self_):
        CondDB.ICnvFromPayload.__init__(self)
        
        self.self_ = self_

        self.tag_begin = "-----"
        self.tag_end = "....."

    def cnv(self, payload):
        # print "got %s"%payload.data()
        decoded = base64.b64decode(payload.data())
        # print "decoded: %s"%decoded

        return self.load(decoded)

    def load(self, decoded):
        curkey = ""

        isfirst = {}

        for line in decoded.splitlines():
            line = line.strip()
            if len(line) == 0:
                continue

            if line.find(self.tag_begin) != -1:
                curkey = self.get_key(line, self.tag_begin)
                # print "Begin curkey: %s"%curkey
                continue
            if line.find(self.tag_end) != -1:
                key = self.get_key(line, self.tag_end)
                if curkey != key:
                    print "mismatch key %s/%s"%(curkey, key)
                curkey = ""
                continue
            if len(curkey) == 0:
                continue
            
            # print "Loading: %s"%line

            if curkey == "pmt":
                if not isfirst.has_key(curkey):
                    # reset
                    del self.self_.pmtid[:]
                    del self.self_.ishama[:]
                    del self.self_.gain[:]
                    del self.self_.fwhm[:]
                    isfirst[curkey] = False

                self.parse_pmt(line)
            else:
                print "Unknown key '%s'" %curkey
        return True

    def get_key(self, line, tag):
        key = ""
        i1 = line.find(tag)
        if i1 == -1:
            return key
        i1 += len(tag)
        i2 = line.find(tag, i1)
        if i2 == -1:
            return key

        key = line[i1:i2].strip()
        return key

    def parse_pmt(self, line):
        # print "parse refractive: %s" %line
        pmtid, ishama, gain, fwhm = line.split()

        self.self_.pmtid.append(int(pmtid.strip()))
        self.self_.ishama.append(int(ishama.strip()))
        self.self_.gain.append(float(gain.strip()))
        self.self_.fwhm.append(float(fwhm.strip()))


class Py_CD_PMT_Prop(CondDB.IConditionObject):

    def __init__(self):
        CondDB.IConditionObject.__init__(self)

        self.cnv = Py_CD_PMT_Prop_Cnv(self)

        # own object
        self.pmtid = []
        self.ishama = []
        self.gain = []
        self.fwhm = []


    def getCnv(self, streamer):

        return self.cnv



if __name__ == "__main__":
    import os.path
    import Sniper
    Sniper.setLogLevel(0)
    task = Sniper.Task("testtask")
    conddbsvc = task.createSvc("CondDBSvc")
    conddbsvc.property("RepoTypes").set({
            # "frontier": "Frontier",
            "local": "LocalFSRepo"
            })
    
    conddbsvc.property("RepoURIs").set({
            # "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier",
            "local": os.path.join("dummy-repo")
            })
    conddbsvc.property("GlobalTag").set(
        "GT_J18_v1"
        )
    conddbsvc.initialize()

    obj_CD_LS_matprop = Py_CD_LS_MatProp()
    print(obj_CD_LS_matprop)
    print(type(obj_CD_LS_matprop))
    conddbsvc.declCondObj("CD.LS.MatProp", obj_CD_LS_matprop)


    obj_CD_PMT_prop = Py_CD_PMT_Prop()
    print(obj_CD_PMT_prop)
    print(type(obj_CD_PMT_prop))
    conddbsvc.declCondObj("CD.PMT.Prop", obj_CD_PMT_prop)

    obj_CD_PMT_prop2 = Py_CD_PMT_Prop()

    conddbsvc.declCondObj("CD.PMT.Prop/pmtprop2", obj_CD_PMT_prop2)

    for cur in range(0,10000,1000):
        conddbsvc.setCurrent(cur)
        print "Cur %d\n -> light yield: %d"%(cur, obj_CD_LS_matprop.light_yield)
        print " -> %s" % ",".join("%f:%f"%(k,v) for k,v in zip(obj_CD_LS_matprop.refractive_energy,
                                                               obj_CD_LS_matprop.refractive_index))
        print " -> Total PMTs: %d "%len(obj_CD_PMT_prop.pmtid)

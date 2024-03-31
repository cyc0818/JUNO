/*
 * Usage:
 *     root -l 'DumpElecTruth.C+("sample_elecsim.root")'
 */
#include <iostream>
#include <map>
#include "TFile.h"
#include "TTree.h"
#include "Event/LpmtElecTruthEvent.h"

void DumpElecTruth(TString fn) {
    TFile* f = TFile::Open(fn);
    if (!f) {
        std::cerr << "Failed to load file " << fn << std::endl;
        return;
    }
    TString treename = "Event/Sim/Truth/LpmtElecTruthEvent";
    TTree* lpmttruth_tree = (TTree*)(f->Get(treename));
    if (!lpmttruth_tree) {
        std::cerr << "Failed to load tree " << treename << std::endl;
        return;
    }

    JM::LpmtElecTruthEvent* truthobj = 0;
    lpmttruth_tree->SetBranchAddress("LpmtElecTruthEvent", &truthobj);
    lpmttruth_tree->GetBranch("LpmtElecTruthEvent")->SetAutoDelete(true);

    for (int evti = 0; evti < lpmttruth_tree->GetEntries(); ++evti) {
        lpmttruth_tree->GetEntry(evti);

        const std::vector<JM::LpmtElecTruth>& truths = truthobj->truths();

        std::map<std::string, int> counter;

        for (int i = 0; i < truths.size(); ++i) {
            counter[ truths[i].pulsetype() ] += 1;
        }
        std::cout << "Event " << evti << ": " << std::endl;
        for (std::map<std::string, int>::iterator it = counter.begin();
             it != counter.end(); ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }

    }

}

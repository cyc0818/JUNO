
/*
 * Generate the library first:
 * $ root -l Relation.C+
 *
 * Process a single root file:
 * $ root -l 'timedispatch.C("/junofs/users/lint/juno-sim/detsim-papers/jobs/02_fast_full/dist-input-20inchonly-photons-map-reduce/jobs/evt-31454-1000.root")'
 *
 * Process a file list:
 *
 * Generate an executable:
 * $ g++ -o timedispatch timedispatch.C $(root-config --cflags --libs)
 *
 * R mode: R or R3
 * T mode: T or CT
 */

#include <iostream>
#include <fstream>
#include "TimeRelation.C"

void timedispatch(TString input="", TString output="hist3d.root",
                  TString rmode="R", TString tmode="T") {
    TChain* relation_tree = new TChain("relation");

    if (input == "") {
        std::cerr << "Please specify the input root or input file list." << std::endl;
        return;
    }
    if (input.EndsWith(".root")) {
        std::cout << "input file: " << input << std::endl;
        relation_tree->AddFile(input.Data());
    } else {
        std::cout << "input filelist: " << "input-file-list.txt" << std::endl;
        std::ifstream inputf(input.Data());
        std::string s;
        while (true) {
            inputf >> s;
            if (!inputf.good()) {
                break;
            }
            relation_tree->AddFile(s.c_str(), 1000*17746);
        }
    }
    
    std::cout << relation_tree->GetEntriesFast() << std::endl;

    TimeRelation* relation = new TimeRelation(relation_tree);
    relation->output = output;

    if (rmode == "R") {
        relation->flag_r = TimeRelation::kR;
    } else if (rmode == "R3") {
        relation->flag_r = TimeRelation::kR3;
    } else {
        std::cerr << "Unknown R mode " << rmode << std::endl;
    }

    if (tmode == "T") {
        relation->flag_t = TimeRelation::kTheta;
        std::cout << " * choose: " <<  relation->flag_t
                  << " TimeRelation::kTheta: " << TimeRelation::kTheta
                  << std::endl;
    } else if (tmode == "CT") {
        relation->flag_t = TimeRelation::kCosTheta;
        std::cout << " * choose: " <<  relation->flag_t
                  << " TimeRelation::kCosTheta: " << TimeRelation::kCosTheta
                  << std::endl;
    } else {
        std::cerr << "Unknown T mode: " << tmode << std::endl;
    }

    std::cout << "R mode: " << relation->flag_r << "; " 
              << "T mode: " << relation->flag_t << std::endl;

    relation->Loop();
}


int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Mismatched number of command line options." << std::endl;
        std::cerr << "usage: ./timedispatch <input|inputlist> <output> [R|R3] [T|CT]" << std::endl;
        return -1;
    }

    TString input = argv[1];
    TString output = argv[2];
    TString rmode = "R3";
    TString tmode = "T";

    if (argc > 3) { rmode = argv[3]; }
    if (argc > 4) { tmode = argv[4]; }
    std::cout << "R mode: " << rmode << "; T mode: " << tmode << std::endl;

    timedispatch(input, output, rmode, tmode);

    return 0;
}

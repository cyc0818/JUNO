#if defined(__CINT__) && ! defined(__ACLIC__)
#error "Plase run in compiled mode."
#endif
/*
 * This script is used in Data Validation and Production.
 *
 * The command allows both command line options and environment variables.
 *
 * We also introduce TEnv to manage some variables.
 *
 * $ g++ -o drawDetSim drawDetSim.C $(root-config --cflags --libs)
 */
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <TString.h>
#include <TEnv.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TFile.h>
#include <TH1.h>

// global variables;
bool debug = 1;

const TString DEFAULT_LABEL = "default";
const char* DEFAULT_LABEL_FORMAT = "label%d";

TEnv* g_env = 0;

TString current_script_dir;

// helper functions
// * string to list
std::vector<TString> cnv_str_list(const TString& str) {
    std::vector<TString> result;

    TString tok;
    Ssiz_t from = 0;
    while (str.Tokenize(tok, from, " ")) {
        tok.Remove(TString::kBoth, ' ');
        if (tok.Length()) {
            result.push_back(tok);
        }
    }

    return result;
}

// Label Manager is used to manage multi input and output
//
//   drawDetSim.labels: LabelA LabelB
//
// So in plots, you could override the options with label name
//
//    LabelA.initXYZ.*.histogram_opts: (200, -20000, 20000)
//
//
struct LabelManager {
    std::vector<TString> labels;

    void set(const TString& lbl_str) {
        labels = cnv_str_list(lbl_str);
    }

    void dump() {

        for (int i = 0; i < labels.size(); ++i) {
            std::cout << "defined label: " << labels[i] << std::endl;
        }
    }
};
LabelManager* g_labelmgr = 0;

struct InputManager {
    struct InputEntry {
        std::vector<TString> files_list;
        // when we already the the chain, we don't need to load again.
        std::map<TString, TChain*> t2chain; 

        void dump() {
        }
    };


    bool load_one_file(const TString& filename) {
        TString label = DEFAULT_LABEL;
        if (g_labelmgr->labels.size()) {
            label = g_labelmgr->labels[0];
        }
        records[label].files_list.push_back(filename);
        return true;
    }

    bool load_file_lists(const std::vector<TString>& file_lists) {
        for (int i = 0; i < file_lists.size(); ++i) {
            load_file_lists_one(file_lists[i]);
        }
        return true;
    }

    void load_file_lists_one(const TString& fn) {
        static int cnt = 0;
        TString label;
        if (cnt == 0) {
            // by default, use label named "default"
            label = DEFAULT_LABEL;
        } else {
            label = Form(DEFAULT_LABEL_FORMAT, cnt);
        }

        // TODO
        // user could override the label by prefix with LABEL:
        if (cnt < g_labelmgr->labels.size()) {
            label = g_labelmgr->labels[cnt];
        }

        ++cnt;

        std::ifstream fp(fn.Data());
        if (!fp.is_open()) {
            std::cerr << "ERROR: input lists does not exists." << std::endl;
        }
        std::string tmp_line;
        while(fp.good()) {
            // 
            std::getline(fp, tmp_line);
            if ( tmp_line.size() ==0 ) {
                continue;
            }

            records[label].files_list.push_back(tmp_line);
        }
    }

    // maintain a table, so user could specify different samples, 
    // such as 1MeV, 2MeV, ...
    // 
    // The key is unified in InputManager, OutputManager and configuration file, 
    std::map<TString, InputEntry> records;
};
InputManager* g_inputmgr = 0;

struct OutputManager {
    OutputManager() {

    }

    void SetOutput(const std::vector<TString>& names) {
        // name should be a list of files. the number of input and output should match

        for (int i = 0; i < names.size(); ++i) {
            SetOutputOne(names[i]);
        }
    }
    
    void SetOutputOne(const TString& name) {

        static int cnt = 0;
        TString label;
        if (cnt == 0) {
            // by default, use label named "default"
            label = DEFAULT_LABEL;
        } else {
            label = Form(DEFAULT_LABEL_FORMAT, cnt);
        }
        // user could override the label by prefix with LABEL:
        if (cnt < g_labelmgr->labels.size()) {
            label = g_labelmgr->labels[cnt];
        }

        ++cnt;


        records[label].label = label;
        records[label].outname = name;
        records[label].outf = TFile::Open(records[label].outname, "RECREATE");
        if (debug) { records[label].dump(); }
    }

    struct OutputEntry {
        TFile* outf;
        TString outname;
        TString label;

        void dump() {
            std::cout << " label: " << label 
                      << " outname: " << outname 
                      << " outf: " << outf
                      << std::endl; 
        }
    };

    std::map<TString, OutputEntry> records;
};
OutputManager* g_ouputmgr = 0;

// Plots Task
//
// in config file:
//
// InitXYZ: task
// InitXYZ.plots: X Y Z
//
// # * is the wildcard
// InitXYZ.*.tree: geninfo 
//
// InitXYZ.X.tree: geninfo
// InitXYZ.X.opt: initX
// initXYZ.X.sel:
//
// InitXYZ.Y.tree: geninfo
// InitXYZ.Y.opt: initX
// initXYZ.Y.sel:
//
//   --> geninfo->Draw(opt, sel)
//
// One task could include multiple plots
//

struct Plot {

    Plot(const TString& task_, const TString plot_) {
        task = task_;
        plot = plot_;

        env = 0;

        histogram_opts_nbin = 100;
        histogram_opts_xmin = 0.;
        histogram_opts_xmax = 100.;
    }

    // helper, get cattr order, if exists, return:
    // 1.0 label.ctask.cplot.cattr
    // 1.1 ctask.cplot.cattr
    //
    // 2.0 label.ctask.*.cattr
    // 2.1 ctask.*.cattr
    // 3.0 label.ctask.cattr
    // 3.1 ctask.cattr
    TString get_attr(const TString& attr) {
        const char* clabel = label.Data();
        const char* ctask = task.Data();
        const char* cplot = plot.Data();
        const char* cattr = attr.Data();

        TString val;

        // 1.0
        val = env->GetValue(Form("%s.%s.%s.%s", clabel, ctask, cplot, cattr), "");
        if (val.Length()) { return val; }
        // 1.1
        val = env->GetValue(Form("%s.%s.%s", ctask, cplot, cattr), "");
        if (val.Length()) { return val; }
        // 2.0
        val = env->GetValue(Form("%s.%s.*.%s", clabel, ctask, cattr), "");
        if (val.Length()) { return val; }
        // 2.1
        val = env->GetValue(Form("%s.*.%s", ctask, cattr), "");
        if (val.Length()) { return val; }
        // 3.0
        val = env->GetValue(Form("%s.%s.%s", clabel, ctask, cattr), "");
        if (val.Length()) { return val; }
        // 3.1
        val = env->GetValue(Form("%s.%s", ctask, cattr), "");
        if (val.Length()) { return val; }

        return val;
    }

    void Parse(TEnv* env_) {
        env = env_;
        const char* ctask = task.Data();
        const char* cplot = plot.Data();
        tree = get_attr("tree");
        if (debug) std::cout << " + tree:" << tree << std::endl;
        opt = get_attr("opt");
        if (debug) std::cout << " + opt:" << opt << std::endl;
        sel = get_attr("sel");
        if (debug) std::cout << " + sel:" << sel << std::endl;

        draw_option = get_attr("draw_option");

        histogram = get_attr("histogram");
        // if user don't specify histogram,
        // the default name is h_cplot
        if (!histogram.Length()) {
            histogram = Form("h_%s_%s", ctask, cplot);
        }

        histogram_opts = get_attr("histogram_opts");
        histogram_opts.Remove(TString::kBoth, ' '); //remove space
        if (histogram_opts.Length()) {
            // detect the '()' exists or not
            if (histogram_opts.Index("(")<0 && histogram_opts.Index(")")<0) {
                // () does not exist, add ()
                histogram_opts = Form("(%s)", histogram_opts.Data());
            }
        }
        TString tmp_opts;
        tmp_opts = get_attr("histogram_opts.nbin");
        if (tmp_opts.Length()) {
            histogram_opts_nbin = tmp_opts;
        }
        tmp_opts = get_attr("histogram_opts.xmin");
        if (tmp_opts.Length()) {
            histogram_opts_xmin = tmp_opts;
        }
        tmp_opts = get_attr("histogram_opts.xmax");
        if (tmp_opts.Length()) {
            histogram_opts_xmax = tmp_opts;
        }
    }

    void PlotIt() {
        // loop all labels
        for (std::map<TString, InputManager::InputEntry>::iterator it = g_inputmgr->records.begin();
                it != g_inputmgr->records.end(); ++it) {
            // parse according to current label
            label = it->first;
            Parse(g_env);
            PlotItOne();
        }
    }

    void PlotItOne() {
        if (debug) { 
            std::cout << "CURRENT LABEL: " << label 
                      << " plot: " << plot
                      << std::endl;
        }
        // check chain exists or not
        TChain* ch = g_inputmgr->records[label].t2chain[tree];
        if (!ch) {
            std::cout << " create new Chain: " << tree << std::endl;
            ch = new TChain(tree);

            for (std::vector<TString>::iterator it = g_inputmgr->records[label].files_list.begin();
                    it != g_inputmgr->records[label].files_list.end(); ++it) {
                std::cout << "add file: " << *it << std::endl;
                ch->Add(*it);
            }

            g_inputmgr->records[label].t2chain[tree] = ch;
        }

        // change to file
        g_ouputmgr->records[label].outf->cd();

        // create a new canvas
        TCanvas* c = new TCanvas();
        // draw it
        // form a varexp with histogram name
        TString varexp;
        if (draw_option == "script") {
            std::cout << "script mode: " << plot << std::endl;
            varexp = opt;
            if (gSystem->AccessPathName(opt)) {
               // does not exist
               varexp = current_script_dir + "/" + opt;
            }
            sel = ""; // in script mode, the selection is ignored.
            draw_option.ReplaceAll("script","");

            Int_t nbin = 100; Double_t xmin = 0; Double_t xmax = 2000;
            if (histogram_opts_nbin.Length()) { nbin = histogram_opts_nbin.Atoi(); }
            if (histogram_opts_xmin.Length()) { xmin = histogram_opts_xmin.Atof(); }
            if (histogram_opts_xmax.Length()) { xmax = histogram_opts_xmax.Atof(); }

            // create histogram, this allows ch->Process to reuse the nbin, xmin and xmax
            // See: tree/treeplayer/src/TBranchProxyDirector.cxx
            TH1F* htemp = new TH1F("htemp2", "htemp", nbin, xmin, xmax);
            htemp->SetDirectory(0);
            htemp->SetTitle(histogram);
            htemp->Draw();

            ch->MakeProxy(Form("analysis_%s", plot.Data()), varexp);
            ch->Process(Form("analysis_%s.h+", plot.Data()), "same");
            TObject* hist = gDirectory->Get(histogram);
            if (hist) {
                hist->Draw("hist");
            }

        } else {
            varexp = Form("%s >> %s%s", opt.Data(), histogram.Data(), histogram_opts.Data());
            ch->Draw(varexp, sel, draw_option);
        }
        TObject* hist = gDirectory->Get(histogram);
        if( hist ) {
            hist->Write();
        }

    }

    TEnv* env;
    TString task;
    TString plot;

    TString label; // this is variable will be changed each time.
    TString tree; // or chain
    TString opt;
    TString sel;
    TString draw_option;
    TString histogram;
    TString histogram_opts;
    TString histogram_opts_nbin;
    TString histogram_opts_xmin;
    TString histogram_opts_xmax;
};
struct Task {

    Task(TString& name_) {
        name = name_;
    }

    void Parse(TEnv* env) {
        const char* cname = name.Data();
        // check self is a task or not
        TString task_type = env->GetValue(name, "");
        if (task_type != "task") {
            std::cerr << name << " is not a task.! " << std::endl;
            return;
        }

        // get plots
        TString plots_name = env->GetValue(Form("%s.plots", cname), "");
        TString tok;
        Ssiz_t from = 0;
        while (plots_name.Tokenize(tok, from, " ")) {
            if(debug) std::cout << "plot: " << tok << std::endl;
            tok.Remove(TString::kBoth, ' ');
            if (!tok.Length()) { continue; }
            plots_name_list.push_back(tok);
            plots[tok] = new Plot(name, tok); //task, plot
            // LT: to allow multiple inputs override the options, we postpone parsing.
            // plots[tok]->Parse(env);
        }
    }

    void PlotIt() {
        for (std::vector<TString>::iterator it = plots_name_list.begin(); it != plots_name_list.end(); ++it) {
            plots[*it]->PlotIt();
        }
    }

    TString name;
    std::vector<TString> plots_name_list;
    std::map<TString, Plot*> plots;
};

struct TasksManager {
    TasksManager(const TString& env_name) {
        env = new TEnv(env_name);
        g_env = env;

    }

    void Parse() {
        ParseLabel();
        ParseTask();
        ParseInput();

        ParseOutput();
    }

    void ParseTask() {
        // retrieve tasks
        TString tasks_name = env->GetValue("drawDetSim.tasks", "");
        TString tok;
        Ssiz_t from = 0;
        while (tasks_name.Tokenize(tok, from, " ")) {
            if(debug) std::cout << "task: " << tok << std::endl;
            tok.Remove(TString::kBoth, ' ');
            if (!tok.Length()) { continue; }
            tasks_name_list.push_back(tok);
            tasks[tok] = new Task(tok);
            tasks[tok]->Parse(env);
        }
    }

    void ParseInput() {
        // one input filename or input list (file contains file list)
        TString one_input = env->GetValue("drawDetSim.input", "");
        TString input_lists = env->GetValue("drawDetSim.input_lists", "");
        one_input.Remove(TString::kBoth, ' ');
        input_lists.Remove(TString::kBoth, ' ');

        if ((!one_input.Length()) && (!input_lists.Length())) {
            // no input
            std::cerr << "WARNING: no input and input_list " << std::endl;

        } else if ((one_input.Length()) && (!input_lists.Length())) {
            // one input, no input list
            g_inputmgr->load_one_file(one_input);
        } else if ((!one_input.Length()) && (input_lists.Length())) {
            // no one input, input list
            // load the input list
            std::vector<TString> input_lists_vec = cnv_str_list(input_lists);
            g_inputmgr->load_file_lists(input_lists_vec);
        } else {
            // all have value, so we will use the value in input_lists
            std::cerr << "WARNING: due to input and input_list all have values, we don't know which to use." << std::endl;
        }

    }

    void ParseOutput() {
        TString one_output = env->GetValue("drawDetSim.output", "");
        if (!one_output.Length()) {
            std::cerr << "WARNING: Don't specify the output name. " << std::endl;
            return;
        }

        std::vector<TString> one_output_vec = cnv_str_list(one_output);

        g_ouputmgr->SetOutput(one_output_vec);
    }

    void ParseLabel() {
        TString lstr = env->GetValue("drawDetSim.labels", "");
        if (!lstr.Length()) {
            std::cerr << "WARNING: Don't specify any label name. " << std::endl;
            return;
        }

        g_labelmgr->set(lstr);
        g_labelmgr->dump();
    }

    void PlotIt() {
        for (std::vector<TString>::iterator it = tasks_name_list.begin(); it != tasks_name_list.end(); ++it) {
            tasks[*it]->PlotIt();
        }
    }

    TEnv* env;
    std::vector<TString> tasks_name_list;
    std::map<TString, Task*> tasks;

};


/*
 * The main entry point
 */
void drawDetSim(TString env_path="env") {
    
    // find the env
    if (gSystem->AccessPathName(env_path)) {
        // does not exsit, try use "env" under directory of this script
        std::cout << "try to locate " << env_path << std::endl;
        // if (gApplication->Argc()) {
        //     for (int i = 0; i < gApplication->Argc(); ++i) {
        //         std::cout << gApplication->Argv(i) << std::endl;
        //     }
        // }
        // std::cout << gSystem->DirName(__FILE__) << std::endl;
        current_script_dir = gSystem->DirName(__FILE__);
        env_path = current_script_dir + "/" + env_path;
    }

    TasksManager* gTaskMgr = new TasksManager(env_path);
    g_labelmgr = new LabelManager();
    g_inputmgr = new InputManager();
    g_ouputmgr = new OutputManager();
    gTaskMgr->Parse();

    gTaskMgr->PlotIt();
}

int main(int argc, char** argv) {
    drawDetSim();
    return 0;
}

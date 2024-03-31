#include <vector>
#include <iostream>
#include "Rest/cURL.h"
#include "Rest/object.h"

#include <TFile.h>
#include <TTree.h>
#include <TBufferFile.h>
#include <TBase64.h>
#include <TROOT.h>
#include <TKey.h>
#include <TCollection.h>
#include <TObjArray.h>

const std::string baseurl = "http://202.122.39.193";

struct Params {
    void help() {
        std::cout << "Options: " << std::endl;
        std::cout << " --account username " << std::endl;
        std::cout << " --password password " << std::endl;
        std::cout << " --rawdata rawdatastr " << std::endl;
        std::cout << " --rootfile rootfile " << std::endl;
        std::cout << " --treename treename " << std::endl;
        std::cout << " --arraylen arraylen " << std::endl;
        std::cout << " --iovsince iovsince " << std::endl;
        std::cout << " --version version " << std::endl;
        std::cout << " --type type " << std::endl;
        std::cout << " --streamer streamer " << std::endl;
        std::cout << " --release release " << std::endl;
    }

    bool parse(int argc, char* argv[]) {

        for (int i = 1; i < argc; ++i) {
            std::string opt = argv[i];
            if (opt=="--uri") {
                if (++i>=argc) { return false; }
                uri = argv[i];
            } else if (opt=="--account") {
                if (++i>=argc) { return false; }
                account = argv[i];
            } else if (opt=="--password") {
                if (++i>=argc) { return false; }
                password = argv[i];
            } else if (opt=="--rootfile") {
                if (++i>=argc) { return false; }
                fnrootfile = argv[i];
            } else if (opt=="--treename") {
                if (++i>=argc) { return false; }
                treename = argv[i];
            } else if (opt=="--arraylen") {
                if (++i>=argc) { return false; }
                arraylen = TString(argv[i]).Atoi();
            } else if (opt=="--iovsince") {
                if (++i>=argc) { return false; }
                iovsince = argv[i];
            } else if (opt=="--version") {
                if (++i>=argc) { return false; }
                version = argv[i];
            } else if (opt=="--release") {
                if (++i>=argc) { return false; }
                release = argv[i];
            } else if (opt=="--type") {
                if (++i>=argc) { return false; }
                type = argv[i];
            } else if (opt=="--streamer") {
                if (++i>=argc) { return false; }
                streamer = argv[i];
            } else if (opt=="--rawdata") {
                if (++i>=argc) { return false; }
                rawdata = argv[i];
            } else if (opt=="--help" or opt=="-h") {
                help();
                return false;
            } else {
                std::cerr << "unknown options: " << opt << std::endl;
                help();
		return false;
	    }
        }

        return true;
    }

    Params() {
        uri = baseurl + "/api/login";
        account = "";
        password = "";

        fnrootfile = "";
        treename = "";

        iovsince = "20200601";
        version = "1";
        release = "J20";

        type = "Test";
        streamer = "root";

        rawdata = "";
    }

    std::string uri;

    std::string account;
    std::string password;

    std::string fnrootfile;
    std::string treename;
    int arraylen; // if arraylen != 0, using TObjArray

    std::string iovsince;
    std::string version;
    std::string release;
    std::string type; // PMT, etc.
    std::string streamer; // root, cvs

    // rawdata string is for test-only.
    std::string rawdata;
};

struct LoginObject: Rest::object {
    std::string msg;
    int code;
    std::string token;

    struct UserObject: Rest::object {
        std::string id;
        std::string loginName;

        UserObject() {
            decl("id", id = -1);
            decl("loginName", loginName = "Unknown");
        }
    } user;

    LoginObject() {
        decl("msg", msg = "Unknown");
        decl("code", code = -1);
        decl("user", user);
        decl("token", token = "Unknown");

    }

    bool show() {
        std::cout << "Login Object: "
                  << " loginName is " << user.loginName
                  << " token is " << token
                  << std::endl;
        return true;
    }
};

struct PayloadObject: Rest::object {
    std::string remarks;
    std::string tmppath;
    std::string hash;
    std::string objectType;
    std::string iovsince;
    std::string path;
    std::string streamerInfo;
    std::string version;
    std::string release;
    std::string data;

    PayloadObject() {
        decl("remarks", remarks);
        decl("tmppath", tmppath);
        decl("hash", hash);
        decl("objectType", objectType);
        decl("iovsince", iovsince);
        decl("path", path);
        decl("streamerInfo", streamerInfo);
        decl("version", version);
        decl("release", release);
        decl("data", data);
    }
};

struct PayloadResponseObject: Rest::object {
    std::string message;
    int code;

    struct DataObject: Rest::object {

        struct PayloadObj: Rest::object {
            std::string id;
            std::string hash;

            PayloadObj() {
                decl("id", id);
                decl("hash", hash);
            }
        } payload;

        std::vector<std::string> message;

        DataObject() {
            decl("payload", payload);
            decl("message", message);
        }
    } data;

    PayloadResponseObject() {
        decl("message", message = "Unknown");
        decl("code", code = -1);
        decl("data", data);

    }

    bool show() {
        std::cout << "Payload Response Object: "
                  << " message is " << message
                  << " code is " << code
                  << std::endl;
        for (auto& i: data.message) {
            std::cout << "message: " << i << std::endl;
        }
        std::cout << " payload.id: " << data.payload.id << std::endl;
        std::cout << " payload.hash: " << data.payload.hash << std::endl;
        return true;
    }
};


bool get_encoded_buff_str(const std::string& rawdata, TString& encoded_buff) {
    encoded_buff = TBase64::Encode(rawdata.c_str());
    return true;
}

bool get_encoded_buff_root(const std::string& fn, const std::string& treename, TString& encoded_buff) {

   TFile* f = TFile::Open(fn.c_str());
    if (!f) {
        std::cerr << "Failed to open root file '" << fn << "'" << std::endl;
        return false;
    }
    
    TTree* tree = (TTree*)(f->Get(treename.c_str()));
    if (!tree) {
        std::cerr << "Failed to find tree named " << treename
                  << " in file " << fn << std::endl;
        return false;
    }

    gROOT->cd();
    TTree* clonetree = (TTree*)tree->CloneTree();

    TBufferFile* tempBuf = new TBufferFile(TBuffer::kWrite);
    clonetree->Streamer(*tempBuf);
    std::cout << "clone tree buffer size: " << tempBuf->BufferSize() << std::endl;

    encoded_buff = TBase64::Encode(tempBuf->Buffer(), tempBuf->BufferSize());

    return true;
}

bool get_encoded_buff_root_array(const std::string& fn, int arraylen, TString& encoded_buff) {

   TFile* f = TFile::Open(fn.c_str());
    if (!f) {
        std::cerr << "Failed to open root file '" << fn << "'" << std::endl;
        return false;
    }

    if (arraylen == 0) {
        std::cerr << "0 object need to be added in file " << fn << std::endl;
        return false;
    }

    TObjArray saved_obj_arrays;

    int cnt = 0;
    TObject *obj = NULL;
    TKey *key = NULL;
    TIter next( f->GetListOfKeys());
    while ((key = (TKey *) next())) {
        obj = f->Get(key->GetName()); // copy object to memory
        // do something with obj
        if (cnt < 10) {
            printf(" found object:%s\n",key->GetName());
        }

        saved_obj_arrays.Add(obj);


        if (arraylen > 0 && ++cnt >= arraylen) {
            break;
        }
    }


    TBufferFile* tempBuf = new TBufferFile(TBuffer::kWrite);

    saved_obj_arrays.Streamer(*tempBuf);
    std::cout << "clone tree buffer size: " << tempBuf->BufferSize() << std::endl;

    encoded_buff = TBase64::Encode(tempBuf->Buffer(), tempBuf->BufferSize());

    return true;
}


int main(int argc, char* argv[]) {

    Params param;
    bool st = param.parse(argc, argv);
    if (!st) {
        return -1;
    }

    // before post the data, we need to check whether the rootfile exists or not.

    TString encoded_buff;
    bool exists = false;

    // 
    // In this example, if user specify the rawdata string from command line,
    // then we will use it first.
    //
    if (!param.rawdata.empty()) {

        exists = get_encoded_buff_str(param.rawdata, encoded_buff);

    } else if (param.arraylen != 0) {
        // put the objects into a TObjArray, then encode it
        if (param.fnrootfile.empty()) {
            std::cerr << "Please specify the input file. Now it is empty." << std::endl;
            return -1;
        }

        exists = get_encoded_buff_root_array(param.fnrootfile, param.arraylen, encoded_buff);

        if (exists) {
            param.streamer = "root";
        }

    } else {

        if (param.fnrootfile.empty()) {
            std::cerr << "Please specify the input file. Now it is empty." << std::endl;
            return -1;
        }
        if (param.treename.empty()) {
            std::cerr << "Please specify the tree name in the file. Now it is empty." << std::endl;
            return -1;
        }

        exists = get_encoded_buff_root(param.fnrootfile, param.treename, encoded_buff);

        if (exists) {
            param.streamer = "root";
        }
    }


    if (!exists) {
        std::cerr << "Failed to find file '" << param.fnrootfile << "'" << std::endl;
        std::cerr << "Or you don't specify the raw data string. " << std::endl;
        return -1;
    }

    std::string data;
    data += "account=";
    data += param.account;
    data += "&";
    data += "password=";
    data += param.password;

    Rest::cURL curlobj;

    LoginObject loginobj;

    if (curlobj.request(param.uri, data)) {

        cJSON* json = cJSON_Parse(curlobj.result().c_str());

        std::cout << curlobj.result() << std::endl;
        loginobj.from_json(json);

        if (loginobj.code != 200) {
            std::cerr << "Failed to get the token. " << std::endl;
            return -1;
        }

        loginobj.show();
    }

    // after we have the token, try to access the service
    std::string payload_uri = baseurl + "/api/payload/create";

    std::vector<std::string> header_list;
    header_list.push_back(std::string("token:")+loginobj.token);
    header_list.push_back(std::string("Content-Type: application/json"));

    // build a JSON object
    cJSON *root = NULL, *item = NULL;
    root = cJSON_CreateObject();
    if (!root) {
        std::cerr << "Failed to create a JSON object." << std::endl;
        return -1;
    }

    PayloadObject po;

    po.remarks = "This is a comment";
    po.tmppath = param.fnrootfile + ":/" + param.treename;
    po.hash = encoded_buff.MD5().Data();
    po.objectType = param.type;
    po.iovsince = param.iovsince;
    // po.path = param.fnrootfile + ":/" + param.treename;
    po.streamerInfo = param.streamer;
    po.version = param.version;
    po.release = param.release; // the software release created the calibration data
    po.data = encoded_buff.Data();

    po.to_json(root);

    char* json_data = cJSON_PrintUnformatted(root);

    data = json_data;
    
    cJSON_Delete(root);
    free(json_data);

    if (curlobj.request(payload_uri, header_list, data)) {
        if (!param.rawdata.empty()) {
            std::cout << curlobj.result() << std::endl;
        }
        // std::cout << curlobj.result() << std::endl;
        cJSON* json = cJSON_Parse(curlobj.result().c_str());
        PayloadResponseObject pro;
        pro.from_json(json);
        pro.show();

        if (pro.code != 200) {
            std::cout << curlobj.result() << std::endl;
            return -1;
        }

    } else {
        std::cerr << "Failed to POST request. " << std::endl;
        return -1;
    }

    return 0;
}

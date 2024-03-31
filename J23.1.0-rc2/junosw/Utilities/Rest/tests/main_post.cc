#include <vector>
#include <iostream>
#include "Rest/cURL.h"
#include "Rest/object.h"

const std::string baseurl = "http://202.122.39.193";

struct Params {
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
            } else {
                std::cerr << "unknown options: " << opt << std::endl;
		return false;
	    }
        }

        return true;
    }

    Params() {
        uri = baseurl + "/api/login";
        account = "";
        password = "";
    }

    std::string uri;

    std::string account;
    std::string password;
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

int main(int argc, char* argv[]) {

    Params param;
    bool st = param.parse(argc, argv);

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
    po.tmppath = "/tmp";
    po.hash = "thisisahash";
    po.objectType = "pmt";
    po.iovsince = "202005";
    po.path = "www";
    po.streamerInfo = "plain";
    po.version = "1";
    po.release = "J20"; // the software release created the calibration data
    po.data = "data";

    po.to_json(root);

    char* json_data = cJSON_PrintUnformatted(root);

    data = json_data;
    
    cJSON_Delete(root);
    free(json_data);

    if (curlobj.request(payload_uri, header_list, data)) {
        std::cout << curlobj.result() << std::endl;
    }

    return 0;
}

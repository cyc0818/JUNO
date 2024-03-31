#include <vector>
#include <iostream>
#include "Rest/cURL.h"
#include "Rest/object.h"

// Testing nested json

struct LoginObject: Rest::object {
    std::string msg;
    int code;
    std::string token;

    struct UserObject: Rest::object {
        std::string id;
        std::string loginName;

        UserObject() {
            decl("id", id = "-1");
            decl("loginName", loginName = "Unknown");
        }
    } user;

    LoginObject() {
        decl("msg", msg = "Unknown");
        decl("code", code = -1);
        decl("user", user);
        decl("token", token = "Unknown");

    }
};


int main() {
    cJSON *root = NULL, *item = NULL;
    root = cJSON_CreateObject();

    LoginObject lo;
    
    lo.to_json(root);

    char* json_data = cJSON_PrintUnformatted(root);
    std::cout << json_data << std::endl;
    cJSON_Delete(root);
    free(json_data);

    lo.show();
}

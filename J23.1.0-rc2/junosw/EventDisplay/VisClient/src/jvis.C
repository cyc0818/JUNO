#include "VisClient/VisClient.h"

#include "TApplication.h"
#include "TGClient.h"
#include "TGFrame.h"

#include <iostream>

void createApp() {
}

int main(int argc, char **argv) {

    std::cout << "Vis creating..." << std::endl;

    TApplication theApp("App", &argc, argv);
    VisClient jvis(gClient->GetRoot(), "SERENA", 1200, 800, "", theApp.Argc(), theApp.Argv());

    try {
        theApp.Run();
    }
    catch (const char *s) {
        std::cout << s << std::endl;
    }
}

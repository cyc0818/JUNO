
#include "G4DAEChroma/G4DAECerenkovStepList.hh"

int main() {

    G4DAECerenkovStepList* csl = new G4DAECerenkovStepList(10000);

    for (int i = 0; i < 10000; ++i) {
        float* cs = csl->GetNextPointer();     

        for (int j = 0; j < 6*4; ++j) {
            cs[j] = 0.0;
        }
    }

    csl->SavePath("test.npy", "NPY");
}


int totalPE_20inch() {
    int total = 0;

    for (int i = 0; i < nPhotons; ++i) {
        if (pmtID[i] < 18000) {
            total += nPE[i];
        }
    }

    return total;
}

void totalPE_20inch_Terminate() {
    htemp->Clone("h_totalPE_20inch");
}

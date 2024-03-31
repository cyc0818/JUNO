#ifndef ISensitiveDetectorElement_h
#define ISensitiveDetectorElement_h

class G4VSensitiveDetector;

class ISensitiveDetectorElement {
public:
    virtual ~ISensitiveDetectorElement() {}

    virtual G4VSensitiveDetector* getSD() = 0;
};

#endif

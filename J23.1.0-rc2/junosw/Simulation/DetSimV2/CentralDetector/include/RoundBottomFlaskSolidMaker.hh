#ifndef RoundBottomFlaskSolidMaker_hh
#define RoundBottomFlaskSolidMaker_hh

/* This class is used to create a Ball with Chimney.
 * It is a Tube plus the Ball
 *
 * For the LS in the container, a sphere+tube is a good solid.
 * Because we can modify the top surface.
 */
#include <string>
class G4VSolid;

enum HeightIndicator {
    TUBEH,
    TOPTOEQUATORH
};

class RoundBottomFlaskSolidMaker {
public:
    G4VSolid* getSolid();

    /* the ball is the base */
    RoundBottomFlaskSolidMaker(const std::string& name, 
                               double BallR, 
                               double TubeR, 
                               double TubeH, // or TopToEquator
                               const HeightIndicator& indicator=TUBEH);
    RoundBottomFlaskSolidMaker(const std::string& name, 
                               double BallR, 
                               double TubeR, 
                               double TubeH, 
                               double offset);

private:
    void calculate();

private:
    std::string m_solid_name;

    double m_BallR;
    double m_TubeR;
    double m_TubeH;

    bool m_use_offset; // if user set the offset, it is true, 
    double m_offset;

    double m_tmp_h; // from equator to bottom of tube
};

#endif

#ifndef PMTinPrototypePos_hh
#define PMTinPrototypePos_hh
#include "DetSimAlg/IDetElementPos.h"

#include <vector>

namespace JUNO {
namespace Prototype {

/* 20inch MCP PMT */
class MCPPMT20inchPos : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    MCPPMT20inchPos(double ball_r_MCP20inch);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};
/*20inch Ham PMT */
class HamPMT20inchPos : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    HamPMT20inchPos(double ball_r_Ham20inch);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};


/* 8inch MCP PMT */
class MCPPMT8inchPos : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    MCPPMT8inchPos(double r, double pmt_r, double d);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};

/* 8inch Ham PMT */
class HamPMT8inchPos : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    HamPMT8inchPos(double r, double pmt_r, double d);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};

/* 9inch HZC PMT */
class HZCPMT9inchPos : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    HZCPMT9inchPos(double r, double pmt_r, double d);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};

/*bottom 8inch MCP PMT*/
class MCPPMT8inchPos_BTM : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    MCPPMT8inchPos_BTM(double r, double pmt_r);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};

/*bottom 8inch Ham PMT*/
class HamPMT8inchPos_BTM : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    HamPMT8inchPos_BTM(double r, double pmt_r);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};

/*bottom 9inch HZC PMT*/
class HZCPMT9inchPos_BTM : public IDetElementPos{
public:
    G4bool hasNext();
    G4Transform3D next();

    HZCPMT9inchPos_BTM(double r, double pmt_r);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
};

}
}

#endif

#pragma once

/*
 * Description:
 *   The pmtid used in this header file is the copy number. 
 *   User should avoid to use the hardcode number. Use the enum values instead. 
 *
 * Author:
 *   Simon C. Blyth <blyth@ihep.ac.cn>
 *   Tao Lin <lintao@ihep.ac.cn>
 */

enum PMTID_OFFSET_DETSIM {
  kOFFSET_CD_LPMT=0,     kOFFSET_CD_LPMT_END=20000, // CD-LPMT
  kOFFSET_WP_PMT=30000,  kOFFSET_WP_PMT_END=32400,  // WP-LPMT
  kOFFSET_CD_SPMT=300000,kOFFSET_CD_SPMT_END=325600 // CD-SPMT
};

struct PMT
{
    static bool IsCD(int pmtid); 
    static bool IsWP(int pmtid); 
    static bool Is20inch(int pmtid); 
    static bool Is3inch(int pmtid); 
};

inline bool PMT::IsCD(int pmtid)
{
    return (kOFFSET_CD_LPMT<=pmtid and pmtid<kOFFSET_CD_LPMT_END) 
        or (kOFFSET_CD_SPMT<=pmtid and pmtid<kOFFSET_CD_SPMT_END);
}
inline bool PMT::IsWP(int pmtid)
{
    return (kOFFSET_WP_PMT<=pmtid and pmtid<kOFFSET_WP_PMT_END);
}
inline bool PMT::Is20inch(int pmtid)
{
    return (kOFFSET_CD_LPMT<=pmtid and pmtid<kOFFSET_CD_LPMT_END)
        or (kOFFSET_WP_PMT<=pmtid and pmtid<kOFFSET_WP_PMT_END);
}
inline bool PMT::Is3inch(int pmtid)
{
    return (kOFFSET_CD_SPMT<=pmtid and pmtid<kOFFSET_CD_SPMT_END);
}


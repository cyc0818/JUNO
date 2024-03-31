#include <boost/filesystem.hpp>
#include <Geometry/PMTParamSvc.h>
#include <fstream>
#include <sstream>
#include <numeric>
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"
#include "TMath.h"

DECLARE_SERVICE(PMTParamSvc);
DECLARE_CAN_BE_SHARED(PMTParamSvc);

PMTParamSvc::PMTParamSvc(const std::string& name)
    : 
    SvcBase(name),
    m_db_type("File"), 
    m_pmt_categories(data.m_pmt_categories),
    m_file_cd_lpmt_pos_path(), 
    m_file_cd_lpmt_type_path(), 
    m_file_cd_spmt_pos_path(), 
    m_file_cd_spmt_type_path(), 
    m_file_wp_lpmt_pos_path(), 
    m_file_wp_lpmt_type_path()
{
  declProp("DBType", m_db_type);
  declProp("Path_CD_LPMT_Pos", m_file_cd_lpmt_pos_path);
  declProp("Path_CD_LPMT_Type", m_file_cd_lpmt_type_path);
  declProp("Path_CD_SPMT_Pos", m_file_cd_spmt_pos_path);
  declProp("Path_CD_SPMT_Type", m_file_cd_spmt_type_path);
  declProp("Path_WP_LPMT_Pos", m_file_wp_lpmt_pos_path);
  declProp("Path_WP_LPMT_Type", m_file_wp_lpmt_type_path);

  declProp("Ranges_PMT_Enabled", m_pmt_enabled_ranges);
  declProp("Ranges_PMT_Constructed", m_pmt_constructed_ranges);

  fInstance = this ;  
}

PMTParamSvc::~PMTParamSvc()
{

}

PMTParamSvc* PMTParamSvc::fInstance = NULL ; 
PMTParamSvc* PMTParamSvc::Instance() 
{
    return fInstance ; 
} 




/**
PMTParamSvc::getPMTCategory
----------------------------

WP PMTs (pmtID 30,000+) are not listed, so return category kPMT_Unknown -1 

**/
int PMTParamSvc::getPMTCategory(int pmtID) const 
{
    //return m_pmt_categories.count(pmtID) == 1 ? m_pmt_categories.at(pmtID) : kPMT_Unknown  ; 
    return isWP(pmtID) ? kPMT_Unknown : m_pmt_categories.at(pmtID) ; 
}

bool PMTParamSvc::initialize() {
  bool st;

  // init the ranges
  st = init_ranges();
  if (!st) { return st; }

  // init global info
  st = init_default();
  if (!st) { return st; }

  // FILE MODE
  if (m_db_type == "File") {
    st = init_file();
  } else if (m_db_type == "ROOT") {

  } else if (m_db_type == "MySQL") {

  } else {
  }

  return st;
}

bool PMTParamSvc::finalize() {
  return true;
}

bool PMTParamSvc::isCD(int pmtid) const  {
    return PMT::IsCD(pmtid);
}

bool PMTParamSvc::isWP(int pmtid) const {
    return PMT::IsWP(pmtid);
}

bool PMTParamSvc::is20inch(int pmtid) const {
    return PMT::Is20inch(pmtid);
}

bool PMTParamSvc::is3inch(int pmtid) const {
    return PMT::Is3inch(pmtid);
}

bool PMTParamSvc::isNormalNNVT(int pmtid) const {
  if (m_pmt_categories.count(pmtid)==0)
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return false;
  }
  else
  {
    return is20inch(pmtid) and m_pmt_categories.at(pmtid) == kPMT_NNVT;
  }
}

bool PMTParamSvc::isNNVT(int pmtid) const {
  if (m_pmt_categories.count(pmtid)==0)
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return false;
  }
  else
  {
    return is20inch(pmtid) and (m_pmt_categories.at(pmtid) == kPMT_NNVT or m_pmt_categories.at(pmtid) == kPMT_NNVT_HighQE);
  }
}

bool PMTParamSvc::isHamamatsu(int pmtid) const  {
  if (m_pmt_categories.count(pmtid)==0)
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return false;
  }
  else
  {
    return is20inch(pmtid) and m_pmt_categories.at(pmtid) == kPMT_Hamamatsu;
  }
}

bool PMTParamSvc::isHighQENNVT(int pmtid) const {
  if (m_pmt_categories.count(pmtid)==0)
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return false;
  }
  else
  {
    return !isHamamatsu(pmtid) and m_pmt_categories.at(pmtid) == kPMT_NNVT_HighQE;
  }
}

bool PMTParamSvc::isHZC(int pmtid) const {
  return is3inch(pmtid) 
         /*and m_pmt_categories[pmtid] == kPMT_HZC*/ // for now, all 3inch PMTs are HZC
         ;
}

bool PMTParamSvc::helper_check_ranges(const std::vector<int>& pmt_ranges) const {
    if (pmt_ranges.size()) {
        if (pmt_ranges.size()%2 == 1) {
            std::cerr << "PMT ranges mismatch: "
                      << " size is "
                      << pmt_ranges.size()
                      << std::endl;
            return false;
        }

        int range_len = pmt_ranges.size() / 2;
        for (int range_i = 0; range_i < range_len; ++range_i) {
            auto& range_from = pmt_ranges[2*range_i];
            auto& range_to = pmt_ranges[2*range_i+1];
            if (range_from > range_to) {
                std::cerr << "wrong order in the PMT ranges: "
                          << " from: " << range_from
                          << " to: " << range_to
                          << std::endl;
                return false;
            }
        }
    }


    return true;
}

bool PMTParamSvc::helper_is_in_range(const std::vector<int>& pmt_ranges, int pmtid) const {
    bool is_in_range = true;
    //
    // if ranges are set, then we need to check whether the pmtid in the range or not
    //
    // -- Tao Lin <lintao AT ihep.ac.cn>, 17 Feb 2022
    //
    if (pmt_ranges.size()) {
        is_in_range = false;

        int range_len = pmt_ranges.size() / 2;
        for (int range_i = 0; range_i < range_len; ++range_i) {
            auto& range_from = pmt_ranges[2*range_i];
            auto& range_to = pmt_ranges[2*range_i+1];

            // OK, in range, stop
            if (range_from <= pmtid && pmtid < range_to) {
                std::cout << " pmtid " << pmtid
                          << " is in range [" << range_from
                          << ", " << range_to << ")"
                          << std::endl;
                is_in_range = true;
                break;
            }
        }
    }

    return is_in_range;

}

bool PMTParamSvc::isEnabled(int pmtid) const {
    return helper_is_in_range(m_pmt_enabled_ranges, pmtid);
}

bool PMTParamSvc::isConstructed(int pmtid) const {
    return helper_is_in_range(m_pmt_constructed_ranges, pmtid);
}


// New Interfaces --qianz 2021.06.05
float PMTParamSvc::getPMTX(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_x.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTY(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_y.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTZ(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_z.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTTheta(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_theta.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTPhi(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_phi.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTPsi(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_psi.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}


float PMTParamSvc::getPMTX_ideal(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_x_ideal.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTY_ideal(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_y_ideal.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTZ_ideal(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_z_ideal.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTTheta_ideal(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_theta_ideal.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTPhi_ideal(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_phi_ideal.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTPsi_ideal(int pmtid) const
{
  if (is20inch(pmtid) || is3inch(pmtid) || isWP(pmtid))
  {
    return m_pmt_psi_ideal.at(pmtid);
  }
  else
  {
    std::cout << "Wrong PMT id input: " << pmtid << std::endl;
    return 0;
  }
}

float PMTParamSvc::getPMTmeanR_CD_LPMT() const
{
  return m_pmt_meanR_CD_lpmt;
}

float PMTParamSvc::getPMTmeanR_CD_SPMT() const
{
  return m_pmt_meanR_CD_spmt;
}

float PMTParamSvc::getPMTmeanR_CD_LPMT_ideal() const
{
  return m_pmt_meanR_CD_lpmt_ideal;
}

float PMTParamSvc::getPMTmeanR_CD_SPMT_ideal() const
{
  return m_pmt_meanR_CD_spmt_ideal;
}

int PMTParamSvc::get_NTotal_CD_LPMT() const
{
  return m_nums_cd_lpmt;
}

int PMTParamSvc::get_NTotal_CD_SPMT() const
{
  return m_nums_cd_spmt;
}

int PMTParamSvc::get_NTotal_WP_LPMT() const
{
  return m_nums_wp_lpmt;
}

// END User Interface

// Below is private methods

bool PMTParamSvc::init_ranges() {

    if (not helper_check_ranges(m_pmt_enabled_ranges)) {
        return false;
    }

    if (not helper_check_ranges(m_pmt_constructed_ranges)) {
        return false;
    }

    return true;
}


bool PMTParamSvc::init_default() {
  
  // CD/LPMT

  // WP/PMT

  // CD/SPMT

  return true;
}

std::string PMTParamSvc::getFilePath(const std::string &f)
{
  std::string f_path;
  if (getenv("JUNOTOP"))
  {
    std::string s = getenv("JUNOTOP");
    s += f;
    if (boost::filesystem::exists(s))
    {
      f_path = s;
    }
  }
  if (getenv("WORKTOP"))
  {
    std::string s = getenv("WORKTOP");
    s += f;
    if (boost::filesystem::exists(s))
    {
      f_path = s;
    }
  }

  return f_path;
}


bool PMTParamSvc::init_file()
{
  std::string dir_path = "/data/Detector/Geometry/";

  if (m_file_cd_lpmt_pos_path.empty())
  {
    m_file_cd_lpmt_pos_path = getFilePath(dir_path + "PMTPos_CD_LPMT.csv");
  }

  if (m_file_cd_lpmt_type_path.empty())
  {
    m_file_cd_lpmt_type_path = getFilePath(dir_path + "PMTType_CD_LPMT.csv");
  }

  if (m_file_cd_spmt_pos_path.empty())
  {
    m_file_cd_spmt_pos_path = getFilePath(dir_path + "PMTPos_CD_SPMT.csv");
  }

  if (m_file_cd_spmt_type_path.empty())
  {
    m_file_cd_spmt_type_path = getFilePath(dir_path + "PMTType_CD_SPMT.csv");
  }

  if (m_file_wp_lpmt_pos_path.empty())
  {
    m_file_wp_lpmt_pos_path = getFilePath(dir_path + "PMTPos_WP_LPMT.csv");
  }
  
  if (m_file_cd_lpmt_pos_shift_path.empty())
  {
    m_file_cd_lpmt_pos_shift_path = getFilePath(dir_path + "PMTPos_Shift_CD_LPMT.csv");
  }

  if (m_file_cd_spmt_pos_shift_path.empty())
  {
    m_file_cd_spmt_pos_shift_path = getFilePath(dir_path + "PMTPos_Shift_CD_SPMT.csv");
  }

  if (m_file_wp_lpmt_pos_shift_path.empty())
  {
    m_file_wp_lpmt_pos_shift_path = getFilePath(dir_path + "PMTPos_Shift_WP_LPMT.csv");
  }

  bool st_cd_lpmt_pos, st_cd_lpmt_type, st_cd_spmt_pos, st_cd_spmt_type, st_wp_lpmt_pos;//, st_wp_lpmt_type;
  bool st_cd_lpmt_shift_pos, st_cd_spmt_shift_pos, st_wp_lpmt_shift_pos;

  // nums
  m_nums_cd_lpmt = 0;
  m_nums_cd_spmt = 0;
  m_nums_wp_lpmt = 0;

  // position
  st_cd_lpmt_pos = init_file_PMTPos(m_file_cd_lpmt_pos_path);
  st_cd_spmt_pos = init_file_PMTPos(m_file_cd_spmt_pos_path);
  st_wp_lpmt_pos = init_file_PMTPos(m_file_wp_lpmt_pos_path);

  // save the unshift values
  m_pmt_x_ideal = m_pmt_x;
  m_pmt_y_ideal = m_pmt_y;
  m_pmt_z_ideal = m_pmt_z;
  m_pmt_theta_ideal = m_pmt_theta;
  m_pmt_phi_ideal = m_pmt_phi;
  m_pmt_psi_ideal = m_pmt_psi;

  // position shift
  st_cd_lpmt_shift_pos = init_file_PMTPos_Shift(m_file_cd_lpmt_pos_shift_path);
  st_cd_spmt_shift_pos = init_file_PMTPos_Shift(m_file_cd_spmt_pos_shift_path);
  st_wp_lpmt_shift_pos = init_file_PMTPos_Shift(m_file_wp_lpmt_pos_shift_path);

  // pmt_r
  init_PMT_R();

  // type
  st_cd_lpmt_type = init_file_PMTType(m_file_cd_lpmt_type_path);
  st_cd_spmt_type = init_file_PMTType(m_file_cd_spmt_type_path);
  LogInfo<<"CD LPMT Num: "<<m_nums_cd_lpmt<<"\t"<<"CD SPMT Num: "<<m_nums_cd_spmt<<"\t"<<"WP LPMT Num: "<<m_nums_wp_lpmt<<std::endl;


  if(st_cd_lpmt_shift_pos&&st_cd_spmt_shift_pos&&st_wp_lpmt_shift_pos)
  {
    LogInfo<<"Pos of CD_LPMT, CD_SPMT and WP_LPMT are already shifted."<<std::endl;
  }else
  {
    if(st_cd_lpmt_shift_pos)
      LogInfo<<"Pos of CD_LPMT is shifted."<<std::endl;
    if(st_cd_spmt_shift_pos)
      LogInfo<<"Pos of CD_SPMT is shifted."<<std::endl;
    if(st_wp_lpmt_shift_pos)
      LogInfo<<"Pos of WP_LPMT is shifted."<<std::endl;
  }


  if(st_cd_lpmt_pos && st_cd_spmt_pos && st_wp_lpmt_pos && st_cd_lpmt_type && st_cd_spmt_type)
  {
    return true;
  }else
  {
    LogWarn<<"st_cd_lpmt_pos: "<<st_cd_lpmt_pos<<"\t"<<"st_cd_spmt_pos: "<<st_cd_spmt_pos<<"\t"<<"st_wp_lpmt_pos: "<<st_wp_lpmt_pos<<"\t"<<"st_cd_lpmt_type: "<<st_cd_lpmt_type<<"\t"<<"st_cd_spmt_type: "<<st_cd_spmt_type<<std::endl;
    return true;
  }
  
}

bool PMTParamSvc::init_file_PMTPos(const std::string &f)
{
  bool st = true;

  if (boost::filesystem::exists(f))
  {
    LogInfo << "Loading PMT pos from file: " << f << std::endl;
  }
  else
  {
    LogError << "Can't find PMT pos file '" << f << "'." << std::endl;
    st = false;
    return st;
  }

  std::ifstream input(f.c_str());
  std::string tmp_line;
  int pmtid;       // col 1
  float pmt_x;     // col 2
  float pmt_y;     // col 3
  float pmt_z;     // col 3
  float pmt_theta; // col 4
  float pmt_phi;   // col 5
  float pmt_psi;   // col 6 (may not have it)

  while (input.good())
  {
    std::getline(input, tmp_line);
    if (input.fail())
    {
      break;
    }

    std::stringstream ss;
    ss << tmp_line;

    ss >> pmtid;
    if (ss.fail())
    {
      continue;
    }

    if(pmtid<kOFFSET_CD_LPMT_END&&pmtid>=kOFFSET_CD_LPMT) m_nums_cd_lpmt++;
    if(pmtid<kOFFSET_WP_PMT_END&&pmtid>=kOFFSET_WP_PMT) m_nums_wp_lpmt++;
    if(pmtid<kOFFSET_CD_SPMT_END&&pmtid>=kOFFSET_CD_SPMT) m_nums_cd_spmt++;

    bool if_fail_read = false;
    // check if pmt is already set
    if (m_pmt_x.find(pmtid) != m_pmt_x.end())
    {
      LogWarn << " pmtid " << pmtid << " is already set. " << std::endl;
    }
    // reset the default value if failed to parse
    ss >> pmt_x;
    if (ss.fail())
    {
      if_fail_read = true;
      pmt_x = 0;
    }
    m_pmt_x[pmtid] = pmt_x;
    ss >> pmt_y;
    if (ss.fail())
    {
      if_fail_read = true;
      pmt_y = 0;
    }
    m_pmt_y[pmtid] = pmt_y;
    ss >> pmt_z;
    if (ss.fail())
    {
      if_fail_read = true;
      pmt_z = 0;
    }
    m_pmt_z[pmtid] = pmt_z;

    ss >> pmt_theta;
    if (ss.fail())
    {
      pmt_theta = 0;
    }
    m_pmt_theta[pmtid] = pmt_theta;
    ss >> pmt_phi;
    if (ss.fail())
    {
      pmt_phi = 0;
    }
    m_pmt_phi[pmtid] = pmt_phi;
    ss >> pmt_psi;
    if (ss.fail())
    {
      pmt_psi = 0.0;
    }
    m_pmt_psi[pmtid] = pmt_psi;

    if(!if_fail_read)
    {
      float pmt_r = TMath::Power(pmt_x*pmt_x+ pmt_y*pmt_y + pmt_z*pmt_z ,1/2.);
      if(pmtid<kOFFSET_CD_LPMT_END&&pmtid>=kOFFSET_CD_LPMT) m_pmt_r_lpmt_ideal.push_back(pmt_r);
      if(pmtid<kOFFSET_CD_SPMT_END&&pmtid>=kOFFSET_CD_SPMT) m_pmt_r_spmt_ideal.push_back(pmt_r);
    }
  }

  return st;
}

bool PMTParamSvc::init_file_PMTType(const std::string &f)
{
  bool st = true;

  if (boost::filesystem::exists(f))
  {
    LogInfo << "Loading PMT category from file: " << f << std::endl;
  }
  else
  {
    LogError << "Can't find PMT category file '" << f << "'." << std::endl;
    st = false;
    return st;
  }

  std::ifstream input(f.c_str());
  std::string tmp_line;
  int pmtid;            // col 1
  std::string category; // col 2

  while (input.good())
  {
    std::getline(input, tmp_line);
    if (input.fail())
    {
      break;
    }

    std::stringstream ss;
    ss << tmp_line;

    ss >> pmtid;
    if (ss.fail())
    {
      continue;
    }

    // check if pmt is already set
    if (m_pmt_categories.find(pmtid) != m_pmt_categories.end())
    {
      LogWarn << " pmtid " << pmtid << " is already set. " << std::endl;
    }

    // reset the default value if failed to parse
    ss >> category;
    if (ss.fail())
    {
      if (pmtid <= kOFFSET_CD_LPMT_END)
        category = "Hamamatsu";
      else if (PMT::Is3inch(pmtid))
        category = "HZC";
      else
        category = "Unknown";
    }
    // save into m_pmt_categories
    if (category == "Hamamatsu")
    {
      m_pmt_categories[pmtid] = kPMT_Hamamatsu;
    }
    else if (category == "NNVT")
    {
      m_pmt_categories[pmtid] = kPMT_NNVT;
    }
    else if (category == "HighQENNVT")
    {
      m_pmt_categories[pmtid] = kPMT_NNVT_HighQE;
    }
    else if (category == "HZC")
    {
      m_pmt_categories[pmtid] = kPMT_HZC;
    }
    else
    {
      LogError << "Unknown PMT category name: " << category << std::endl;
    }
  }

  return st;
}


bool PMTParamSvc::init_file_PMTPos_Shift(const std::string &f)
{
  bool st = true;

  if (boost::filesystem::exists(f))
  {
    LogInfo << "Loading PMT pos shift from file: " << f << std::endl;
  }
  else
  {
    st = false;
    return st;
  }

  std::ifstream input(f.c_str());
  std::string tmp_line;
  int pmtid;             // col 1
  float pmt_x_shift;     // col 2
  float pmt_y_shift;     // col 3
  float pmt_z_shift;     // col 3
  float pmt_theta_shift; // col 4
  float pmt_phi_shift;   // col 5
  float pmt_psi_shift;   // col 6 (may not have it)

  int num_pmt_shift = 0;
  
  while (input.good())
  {
    std::getline(input, tmp_line);
    if (input.fail())
    {
      break;
    }

    std::stringstream ss;
    ss << tmp_line;

    ss >> pmtid;
    if (ss.fail())
    {
      continue;
    }

    bool if_fail_read = false;
    // check if pmt is already set
    if (m_pmt_x.find(pmtid) == m_pmt_x.end())
    {
      LogWarn << " pmtid " << pmtid << " hasn't been set when the pos is shifting " << std::endl;
    }
    // reset the default value if failed to parse
    ss >> pmt_x_shift;
    if (ss.fail())
    {
      if_fail_read = true;
      pmt_x_shift = 0;
    }
    m_pmt_x[pmtid] += pmt_x_shift;
    
    ss >> pmt_y_shift;
    if (ss.fail())
    {
      if_fail_read = true;
      pmt_y_shift = 0;
    }
    m_pmt_y[pmtid] += pmt_y_shift;

    ss >> pmt_z_shift;
    if (ss.fail())
    {
      if_fail_read = true;
      pmt_z_shift = 0;
    }
    m_pmt_z[pmtid] += pmt_z_shift;

    ss >> pmt_theta_shift;
    if (ss.fail())
    {
      pmt_theta_shift = 0;
    }
    m_pmt_theta[pmtid] += pmt_theta_shift;

    ss >> pmt_phi_shift;
    if (ss.fail())
    {
      pmt_phi_shift = 0;
    }
    m_pmt_phi[pmtid] += pmt_phi_shift;

    ss >> pmt_psi_shift;
    if (ss.fail())
    {
      pmt_psi_shift = 0.0;
    }
    m_pmt_psi[pmtid] += pmt_psi_shift;

    if(if_fail_read)
    {
      LogWarn << "Something wrong when loading the pos shift files." <<std::endl;
    }else
    {
      num_pmt_shift++;
    }
  }
  LogInfo<<num_pmt_shift<< " PMTs Position are shifted"<<std::endl;
  return st;
}

bool PMTParamSvc::init_PMT_R()
{
  std::map<int, float>::iterator iter;
  for(iter = m_pmt_x.begin(); iter != m_pmt_x.end(); iter++)
  { 
    int pmtid = iter->first;
    float pmt_x = m_pmt_x[pmtid];
    float pmt_y = m_pmt_y[pmtid];
    float pmt_z = m_pmt_z[pmtid];
    float pmt_r = TMath::Power(pmt_x*pmt_x+ pmt_y*pmt_y + pmt_z*pmt_z ,1/2.);
    if(pmtid<kOFFSET_CD_LPMT_END&&pmtid>=kOFFSET_CD_LPMT) m_pmt_r_lpmt.push_back(pmt_r);
    if(pmtid<kOFFSET_CD_SPMT_END&&pmtid>=kOFFSET_CD_SPMT) m_pmt_r_spmt.push_back(pmt_r);
  }
  
  m_pmt_meanR_CD_lpmt = std::accumulate(std::begin(m_pmt_r_lpmt), std::end(m_pmt_r_lpmt), 0.0)/float(m_pmt_r_lpmt.size());
  m_pmt_meanR_CD_spmt = std::accumulate(std::begin(m_pmt_r_spmt), std::end(m_pmt_r_spmt), 0.0)/float(m_pmt_r_spmt.size());

  m_pmt_meanR_CD_lpmt_ideal = std::accumulate(std::begin(m_pmt_r_lpmt_ideal), std::end(m_pmt_r_lpmt_ideal), 0.0)/float(m_pmt_r_lpmt_ideal.size());
  m_pmt_meanR_CD_spmt_ideal = std::accumulate(std::begin(m_pmt_r_spmt_ideal), std::end(m_pmt_r_spmt_ideal), 0.0)/float(m_pmt_r_spmt_ideal.size());

  LogInfo<<"CD LPMT Mean R: "<<m_pmt_meanR_CD_lpmt<<" mm"<<"\t"<<"CD SPMT Mean R: "<<m_pmt_meanR_CD_spmt<<" mm"<<std::endl;
  LogInfo<<"CD LPMT Mean R: "<<m_pmt_meanR_CD_lpmt_ideal<<" mm (unshifted)"<<"\t"<<"CD SPMT Mean R: "<<m_pmt_meanR_CD_spmt_ideal<<" mm  (unshifted)"<<std::endl;

  return true;
}

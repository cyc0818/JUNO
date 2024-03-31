#include "tests/Testing_CD_LS_MatProp.h"
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/trim.hpp>

#include "TBase64.h"

namespace CondDB {
    namespace Testing {
        ICnvFromPayload* CD_LS_MatProp::getCnv(const std::string& streamer) {
            if (streamer == "ROOT") {
                cnvFromROOT = new CnvFromPayloadROOT();
                cnvFromROOT->init(this);
                return cnvFromROOT;
            } else if (streamer == "DB") {
                cnvFromDB = new CnvFromPayloadDB();
                cnvFromDB->init(this);
                return cnvFromDB;
            } else { 
                std::cerr << "WARNING: unknown streamer "
                          << streamer
                          << std::endl;
            }
            return NULL;
        }


        CD_LS_MatProp::CD_LS_MatProp() {
            light_yield = 0;
        }

        void CD_LS_MatProp::dump() {
            std::cout << "Light yield: " << light_yield << std::endl;
            std::cout << "Refractive len: " << refractive_energy.size() << std::endl;

        }

        bool CnvFromPayloadROOT::cnv(const Payload& payload) {
            return true;
        }

        bool CnvFromPayloadROOT::init(CD_LS_MatProp* self_) {
            self = self_;
            return true;
        }

        bool CnvFromPayloadDB::cnv(const Payload& payload) {
            std::cout << "Invoke CnvFromPayloadDB." << std::endl;
            const std::string& data = payload.data();
            TString decoded_data = TBase64::Decode(data.c_str());
            //std::cout << "data is" << decoded_data << std::endl;
            self->Data = decoded_data;
            load(decoded_data.Data());
            return true;
        }

        bool CnvFromPayloadDB::init(CD_LS_MatProp* self_) {
            self = self_;
            return true;
        }
        
        void CnvFromPayloadDB::load(const char* buf) {
            std::stringstream ss(buf);
            std::string line;

            std::string curkey;

            bool isfirst = false;

            while (getline(ss, line)) {
                // find key name first
                // parse the value
                // end

                const static std::string tag_begin = "-----";
                const static std::string tag_end = ".....";

                if (line.substr(0, tag_begin.size()) == tag_begin) {
                    std::cout << line << std::endl;
                    // now, try to find the second tag
                    curkey = get_key(line, tag_begin);
                    isfirst = true;
                    continue;
                } else if (line.substr(0, tag_end.size()) == tag_end) {
                    std::cout << line << std::endl;
                    std::string key = get_key(line, tag_end);
                    if (curkey != key) {
                        std::cerr << "mismatch key for begin/end: "
                                  << curkey
                                  << "/"
                                  << key;
                    }
                    curkey = "";
                    continue;
                }

                if (curkey.empty()) {
                    continue;
                }
                std::cout << "cur key: " << curkey
                          << " -> " << line << std::endl;
                // parse according to the key
                if (curkey == "light_yield") {
                    parse_light_yield(line);
                } else if (curkey == "refractive") {
                    if (isfirst) {
                        // reset the refractive array
                        self->refractive_energy.clear();
                        self->refractive_index.clear();
                    }

                    parse_refractive(line);
                } else {
                    std::cerr << "Unsupported key " << curkey << std::endl;
                }

                isfirst = false;
            }
        }

        void CnvFromPayloadDB::parse_light_yield(const std::string& line) {
            std::stringstream sss(line);
            double ly;
            sss >> ly;
            // std::cout << " sss.good: " << sss.good()
            //           << " sss.fail: " << sss.fail() << std::endl;
            // std::cout << ly << std::endl;
            if (!sss.fail()) {
                self->light_yield = ly;
            }
        }

        void CnvFromPayloadDB::parse_refractive(const std::string& line) {
            double ene;
            double refidx;
            std::string unit;
            std::stringstream sss(line);
            sss >> ene;
            if (sss.fail()) {
                std::cerr << "Skip due to ene: " << line << std::endl;
                return;
            }
            char c = sss.get();
            while (isspace(c)) {
                c = sss.get();
            }
            sss.unget();
            if (!isdigit(c)) {
                sss >> unit;
            }

            c = sss.get();
            while (isspace(c)) {
                c = sss.get();
            }
            sss.unget();

            sss >> refidx;
            if (sss.fail()) {
                std::cerr << "Skip due to idx: " << line << std::endl;
                return;
            }

            self->refractive_energy.push_back(ene);
            self->refractive_index.push_back(refidx);

        }

        std::string CnvFromPayloadDB::get_key(const std::string& line, const std::string& tag) {
            size_t t1st = line.find_first_not_of(" ", tag.size());
            size_t t2nd = line.find(tag, tag.size());
            if (t2nd == std::string::npos) {
                std::cerr << "can't find the second tag "
                          << tag
                          << std::endl;
            } else {
                t2nd -= t1st;
            }
            std::string key = line.substr(t1st, t2nd);
            boost::trim(key);
            return key;
        }


    } // namespace Testing

} // namespace CondDB

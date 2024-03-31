#ifndef Testing_CD_LS_MatProp_h
#define Testing_CD_LS_MatProp_h

#include <CondDB/ICondDBSvc.h>
#include <CondDB/IRepo.h>
#include <CondDB/ICnvFromPayload.h>
// CD.LS.MatProp
namespace CondDB {
    namespace Testing {
        struct CnvFromPayloadROOT;
        struct CnvFromPayloadDB;

        struct CD_LS_MatProp: IConditionObject {

            ICnvFromPayload* getCnv(const std::string& streamer);

            CD_LS_MatProp();

            void dump();

            double light_yield; // scalar

            std::vector<double> refractive_energy; // vector
            std::vector<double> refractive_index;

            std::string Data;// data in payload
        private:
            CnvFromPayloadROOT* cnvFromROOT;
            CnvFromPayloadDB* cnvFromDB;
        }; // struct CD_LS_MatProp

        struct CnvFromPayloadROOT: ICnvFromPayload {
            bool cnv(const Payload& payload);
            bool init(CD_LS_MatProp* self_);

            CD_LS_MatProp* self;
        };

        struct CnvFromPayloadDB: ICnvFromPayload {
            bool cnv(const Payload& payload);
            bool init(CD_LS_MatProp* self_);

            void load(const char* buf);
            void parse_light_yield(const std::string& line);
            void parse_refractive(const std::string& line);

            std::string get_key(const std::string& line, const std::string& tag);

            CD_LS_MatProp* self;
        };


    } // namespace Testing
} // CondDB
#endif

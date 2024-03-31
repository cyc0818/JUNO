#include "CondObj/CommonCondObj.h"
#include "TBase64.h"

namespace CondObj { 
    namespace Common {
        CondDB::ICnvFromPayload* CommonCondObj::getCnv(const std::string& streamer) {
            // todo: need streamer types
            if (streamer.size() == 0) {
                cnvFromDB = new CnvCommonCondObjFromDB(CnvCommonCondObjFromDB::kPathFromPath);
                cnvFromDB->init(this);

            } else {
                cnvFromDB = new CnvCommonCondObjFromDB(CnvCommonCondObjFromDB::kPathFromData);
                cnvFromDB->init(this);
            }

            return cnvFromDB;
        }

        CommonCondObj::CommonCondObj() {
            
        }

        const std::string& CommonCondObj::path() {
            return m_path;
        }

        void CommonCondObj::set_path(const std::string& val) {
            // update the path
            m_path = val;
        }

        CnvCommonCondObjFromDB::CnvCommonCondObjFromDB(kPathMode mode)
            : self(nullptr), m_mode(mode) {

        }

        bool CnvCommonCondObjFromDB::cnv(const CondDB::Payload& payload) {

            if (m_mode == kPathFromData) {
                const std::string& data = payload.data();
                TString decoded_data = TBase64::Decode(data.c_str());

                self->set_path(decoded_data.Data());
            } else if (m_mode == kPathFromPath) {
                const std::string& path = payload.path();

                self->set_path(path);
            }
            return true;
        }

        bool CnvCommonCondObjFromDB::init(CommonCondObj* self_) {
            self = self_;
            return true;
        }

    } // end namespace Common
} // end namespace CondObj

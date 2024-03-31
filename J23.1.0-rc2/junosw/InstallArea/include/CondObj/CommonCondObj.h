#ifndef CondObj_CommonCondObj_h
#define CondObj_CommonCondObj_h

#include <CondDB/IConditionObject.h>
#include <CondDB/ICnvFromPayload.h>

namespace CondObj {
    namespace Common {
        class CnvCommonCondObjFromDB;
        // This is a common type of condition objects which return the path based on the payload
        // 
        class CommonCondObj: public CondDB::IConditionObject {
        public:
            CommonCondObj();

            CondDB::ICnvFromPayload* getCnv(const std::string& streamer);

            // return the path
            const std::string& path();
            // setter of path
            void set_path(const std::string& val);
        private:
            std::string m_path;
            CnvCommonCondObjFromDB* cnvFromDB;
        };

        class CnvCommonCondObjFromDB: public CondDB::ICnvFromPayload {
        public:
            // kPathFromPath: use the path in Payload
            // kPathFromData: decode the data in Payload and use it as path
            enum kPathMode {kPathFromPath, kPathFromData};
            CnvCommonCondObjFromDB(kPathMode);
            

            bool cnv(const CondDB::Payload& payload);
            bool init(CommonCondObj* self_);

        private:
            CommonCondObj* self;
            kPathMode m_mode;
        };

    }
}

#endif

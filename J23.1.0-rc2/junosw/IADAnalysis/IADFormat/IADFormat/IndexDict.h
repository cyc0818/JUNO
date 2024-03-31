#ifndef INDEX_DICT_H
#define INDEX_DICT_H

#include <vector>
#include <map>
#include "IADFormat/Buffer.h"
#include "IADFormat/IADDefine.h"
#include <mutex>

namespace IAD
{
    // How to retrieve the EDM path
    // There is no global ID of EDM file
    // In each IAD file, the ID of EDM files starts from 0
    // Each IADEvent has the corresponding ID
    // So to locate the full Event we need: IAD file ID, EDM ID, entry
    // The IAD file ID is generated dynamically when reading the IAD files
    class IndexDict
    {
    private:
        static IndexDict *s_obj;
        std::vector<std::map<uint32_t, std::string>> m_indexMap; // Store the index map in IAD files for querries
        std::mutex m_mapMutex;
        IndexDict(){};

    public:
        static IndexDict *instance();
        ~IndexDict(){};

        const std::string& getPath(uint32_t pathId, int fileId = 0);
        // Insert the metaBlock to the Index Dict, the dynamic ID of IADFile is returned
        int insertMap(const std::map<uint32_t, std::string> &indexMap);
        const std::map<uint32_t, std::string> &getMap(int fileId = 0);
    };
};

#endif
#ifndef META_BLOCK_H
#define META_BLOCK_H

#include <vector>
#include <map>
#include <string>
#include "IADFormat/Buffer.h"
#include "IADFormat/IADDefine.h"

namespace IAD
{
    /*
        structure of MetaBlock in the file
        At beginning is the header of block
        The following is the index map
        |*****| MetaBlockHeader |********|
        |*****| IndexItemHeader |********|
        |*****|    path(c_str)  |********|
                     ....
        |*****| IndexItemHeader |********|
        |*****|    path(c_str)  |********|
    */
    struct IndexItemHeader
    {
        uint32_t marker;
        uint32_t headerSize;
        uint32_t dataSize;
        uint32_t dataId;
        //-------------------------------------------
        uint32_t pathId;
    };

    struct MetaBlockHeader
    {
        // BaseHeader part
        uint32_t marker;
        uint32_t headerSize;
        uint32_t dataSize;
        uint32_t dataId;
        //----------------------------------------------
        uint32_t blockNum;
        uint32_t eventNum;
        uint32_t indexNum;

        MetaBlockHeader() : marker(Base_header_marker),
                            headerSize(sizeof(MetaBlockHeader)),
                            dataSize(0),
                            dataId(0),
                            blockNum(0),
                            eventNum(0),
                            indexNum(0) {}
    };

    class MetaBlock
    {
    private:
        MetaBlockHeader m_header;
        std::map<uint32_t, std::string> m_index2Path;

    public:
        inline MetaBlock(const std::map<uint32_t, std::string> &index2Path); // Constructing the metaBlock from index map, when writing IAD file
        MetaBlock(BufferPtr buffer);                                         // Construct MetaBlock from buffer read from file
        inline MetaBlock(const MetaBlockHeader &header);                     // Constuct default MetaBlock（without index map）

        inline void syncHeader(const MetaBlockHeader &header); // update meta information
        uint32_t countSize();                                  // Count the size after serialization
        uint32_t streamOut(void *dest);                        // Write to dest, return the size

        inline std::map<uint32_t, std::string> getMap() { return m_index2Path; };
    };

    MetaBlock::MetaBlock(const std::map<uint32_t, std::string> &index2Path) : m_header(), m_index2Path(index2Path)
    {
    }

    MetaBlock::MetaBlock(const MetaBlockHeader &header) : m_header(header) {}

    void MetaBlock::syncHeader(const MetaBlockHeader &header)
    {
        int indexNum = m_header.indexNum;
        m_header = header;
        m_header.indexNum = indexNum;
    }
}

#endif
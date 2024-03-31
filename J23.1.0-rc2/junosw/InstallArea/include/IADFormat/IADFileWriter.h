#ifndef IAD_FILE_WRITER_H
#define IAD_FILE_WRITER_H

#include "IADFormat/IADDefine.h"
#include <string>
#include "IADFormat/Buffer.h"
#include "IADFormat/IWriter.h"
#include "IADFormat/MetaBlock.h"

namespace IAD
{   
    class IADFileWriter{
    private:
        
        std::string m_fileName;
        int m_fileDescriptor;
        FileStartRecord m_startRecord;
        uint64_t m_offset;

        MetaBlockHeader m_metaHeader;
        bool m_isopen;
    public:
        IADFileWriter(const std::string& fileName, uint32_t startBlockSize = 0);
        virtual bool writeBuffer(BufferPtr buffer, uint32_t marker = Seperator_record_marker);

        bool writeMetaBlock(MetaBlock& metaBlock);//write MetaBlock。and close the file
        virtual ~IADFileWriter();
        //What happens at the end of writing the file
        //From the perspective of the writer, it just write buffer, and cares nothing
        //When writeMetaBlock() is called, writer will close the file
        //If writeMetaBlock() is not called, it will generate a default one, write it and close the file
        //meta block contains the map from the number to the path of EDM file, the index
        //According the information in MetaBlock, a dictionary of EDM paths can be built. it is used, when loading the full event of IAD event
        //There is interface to generate MetaBlock from the index dictionary
    };
}

#endif

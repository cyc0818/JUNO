#ifndef IAD_DEFINE_H
#define IAD_DEFINE_H

#include <cstdint>

// File structure
/*
|*****File_Start_record*****|
|_____|               |_____|
|_____|FileStart_block|_____|    the blocks at the begin and end are used to store the metadata
|_____|               |_____|
|*****Seperator_record******|
|_____|               |_____|
|_____|   Data_block  |_____|
|_____|_______________|_____|
|*****Seperator_record******|
|_____|               |_____|
|_____|   Data_block  |_____|
|_____|_______________|_____|
|           ....            |
|           ....            |
|*****Seperator_record******|    seperator at the end has a special marker to tell the reader that the file is end
|*****MetaData_block********|  The end is MetaBlock. it contains the index information

There ia a one to one correspondence between buffer and block.
reader and wirte read/write data in the unit of buffer. When data is loaded in memory, it is managed by Block
*/

namespace IAD
{ //*******************************data seperator*********************************
    const uint32_t File_start_marker = 0xffaa01ff;
    struct FileStartRecord
    {
        uint32_t marker;
        uint32_t recordSize;
        uint32_t startBlockSize;
        uint32_t dataSeperatorSize;
        uint32_t fileNumber;
        uint32_t fileVersion;
        uint32_t dataVersion;
        uint64_t metaBufferLocte; // the locate of metaBlock, it is behind the last seperator

        FileStartRecord() : marker(File_start_marker), recordSize(sizeof(FileStartRecord)), startBlockSize(0), metaBufferLocte(0) {}
    };

    const uint32_t Seperator_record_marker = 0xffaa02ff;
    const uint32_t End_Seperator_marker = 0xffaa03ff;
    struct SeperatorRecord
    {
        uint32_t marker;
        uint32_t recordSize;
        uint32_t dataSize;
    };
    //****************************************************************************
    const uint32_t Base_header_marker = 0xeeaa01ee;
    struct BaseHeader
    {
        uint32_t marker;
        uint32_t headerSize;
        uint32_t dataSize;
        uint32_t dataId;
    };

    struct BlockHeader
    {
        uint32_t marker;
        uint32_t headerSize;
        uint32_t dataSize;
        uint32_t dataId;
        //--------------------------------------
        uint32_t dynamicId;      // it is dynamically specified when reading IAD files
        uint32_t zipLevel;       // level 0, no compression. the defult is 1
        uint32_t preZipDataSize; // data Size before compression
    };
}

#endif
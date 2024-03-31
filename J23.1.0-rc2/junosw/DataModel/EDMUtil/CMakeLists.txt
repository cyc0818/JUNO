
EDM(EDMUtil
        src/BookEDM.cc
        src/EDMManager.cc
        src/InputElementKeeper.cc
        src/InputFileManager.cc
        src/JunoEDMDefinitions.cc
        src/PassiveStream.cc
        src/RootFileInter.cc
        src/SmartRefTable.cc
        src/SmartRefTableImpl.cc
    BASE
    NOXOD
    FILES
        FileMetaData
        JobInfo
        SmartRef
        UniqueIDTable
    HDRS
        BookEDM
        EDMManager
        InputElementKeeper
        RootFileInter
    DEPENDS
        BaseEvent
        # SNiPER::SniperKernel
)

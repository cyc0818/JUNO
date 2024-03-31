PKG(CondDB
    FILES
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/CondDBSvcBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/DBRepoBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/GlobalTagBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/GlobalTagBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/ICnvFromPayloadBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/ICondDBSvcBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/IConditionObjectBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/IOVBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/IRepoBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/LocalFSRepoBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/ModuleDef.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/PayloadBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/TagBinding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/Testing_CD_LS_MatProp_Binding.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:tests/TestCondDBAlg.cc>
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:tests/Testing_CD_LS_MatProp.cc>
    DEPENDS
        frontier_client
        DBISvc dbi
        Boost::filesystem Boost::system # used by local fs repo
)

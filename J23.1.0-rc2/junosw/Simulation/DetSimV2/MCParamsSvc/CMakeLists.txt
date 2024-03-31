include (PKG)
PKG(MCParamsSvc
    DEPENDS
        Boost::filesystem Boost::system
        Rest
        ParaDB DBISvc dbi
)

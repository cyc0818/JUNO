
PKG(Geometry
    DEPENDS
        Identifier
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:Parameter>
        Boost::filesystem Boost::system
        Boost::python Python::Python
        ROOT::Geom
    SETENV
        JUNO_GEOMETRY_PATH="$ENV{JUNOTOP}/data/Detector/Geometry"
)


include(PKG)
PKG(JunoTimer
    FILES 
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:binding/JunoTimerBinding.cc>#if built for on-line, do not bind python 
 )
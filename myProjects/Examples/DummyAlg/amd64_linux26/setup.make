----------> uses
# use SniperKernel v*  (no_version_directory)
#   use SniperPolicy v*  (no_version_directory)
#   use Boost v* Externals (no_version_directory)
#     use Python v* Externals (no_version_directory)
# use RootWriter v* SniperSvc (no_version_directory)
#   use SniperKernel v*  (no_version_directory)
#   use ROOT v* Externals (no_version_directory)
#   use Boost v* Externals (no_version_directory)
#
# Selection :
use CMT v1r26 (/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/ExternalLibs)
use ROOT v0 Externals (/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/ExternalInterface)
use Python v0 Externals (/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/ExternalInterface)
use Boost v0 Externals (/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/ExternalInterface)
use SniperPolicy v0  (/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/sniper)
use SniperKernel v2  (/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/sniper)
use RootWriter v0 SniperSvc (/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/sniper)
----------> tags
CMTv1 (from CMTVERSION)
CMTr26 (from CMTVERSION)
CMTp0 (from CMTVERSION)
Linux (from uname) package [CMT] implies [Unix]
amd64_linux26 (from CMTCONFIG)
sniper_no_config (from PROJECT) excludes [sniper_config]
sniper_root (from PROJECT) excludes [sniper_no_root]
sniper_cleanup (from PROJECT) excludes [sniper_no_cleanup]
sniper_scripts (from PROJECT) excludes [sniper_no_scripts]
sniper_prototypes (from PROJECT) excludes [sniper_no_prototypes]
sniper_with_installarea (from PROJECT) excludes [sniper_without_installarea]
sniper_without_version_directory (from PROJECT) excludes [sniper_with_version_directory]
ExternalInterface_no_config (from PROJECT) excludes [ExternalInterface_config]
ExternalInterface_no_root (from PROJECT) excludes [ExternalInterface_root]
ExternalInterface_cleanup (from PROJECT) excludes [ExternalInterface_no_cleanup]
ExternalInterface_scripts (from PROJECT) excludes [ExternalInterface_no_scripts]
ExternalInterface_prototypes (from PROJECT) excludes [ExternalInterface_no_prototypes]
ExternalInterface_without_installarea (from PROJECT) excludes [ExternalInterface_with_installarea]
ExternalInterface_without_version_directory (from PROJECT) excludes [ExternalInterface_with_version_directory]
sniper (from PROJECT)
x86_64 (from package CMT)
sl74 (from package CMT)
gcc485 (from package CMT)
Unix (from package CMT) excludes [WIN32 Win32]
----------> CMTPATH
# Add path /afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/sniper from initialization
# Add path /afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC7/Release/J17v1r1/ExternalInterface from ProjectPath

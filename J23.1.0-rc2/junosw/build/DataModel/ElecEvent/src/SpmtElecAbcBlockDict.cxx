// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecEventdIsrcdISpmtElecAbcBlockDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/SpmtElecAbcBlock.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLSpmtElecAbcBlock(void *p = nullptr);
   static void *newArray_JMcLcLSpmtElecAbcBlock(Long_t size, void *p);
   static void delete_JMcLcLSpmtElecAbcBlock(void *p);
   static void deleteArray_JMcLcLSpmtElecAbcBlock(void *p);
   static void destruct_JMcLcLSpmtElecAbcBlock(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::SpmtElecAbcBlock*)
   {
      ::JM::SpmtElecAbcBlock *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::SpmtElecAbcBlock >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::SpmtElecAbcBlock", ::JM::SpmtElecAbcBlock::Class_Version(), "Event/SpmtElecAbcBlock.h", 37,
                  typeid(::JM::SpmtElecAbcBlock), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::SpmtElecAbcBlock::Dictionary, isa_proxy, 4,
                  sizeof(::JM::SpmtElecAbcBlock) );
      instance.SetNew(&new_JMcLcLSpmtElecAbcBlock);
      instance.SetNewArray(&newArray_JMcLcLSpmtElecAbcBlock);
      instance.SetDelete(&delete_JMcLcLSpmtElecAbcBlock);
      instance.SetDeleteArray(&deleteArray_JMcLcLSpmtElecAbcBlock);
      instance.SetDestructor(&destruct_JMcLcLSpmtElecAbcBlock);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::SpmtElecAbcBlock*)
   {
      return GenerateInitInstanceLocal((::JM::SpmtElecAbcBlock*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::SpmtElecAbcBlock*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr SpmtElecAbcBlock::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SpmtElecAbcBlock::Class_Name()
{
   return "JM::SpmtElecAbcBlock";
}

//______________________________________________________________________________
const char *SpmtElecAbcBlock::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecAbcBlock*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SpmtElecAbcBlock::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecAbcBlock*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SpmtElecAbcBlock::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecAbcBlock*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SpmtElecAbcBlock::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecAbcBlock*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void SpmtElecAbcBlock::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::SpmtElecAbcBlock.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::SpmtElecAbcBlock::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::SpmtElecAbcBlock::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLSpmtElecAbcBlock(void *p) {
      return  p ? new(p) ::JM::SpmtElecAbcBlock : new ::JM::SpmtElecAbcBlock;
   }
   static void *newArray_JMcLcLSpmtElecAbcBlock(Long_t nElements, void *p) {
      return p ? new(p) ::JM::SpmtElecAbcBlock[nElements] : new ::JM::SpmtElecAbcBlock[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLSpmtElecAbcBlock(void *p) {
      delete ((::JM::SpmtElecAbcBlock*)p);
   }
   static void deleteArray_JMcLcLSpmtElecAbcBlock(void *p) {
      delete [] ((::JM::SpmtElecAbcBlock*)p);
   }
   static void destruct_JMcLcLSpmtElecAbcBlock(void *p) {
      typedef ::JM::SpmtElecAbcBlock current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::SpmtElecAbcBlock

namespace {
  void TriggerDictionaryInitialization_SpmtElecAbcBlockDict_Impl() {
    static const char* headers[] = {
"Event/SpmtElecAbcBlock.h",
nullptr
    };
    static const char* includePaths[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Python/3.9.14/include/python3.9",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Boost/1.78.0",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/log4cpp/1.1.3/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/CLHEP/2.4.1.0/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/CLHEP/2.4.1.0/lib/CLHEP-2.4.1.0/../../include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Geant4/10.04.p02.juno/include/geant4",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/HepMC/2.06.11/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/genie/3.02.00/include/GENIE",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/frontier/2.9.1/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/mysql-connector-c/6.1.9/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/mysql-connector-cpp/1.1.12/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/libyaml/0.2.4/include",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/libonnxruntime/1.11.1/include/onnxruntime",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/nuwro/21.09.2/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SpmtElecAbcBlockDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/SpmtElecAbcBlock.h")))  SpmtElecAbcBlock;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SpmtElecAbcBlockDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/SpmtElecAbcBlock.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::SpmtElecAbcBlock", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SpmtElecAbcBlockDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SpmtElecAbcBlockDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SpmtElecAbcBlockDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SpmtElecAbcBlockDict() {
  TriggerDictionaryInitialization_SpmtElecAbcBlockDict_Impl();
}

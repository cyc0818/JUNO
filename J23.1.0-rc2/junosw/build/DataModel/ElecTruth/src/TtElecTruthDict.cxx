// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecTruthdIsrcdITtElecTruthDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth/Event/TtElecTruth.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLTtElecTruth(void *p = nullptr);
   static void *newArray_JMcLcLTtElecTruth(Long_t size, void *p);
   static void delete_JMcLcLTtElecTruth(void *p);
   static void deleteArray_JMcLcLTtElecTruth(void *p);
   static void destruct_JMcLcLTtElecTruth(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::TtElecTruth*)
   {
      ::JM::TtElecTruth *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::TtElecTruth >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::TtElecTruth", ::JM::TtElecTruth::Class_Version(), "Event/TtElecTruth.h", 39,
                  typeid(::JM::TtElecTruth), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::TtElecTruth::Dictionary, isa_proxy, 4,
                  sizeof(::JM::TtElecTruth) );
      instance.SetNew(&new_JMcLcLTtElecTruth);
      instance.SetNewArray(&newArray_JMcLcLTtElecTruth);
      instance.SetDelete(&delete_JMcLcLTtElecTruth);
      instance.SetDeleteArray(&deleteArray_JMcLcLTtElecTruth);
      instance.SetDestructor(&destruct_JMcLcLTtElecTruth);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::TtElecTruth*)
   {
      return GenerateInitInstanceLocal((::JM::TtElecTruth*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::TtElecTruth*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr TtElecTruth::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TtElecTruth::Class_Name()
{
   return "JM::TtElecTruth";
}

//______________________________________________________________________________
const char *TtElecTruth::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruth*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TtElecTruth::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruth*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TtElecTruth::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruth*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TtElecTruth::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruth*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void TtElecTruth::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::TtElecTruth.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::TtElecTruth::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::TtElecTruth::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLTtElecTruth(void *p) {
      return  p ? new(p) ::JM::TtElecTruth : new ::JM::TtElecTruth;
   }
   static void *newArray_JMcLcLTtElecTruth(Long_t nElements, void *p) {
      return p ? new(p) ::JM::TtElecTruth[nElements] : new ::JM::TtElecTruth[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLTtElecTruth(void *p) {
      delete ((::JM::TtElecTruth*)p);
   }
   static void deleteArray_JMcLcLTtElecTruth(void *p) {
      delete [] ((::JM::TtElecTruth*)p);
   }
   static void destruct_JMcLcLTtElecTruth(void *p) {
      typedef ::JM::TtElecTruth current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::TtElecTruth

namespace {
  void TriggerDictionaryInitialization_TtElecTruthDict_Impl() {
    static const char* headers[] = {
"Event/TtElecTruth.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecTruth",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecTruth/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TtElecTruthDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/TtElecTruth.h")))  TtElecTruth;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TtElecTruthDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/TtElecTruth.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::TtElecTruth", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TtElecTruthDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TtElecTruthDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TtElecTruthDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TtElecTruthDict() {
  TriggerDictionaryInitialization_TtElecTruthDict_Impl();
}

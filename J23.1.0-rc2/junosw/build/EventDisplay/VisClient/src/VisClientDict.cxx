// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIEventDisplaydIVisClientdIsrcdIVisClientDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/VisClient/VisClient.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_VisClient(void *p);
   static void deleteArray_VisClient(void *p);
   static void destruct_VisClient(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VisClient*)
   {
      ::VisClient *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::VisClient >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("VisClient", ::VisClient::Class_Version(), "VisClient/VisClient.h", 35,
                  typeid(::VisClient), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::VisClient::Dictionary, isa_proxy, 4,
                  sizeof(::VisClient) );
      instance.SetDelete(&delete_VisClient);
      instance.SetDeleteArray(&deleteArray_VisClient);
      instance.SetDestructor(&destruct_VisClient);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VisClient*)
   {
      return GenerateInitInstanceLocal((::VisClient*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::VisClient*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr VisClient::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *VisClient::Class_Name()
{
   return "VisClient";
}

//______________________________________________________________________________
const char *VisClient::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::VisClient*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int VisClient::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::VisClient*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *VisClient::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::VisClient*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *VisClient::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::VisClient*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void VisClient::Streamer(TBuffer &R__b)
{
   // Stream an object of class VisClient.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(VisClient::Class(),this);
   } else {
      R__b.WriteClassBuffer(VisClient::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_VisClient(void *p) {
      delete ((::VisClient*)p);
   }
   static void deleteArray_VisClient(void *p) {
      delete [] ((::VisClient*)p);
   }
   static void destruct_VisClient(void *p) {
      typedef ::VisClient current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::VisClient

namespace {
  void TriggerDictionaryInitialization_VisClientDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/VisClient/VisClient.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/src",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/Event",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/src",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/EventDisplay/VisClient/Event",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/EventDisplay/VisClient/src",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/EventDisplay/VisClient/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "VisClientDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(Vis Client)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/VisClient/VisClient.h")))  VisClient;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "VisClientDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/EventDisplay/VisClient/VisClient/VisClient.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"VisClient", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("VisClientDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_VisClientDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_VisClientDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_VisClientDict() {
  TriggerDictionaryInitialization_VisClientDict_Impl();
}

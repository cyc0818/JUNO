// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIRecEventdIsrcdITtRecHeaderDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent/Event/TtRecHeader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLTtRecHeader(void *p = nullptr);
   static void *newArray_JMcLcLTtRecHeader(Long_t size, void *p);
   static void delete_JMcLcLTtRecHeader(void *p);
   static void deleteArray_JMcLcLTtRecHeader(void *p);
   static void destruct_JMcLcLTtRecHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::TtRecHeader*)
   {
      ::JM::TtRecHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::TtRecHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::TtRecHeader", ::JM::TtRecHeader::Class_Version(), "Event/TtRecHeader.h", 39,
                  typeid(::JM::TtRecHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::TtRecHeader::Dictionary, isa_proxy, 4,
                  sizeof(::JM::TtRecHeader) );
      instance.SetNew(&new_JMcLcLTtRecHeader);
      instance.SetNewArray(&newArray_JMcLcLTtRecHeader);
      instance.SetDelete(&delete_JMcLcLTtRecHeader);
      instance.SetDeleteArray(&deleteArray_JMcLcLTtRecHeader);
      instance.SetDestructor(&destruct_JMcLcLTtRecHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::TtRecHeader*)
   {
      return GenerateInitInstanceLocal((::JM::TtRecHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::TtRecHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr TtRecHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TtRecHeader::Class_Name()
{
   return "JM::TtRecHeader";
}

//______________________________________________________________________________
const char *TtRecHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtRecHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TtRecHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtRecHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TtRecHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtRecHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TtRecHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtRecHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void TtRecHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::TtRecHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::TtRecHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::TtRecHeader::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLTtRecHeader(void *p) {
      return  p ? new(p) ::JM::TtRecHeader : new ::JM::TtRecHeader;
   }
   static void *newArray_JMcLcLTtRecHeader(Long_t nElements, void *p) {
      return p ? new(p) ::JM::TtRecHeader[nElements] : new ::JM::TtRecHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLTtRecHeader(void *p) {
      delete ((::JM::TtRecHeader*)p);
   }
   static void deleteArray_JMcLcLTtRecHeader(void *p) {
      delete [] ((::JM::TtRecHeader*)p);
   }
   static void destruct_JMcLcLTtRecHeader(void *p) {
      typedef ::JM::TtRecHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::TtRecHeader

namespace {
  void TriggerDictionaryInitialization_TtRecHeaderDict_Impl() {
    static const char* headers[] = {
"Event/TtRecHeader.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/RecEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/RecEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TtRecHeaderDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/TtRecHeader.h")))  TtRecHeader;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TtRecHeaderDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/TtRecHeader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::TtRecHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TtRecHeaderDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TtRecHeaderDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TtRecHeaderDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TtRecHeaderDict() {
  TriggerDictionaryInitialization_TtRecHeaderDict_Impl();
}

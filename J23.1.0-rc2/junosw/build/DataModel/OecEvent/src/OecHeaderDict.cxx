// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIOecEventdIsrcdIOecHeaderDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecHeader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLOecHeader(void *p = nullptr);
   static void *newArray_JMcLcLOecHeader(Long_t size, void *p);
   static void delete_JMcLcLOecHeader(void *p);
   static void deleteArray_JMcLcLOecHeader(void *p);
   static void destruct_JMcLcLOecHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::OecHeader*)
   {
      ::JM::OecHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::OecHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::OecHeader", ::JM::OecHeader::Class_Version(), "Event/OecHeader.h", 11,
                  typeid(::JM::OecHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::OecHeader::Dictionary, isa_proxy, 4,
                  sizeof(::JM::OecHeader) );
      instance.SetNew(&new_JMcLcLOecHeader);
      instance.SetNewArray(&newArray_JMcLcLOecHeader);
      instance.SetDelete(&delete_JMcLcLOecHeader);
      instance.SetDeleteArray(&deleteArray_JMcLcLOecHeader);
      instance.SetDestructor(&destruct_JMcLcLOecHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::OecHeader*)
   {
      return GenerateInitInstanceLocal((::JM::OecHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::OecHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr OecHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *OecHeader::Class_Name()
{
   return "JM::OecHeader";
}

//______________________________________________________________________________
const char *OecHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OecHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int OecHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OecHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *OecHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OecHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *OecHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OecHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void OecHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::OecHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::OecHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::OecHeader::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLOecHeader(void *p) {
      return  p ? new(p) ::JM::OecHeader : new ::JM::OecHeader;
   }
   static void *newArray_JMcLcLOecHeader(Long_t nElements, void *p) {
      return p ? new(p) ::JM::OecHeader[nElements] : new ::JM::OecHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLOecHeader(void *p) {
      delete ((::JM::OecHeader*)p);
   }
   static void deleteArray_JMcLcLOecHeader(void *p) {
      delete [] ((::JM::OecHeader*)p);
   }
   static void destruct_JMcLcLOecHeader(void *p) {
      typedef ::JM::OecHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::OecHeader

namespace {
  void TriggerDictionaryInitialization_OecHeaderDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecHeader.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/OecEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/OecEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "OecHeaderDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecHeader.h")))  OecHeader;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "OecHeaderDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecHeader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::OecHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("OecHeaderDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_OecHeaderDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_OecHeaderDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_OecHeaderDict() {
  TriggerDictionaryInitialization_OecHeaderDict_Impl();
}

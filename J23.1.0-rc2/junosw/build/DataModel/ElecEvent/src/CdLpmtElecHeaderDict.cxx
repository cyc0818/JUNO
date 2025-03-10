// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecEventdIsrcdICdLpmtElecHeaderDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdLpmtElecHeader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdLpmtElecHeader(void *p = nullptr);
   static void *newArray_JMcLcLCdLpmtElecHeader(Long_t size, void *p);
   static void delete_JMcLcLCdLpmtElecHeader(void *p);
   static void deleteArray_JMcLcLCdLpmtElecHeader(void *p);
   static void destruct_JMcLcLCdLpmtElecHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdLpmtElecHeader*)
   {
      ::JM::CdLpmtElecHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdLpmtElecHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdLpmtElecHeader", ::JM::CdLpmtElecHeader::Class_Version(), "Event/CdLpmtElecHeader.h", 39,
                  typeid(::JM::CdLpmtElecHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdLpmtElecHeader::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdLpmtElecHeader) );
      instance.SetNew(&new_JMcLcLCdLpmtElecHeader);
      instance.SetNewArray(&newArray_JMcLcLCdLpmtElecHeader);
      instance.SetDelete(&delete_JMcLcLCdLpmtElecHeader);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdLpmtElecHeader);
      instance.SetDestructor(&destruct_JMcLcLCdLpmtElecHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdLpmtElecHeader*)
   {
      return GenerateInitInstanceLocal((::JM::CdLpmtElecHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdLpmtElecHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdLpmtElecHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdLpmtElecHeader::Class_Name()
{
   return "JM::CdLpmtElecHeader";
}

//______________________________________________________________________________
const char *CdLpmtElecHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdLpmtElecHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdLpmtElecHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdLpmtElecHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdLpmtElecHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdLpmtElecHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdLpmtElecHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdLpmtElecHeader::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdLpmtElecHeader(void *p) {
      return  p ? new(p) ::JM::CdLpmtElecHeader : new ::JM::CdLpmtElecHeader;
   }
   static void *newArray_JMcLcLCdLpmtElecHeader(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdLpmtElecHeader[nElements] : new ::JM::CdLpmtElecHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdLpmtElecHeader(void *p) {
      delete ((::JM::CdLpmtElecHeader*)p);
   }
   static void deleteArray_JMcLcLCdLpmtElecHeader(void *p) {
      delete [] ((::JM::CdLpmtElecHeader*)p);
   }
   static void destruct_JMcLcLCdLpmtElecHeader(void *p) {
      typedef ::JM::CdLpmtElecHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdLpmtElecHeader

namespace {
  void TriggerDictionaryInitialization_CdLpmtElecHeaderDict_Impl() {
    static const char* headers[] = {
"Event/CdLpmtElecHeader.h",
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
#line 1 "CdLpmtElecHeaderDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdLpmtElecHeader.h")))  CdLpmtElecHeader;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdLpmtElecHeaderDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdLpmtElecHeader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdLpmtElecHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdLpmtElecHeaderDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdLpmtElecHeaderDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdLpmtElecHeaderDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdLpmtElecHeaderDict() {
  TriggerDictionaryInitialization_CdLpmtElecHeaderDict_Impl();
}

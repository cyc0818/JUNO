// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIRecEventdIsrcdICdTrackRecHeaderDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent/Event/CdTrackRecHeader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdTrackRecHeader(void *p = nullptr);
   static void *newArray_JMcLcLCdTrackRecHeader(Long_t size, void *p);
   static void delete_JMcLcLCdTrackRecHeader(void *p);
   static void deleteArray_JMcLcLCdTrackRecHeader(void *p);
   static void destruct_JMcLcLCdTrackRecHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdTrackRecHeader*)
   {
      ::JM::CdTrackRecHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdTrackRecHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdTrackRecHeader", ::JM::CdTrackRecHeader::Class_Version(), "Event/CdTrackRecHeader.h", 39,
                  typeid(::JM::CdTrackRecHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdTrackRecHeader::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdTrackRecHeader) );
      instance.SetNew(&new_JMcLcLCdTrackRecHeader);
      instance.SetNewArray(&newArray_JMcLcLCdTrackRecHeader);
      instance.SetDelete(&delete_JMcLcLCdTrackRecHeader);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdTrackRecHeader);
      instance.SetDestructor(&destruct_JMcLcLCdTrackRecHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdTrackRecHeader*)
   {
      return GenerateInitInstanceLocal((::JM::CdTrackRecHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdTrackRecHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdTrackRecHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdTrackRecHeader::Class_Name()
{
   return "JM::CdTrackRecHeader";
}

//______________________________________________________________________________
const char *CdTrackRecHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdTrackRecHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdTrackRecHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdTrackRecHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdTrackRecHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdTrackRecHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdTrackRecHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdTrackRecHeader::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdTrackRecHeader(void *p) {
      return  p ? new(p) ::JM::CdTrackRecHeader : new ::JM::CdTrackRecHeader;
   }
   static void *newArray_JMcLcLCdTrackRecHeader(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdTrackRecHeader[nElements] : new ::JM::CdTrackRecHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdTrackRecHeader(void *p) {
      delete ((::JM::CdTrackRecHeader*)p);
   }
   static void deleteArray_JMcLcLCdTrackRecHeader(void *p) {
      delete [] ((::JM::CdTrackRecHeader*)p);
   }
   static void destruct_JMcLcLCdTrackRecHeader(void *p) {
      typedef ::JM::CdTrackRecHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdTrackRecHeader

namespace {
  void TriggerDictionaryInitialization_CdTrackRecHeaderDict_Impl() {
    static const char* headers[] = {
"Event/CdTrackRecHeader.h",
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
#line 1 "CdTrackRecHeaderDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdTrackRecHeader.h")))  CdTrackRecHeader;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdTrackRecHeaderDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdTrackRecHeader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdTrackRecHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdTrackRecHeaderDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdTrackRecHeaderDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdTrackRecHeaderDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdTrackRecHeaderDict() {
  TriggerDictionaryInitialization_CdTrackRecHeaderDict_Impl();
}

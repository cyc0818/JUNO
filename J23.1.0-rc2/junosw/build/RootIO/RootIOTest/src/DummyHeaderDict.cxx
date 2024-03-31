// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIRootIOdIRootIOTestdIsrcdIDummyHeaderDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyHeader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_DummyHeader(void *p = nullptr);
   static void *newArray_DummyHeader(Long_t size, void *p);
   static void delete_DummyHeader(void *p);
   static void deleteArray_DummyHeader(void *p);
   static void destruct_DummyHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DummyHeader*)
   {
      ::DummyHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DummyHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DummyHeader", ::DummyHeader::Class_Version(), "DummyHeader.h", 7,
                  typeid(::DummyHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DummyHeader::Dictionary, isa_proxy, 4,
                  sizeof(::DummyHeader) );
      instance.SetNew(&new_DummyHeader);
      instance.SetNewArray(&newArray_DummyHeader);
      instance.SetDelete(&delete_DummyHeader);
      instance.SetDeleteArray(&deleteArray_DummyHeader);
      instance.SetDestructor(&destruct_DummyHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DummyHeader*)
   {
      return GenerateInitInstanceLocal((::DummyHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DummyHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DummyHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DummyHeader::Class_Name()
{
   return "DummyHeader";
}

//______________________________________________________________________________
const char *DummyHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DummyHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DummyHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DummyHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DummyHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class DummyHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DummyHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(DummyHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DummyHeader(void *p) {
      return  p ? new(p) ::DummyHeader : new ::DummyHeader;
   }
   static void *newArray_DummyHeader(Long_t nElements, void *p) {
      return p ? new(p) ::DummyHeader[nElements] : new ::DummyHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_DummyHeader(void *p) {
      delete ((::DummyHeader*)p);
   }
   static void deleteArray_DummyHeader(void *p) {
      delete [] ((::DummyHeader*)p);
   }
   static void destruct_DummyHeader(void *p) {
      typedef ::DummyHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DummyHeader

namespace {
  void TriggerDictionaryInitialization_DummyHeaderDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyHeader.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/src",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/src",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RootIO/RootIOTest/Event",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RootIO/RootIOTest/src",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RootIO/RootIOTest/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DummyHeaderDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyHeader.h")))  DummyHeader;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DummyHeaderDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyHeader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"DummyHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DummyHeaderDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DummyHeaderDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DummyHeaderDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DummyHeaderDict() {
  TriggerDictionaryInitialization_DummyHeaderDict_Impl();
}

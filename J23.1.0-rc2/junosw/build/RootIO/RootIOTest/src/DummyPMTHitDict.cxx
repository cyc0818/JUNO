// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIRootIOdIRootIOTestdIsrcdIDummyPMTHitDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyPMTHit.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_DummyPMTHit(void *p = nullptr);
   static void *newArray_DummyPMTHit(Long_t size, void *p);
   static void delete_DummyPMTHit(void *p);
   static void deleteArray_DummyPMTHit(void *p);
   static void destruct_DummyPMTHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DummyPMTHit*)
   {
      ::DummyPMTHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DummyPMTHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DummyPMTHit", ::DummyPMTHit::Class_Version(), "DummyPMTHit.h", 8,
                  typeid(::DummyPMTHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DummyPMTHit::Dictionary, isa_proxy, 4,
                  sizeof(::DummyPMTHit) );
      instance.SetNew(&new_DummyPMTHit);
      instance.SetNewArray(&newArray_DummyPMTHit);
      instance.SetDelete(&delete_DummyPMTHit);
      instance.SetDeleteArray(&deleteArray_DummyPMTHit);
      instance.SetDestructor(&destruct_DummyPMTHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DummyPMTHit*)
   {
      return GenerateInitInstanceLocal((::DummyPMTHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DummyPMTHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DummyPMTHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DummyPMTHit::Class_Name()
{
   return "DummyPMTHit";
}

//______________________________________________________________________________
const char *DummyPMTHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyPMTHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DummyPMTHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyPMTHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DummyPMTHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyPMTHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DummyPMTHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyPMTHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DummyPMTHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class DummyPMTHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DummyPMTHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(DummyPMTHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DummyPMTHit(void *p) {
      return  p ? new(p) ::DummyPMTHit : new ::DummyPMTHit;
   }
   static void *newArray_DummyPMTHit(Long_t nElements, void *p) {
      return p ? new(p) ::DummyPMTHit[nElements] : new ::DummyPMTHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_DummyPMTHit(void *p) {
      delete ((::DummyPMTHit*)p);
   }
   static void deleteArray_DummyPMTHit(void *p) {
      delete [] ((::DummyPMTHit*)p);
   }
   static void destruct_DummyPMTHit(void *p) {
      typedef ::DummyPMTHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DummyPMTHit

namespace ROOT {
   static TClass *vectorlEDummyPMTHitgR_Dictionary();
   static void vectorlEDummyPMTHitgR_TClassManip(TClass*);
   static void *new_vectorlEDummyPMTHitgR(void *p = nullptr);
   static void *newArray_vectorlEDummyPMTHitgR(Long_t size, void *p);
   static void delete_vectorlEDummyPMTHitgR(void *p);
   static void deleteArray_vectorlEDummyPMTHitgR(void *p);
   static void destruct_vectorlEDummyPMTHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DummyPMTHit>*)
   {
      vector<DummyPMTHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DummyPMTHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DummyPMTHit>", -2, "vector", 389,
                  typeid(vector<DummyPMTHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDummyPMTHitgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DummyPMTHit>) );
      instance.SetNew(&new_vectorlEDummyPMTHitgR);
      instance.SetNewArray(&newArray_vectorlEDummyPMTHitgR);
      instance.SetDelete(&delete_vectorlEDummyPMTHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDummyPMTHitgR);
      instance.SetDestructor(&destruct_vectorlEDummyPMTHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DummyPMTHit> >()));

      ::ROOT::AddClassAlternate("vector<DummyPMTHit>","std::vector<DummyPMTHit, std::allocator<DummyPMTHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DummyPMTHit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDummyPMTHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DummyPMTHit>*)nullptr)->GetClass();
      vectorlEDummyPMTHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDummyPMTHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDummyPMTHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyPMTHit> : new vector<DummyPMTHit>;
   }
   static void *newArray_vectorlEDummyPMTHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyPMTHit>[nElements] : new vector<DummyPMTHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDummyPMTHitgR(void *p) {
      delete ((vector<DummyPMTHit>*)p);
   }
   static void deleteArray_vectorlEDummyPMTHitgR(void *p) {
      delete [] ((vector<DummyPMTHit>*)p);
   }
   static void destruct_vectorlEDummyPMTHitgR(void *p) {
      typedef vector<DummyPMTHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DummyPMTHit>

namespace {
  void TriggerDictionaryInitialization_DummyPMTHitDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyPMTHit.h",
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
#line 1 "DummyPMTHitDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyPMTHit.h")))  DummyPMTHit;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DummyPMTHitDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyPMTHit.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"DummyPMTHit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DummyPMTHitDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DummyPMTHitDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DummyPMTHitDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DummyPMTHitDict() {
  TriggerDictionaryInitialization_DummyPMTHitDict_Impl();
}

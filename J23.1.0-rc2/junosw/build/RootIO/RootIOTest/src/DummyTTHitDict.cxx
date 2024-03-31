// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIRootIOdIRootIOTestdIsrcdIDummyTTHitDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyTTHit.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_DummyTTHit(void *p = nullptr);
   static void *newArray_DummyTTHit(Long_t size, void *p);
   static void delete_DummyTTHit(void *p);
   static void deleteArray_DummyTTHit(void *p);
   static void destruct_DummyTTHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DummyTTHit*)
   {
      ::DummyTTHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DummyTTHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DummyTTHit", ::DummyTTHit::Class_Version(), "DummyTTHit.h", 8,
                  typeid(::DummyTTHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DummyTTHit::Dictionary, isa_proxy, 4,
                  sizeof(::DummyTTHit) );
      instance.SetNew(&new_DummyTTHit);
      instance.SetNewArray(&newArray_DummyTTHit);
      instance.SetDelete(&delete_DummyTTHit);
      instance.SetDeleteArray(&deleteArray_DummyTTHit);
      instance.SetDestructor(&destruct_DummyTTHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DummyTTHit*)
   {
      return GenerateInitInstanceLocal((::DummyTTHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DummyTTHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DummyTTHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DummyTTHit::Class_Name()
{
   return "DummyTTHit";
}

//______________________________________________________________________________
const char *DummyTTHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyTTHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DummyTTHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyTTHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DummyTTHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyTTHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DummyTTHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyTTHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DummyTTHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class DummyTTHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DummyTTHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(DummyTTHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DummyTTHit(void *p) {
      return  p ? new(p) ::DummyTTHit : new ::DummyTTHit;
   }
   static void *newArray_DummyTTHit(Long_t nElements, void *p) {
      return p ? new(p) ::DummyTTHit[nElements] : new ::DummyTTHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_DummyTTHit(void *p) {
      delete ((::DummyTTHit*)p);
   }
   static void deleteArray_DummyTTHit(void *p) {
      delete [] ((::DummyTTHit*)p);
   }
   static void destruct_DummyTTHit(void *p) {
      typedef ::DummyTTHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DummyTTHit

namespace ROOT {
   static TClass *vectorlEDummyTTHitgR_Dictionary();
   static void vectorlEDummyTTHitgR_TClassManip(TClass*);
   static void *new_vectorlEDummyTTHitgR(void *p = nullptr);
   static void *newArray_vectorlEDummyTTHitgR(Long_t size, void *p);
   static void delete_vectorlEDummyTTHitgR(void *p);
   static void deleteArray_vectorlEDummyTTHitgR(void *p);
   static void destruct_vectorlEDummyTTHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DummyTTHit>*)
   {
      vector<DummyTTHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DummyTTHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DummyTTHit>", -2, "vector", 389,
                  typeid(vector<DummyTTHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDummyTTHitgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DummyTTHit>) );
      instance.SetNew(&new_vectorlEDummyTTHitgR);
      instance.SetNewArray(&newArray_vectorlEDummyTTHitgR);
      instance.SetDelete(&delete_vectorlEDummyTTHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDummyTTHitgR);
      instance.SetDestructor(&destruct_vectorlEDummyTTHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DummyTTHit> >()));

      ::ROOT::AddClassAlternate("vector<DummyTTHit>","std::vector<DummyTTHit, std::allocator<DummyTTHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DummyTTHit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDummyTTHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DummyTTHit>*)nullptr)->GetClass();
      vectorlEDummyTTHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDummyTTHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDummyTTHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyTTHit> : new vector<DummyTTHit>;
   }
   static void *newArray_vectorlEDummyTTHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyTTHit>[nElements] : new vector<DummyTTHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDummyTTHitgR(void *p) {
      delete ((vector<DummyTTHit>*)p);
   }
   static void deleteArray_vectorlEDummyTTHitgR(void *p) {
      delete [] ((vector<DummyTTHit>*)p);
   }
   static void destruct_vectorlEDummyTTHitgR(void *p) {
      typedef vector<DummyTTHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DummyTTHit>

namespace {
  void TriggerDictionaryInitialization_DummyTTHitDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyTTHit.h",
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
#line 1 "DummyTTHitDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyTTHit.h")))  DummyTTHit;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DummyTTHitDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyTTHit.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"DummyTTHit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DummyTTHitDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DummyTTHitDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DummyTTHitDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DummyTTHitDict() {
  TriggerDictionaryInitialization_DummyTTHitDict_Impl();
}

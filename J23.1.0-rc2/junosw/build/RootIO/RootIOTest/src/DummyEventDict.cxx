// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIRootIOdIRootIOTestdIsrcdIDummyEventDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyEvent.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_DummyEvent(void *p = nullptr);
   static void *newArray_DummyEvent(Long_t size, void *p);
   static void delete_DummyEvent(void *p);
   static void deleteArray_DummyEvent(void *p);
   static void destruct_DummyEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DummyEvent*)
   {
      ::DummyEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DummyEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DummyEvent", ::DummyEvent::Class_Version(), "DummyEvent.h", 16,
                  typeid(::DummyEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DummyEvent::Dictionary, isa_proxy, 4,
                  sizeof(::DummyEvent) );
      instance.SetNew(&new_DummyEvent);
      instance.SetNewArray(&newArray_DummyEvent);
      instance.SetDelete(&delete_DummyEvent);
      instance.SetDeleteArray(&deleteArray_DummyEvent);
      instance.SetDestructor(&destruct_DummyEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DummyEvent*)
   {
      return GenerateInitInstanceLocal((::DummyEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DummyEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DummyEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DummyEvent::Class_Name()
{
   return "DummyEvent";
}

//______________________________________________________________________________
const char *DummyEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DummyEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DummyEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DummyEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DummyEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DummyEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DummyEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class DummyEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DummyEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(DummyEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DummyEvent(void *p) {
      return  p ? new(p) ::DummyEvent : new ::DummyEvent;
   }
   static void *newArray_DummyEvent(Long_t nElements, void *p) {
      return p ? new(p) ::DummyEvent[nElements] : new ::DummyEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_DummyEvent(void *p) {
      delete ((::DummyEvent*)p);
   }
   static void deleteArray_DummyEvent(void *p) {
      delete [] ((::DummyEvent*)p);
   }
   static void destruct_DummyEvent(void *p) {
      typedef ::DummyEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DummyEvent

namespace ROOT {
   static TClass *vectorlEDummyTrackmUgR_Dictionary();
   static void vectorlEDummyTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEDummyTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlEDummyTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEDummyTrackmUgR(void *p);
   static void deleteArray_vectorlEDummyTrackmUgR(void *p);
   static void destruct_vectorlEDummyTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DummyTrack*>*)
   {
      vector<DummyTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DummyTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DummyTrack*>", -2, "vector", 389,
                  typeid(vector<DummyTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDummyTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<DummyTrack*>) );
      instance.SetNew(&new_vectorlEDummyTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEDummyTrackmUgR);
      instance.SetDelete(&delete_vectorlEDummyTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDummyTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEDummyTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DummyTrack*> >()));

      ::ROOT::AddClassAlternate("vector<DummyTrack*>","std::vector<DummyTrack*, std::allocator<DummyTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DummyTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDummyTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DummyTrack*>*)nullptr)->GetClass();
      vectorlEDummyTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDummyTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDummyTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyTrack*> : new vector<DummyTrack*>;
   }
   static void *newArray_vectorlEDummyTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyTrack*>[nElements] : new vector<DummyTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDummyTrackmUgR(void *p) {
      delete ((vector<DummyTrack*>*)p);
   }
   static void deleteArray_vectorlEDummyTrackmUgR(void *p) {
      delete [] ((vector<DummyTrack*>*)p);
   }
   static void destruct_vectorlEDummyTrackmUgR(void *p) {
      typedef vector<DummyTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DummyTrack*>

namespace ROOT {
   static TClass *vectorlEDummyTTHitmUgR_Dictionary();
   static void vectorlEDummyTTHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEDummyTTHitmUgR(void *p = nullptr);
   static void *newArray_vectorlEDummyTTHitmUgR(Long_t size, void *p);
   static void delete_vectorlEDummyTTHitmUgR(void *p);
   static void deleteArray_vectorlEDummyTTHitmUgR(void *p);
   static void destruct_vectorlEDummyTTHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DummyTTHit*>*)
   {
      vector<DummyTTHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DummyTTHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DummyTTHit*>", -2, "vector", 389,
                  typeid(vector<DummyTTHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDummyTTHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<DummyTTHit*>) );
      instance.SetNew(&new_vectorlEDummyTTHitmUgR);
      instance.SetNewArray(&newArray_vectorlEDummyTTHitmUgR);
      instance.SetDelete(&delete_vectorlEDummyTTHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDummyTTHitmUgR);
      instance.SetDestructor(&destruct_vectorlEDummyTTHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DummyTTHit*> >()));

      ::ROOT::AddClassAlternate("vector<DummyTTHit*>","std::vector<DummyTTHit*, std::allocator<DummyTTHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DummyTTHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDummyTTHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DummyTTHit*>*)nullptr)->GetClass();
      vectorlEDummyTTHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDummyTTHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDummyTTHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyTTHit*> : new vector<DummyTTHit*>;
   }
   static void *newArray_vectorlEDummyTTHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyTTHit*>[nElements] : new vector<DummyTTHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDummyTTHitmUgR(void *p) {
      delete ((vector<DummyTTHit*>*)p);
   }
   static void deleteArray_vectorlEDummyTTHitmUgR(void *p) {
      delete [] ((vector<DummyTTHit*>*)p);
   }
   static void destruct_vectorlEDummyTTHitmUgR(void *p) {
      typedef vector<DummyTTHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DummyTTHit*>

namespace ROOT {
   static TClass *vectorlEDummyPMTHitmUgR_Dictionary();
   static void vectorlEDummyPMTHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEDummyPMTHitmUgR(void *p = nullptr);
   static void *newArray_vectorlEDummyPMTHitmUgR(Long_t size, void *p);
   static void delete_vectorlEDummyPMTHitmUgR(void *p);
   static void deleteArray_vectorlEDummyPMTHitmUgR(void *p);
   static void destruct_vectorlEDummyPMTHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DummyPMTHit*>*)
   {
      vector<DummyPMTHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DummyPMTHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DummyPMTHit*>", -2, "vector", 389,
                  typeid(vector<DummyPMTHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDummyPMTHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<DummyPMTHit*>) );
      instance.SetNew(&new_vectorlEDummyPMTHitmUgR);
      instance.SetNewArray(&newArray_vectorlEDummyPMTHitmUgR);
      instance.SetDelete(&delete_vectorlEDummyPMTHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDummyPMTHitmUgR);
      instance.SetDestructor(&destruct_vectorlEDummyPMTHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DummyPMTHit*> >()));

      ::ROOT::AddClassAlternate("vector<DummyPMTHit*>","std::vector<DummyPMTHit*, std::allocator<DummyPMTHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DummyPMTHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDummyPMTHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DummyPMTHit*>*)nullptr)->GetClass();
      vectorlEDummyPMTHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDummyPMTHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDummyPMTHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyPMTHit*> : new vector<DummyPMTHit*>;
   }
   static void *newArray_vectorlEDummyPMTHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DummyPMTHit*>[nElements] : new vector<DummyPMTHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDummyPMTHitmUgR(void *p) {
      delete ((vector<DummyPMTHit*>*)p);
   }
   static void deleteArray_vectorlEDummyPMTHitmUgR(void *p) {
      delete [] ((vector<DummyPMTHit*>*)p);
   }
   static void destruct_vectorlEDummyPMTHitmUgR(void *p) {
      typedef vector<DummyPMTHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DummyPMTHit*>

namespace {
  void TriggerDictionaryInitialization_DummyEventDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyEvent.h",
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
#line 1 "DummyEventDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyEvent.h")))  DummyEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DummyEventDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/RootIO/RootIOTest/Event/DummyEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"DummyEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DummyEventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DummyEventDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DummyEventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DummyEventDict() {
  TriggerDictionaryInitialization_DummyEventDict_Impl();
}

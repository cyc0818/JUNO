// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldISimEventdIsrcdISimTTHitDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTTHit.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLSimTTHit(void *p = nullptr);
   static void *newArray_JMcLcLSimTTHit(Long_t size, void *p);
   static void delete_JMcLcLSimTTHit(void *p);
   static void deleteArray_JMcLcLSimTTHit(void *p);
   static void destruct_JMcLcLSimTTHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::SimTTHit*)
   {
      ::JM::SimTTHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::SimTTHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::SimTTHit", ::JM::SimTTHit::Class_Version(), "Event/SimTTHit.h", 9,
                  typeid(::JM::SimTTHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::SimTTHit::Dictionary, isa_proxy, 4,
                  sizeof(::JM::SimTTHit) );
      instance.SetNew(&new_JMcLcLSimTTHit);
      instance.SetNewArray(&newArray_JMcLcLSimTTHit);
      instance.SetDelete(&delete_JMcLcLSimTTHit);
      instance.SetDeleteArray(&deleteArray_JMcLcLSimTTHit);
      instance.SetDestructor(&destruct_JMcLcLSimTTHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::SimTTHit*)
   {
      return GenerateInitInstanceLocal((::JM::SimTTHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::SimTTHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr SimTTHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SimTTHit::Class_Name()
{
   return "JM::SimTTHit";
}

//______________________________________________________________________________
const char *SimTTHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTTHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SimTTHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTTHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimTTHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTTHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimTTHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTTHit*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void SimTTHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::SimTTHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::SimTTHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::SimTTHit::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLSimTTHit(void *p) {
      return  p ? new(p) ::JM::SimTTHit : new ::JM::SimTTHit;
   }
   static void *newArray_JMcLcLSimTTHit(Long_t nElements, void *p) {
      return p ? new(p) ::JM::SimTTHit[nElements] : new ::JM::SimTTHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLSimTTHit(void *p) {
      delete ((::JM::SimTTHit*)p);
   }
   static void deleteArray_JMcLcLSimTTHit(void *p) {
      delete [] ((::JM::SimTTHit*)p);
   }
   static void destruct_JMcLcLSimTTHit(void *p) {
      typedef ::JM::SimTTHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::SimTTHit

namespace ROOT {
   static TClass *vectorlEJMcLcLSimTTHitgR_Dictionary();
   static void vectorlEJMcLcLSimTTHitgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSimTTHitgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSimTTHitgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSimTTHitgR(void *p);
   static void deleteArray_vectorlEJMcLcLSimTTHitgR(void *p);
   static void destruct_vectorlEJMcLcLSimTTHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SimTTHit>*)
   {
      vector<JM::SimTTHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SimTTHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SimTTHit>", -2, "vector", 389,
                  typeid(vector<JM::SimTTHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSimTTHitgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<JM::SimTTHit>) );
      instance.SetNew(&new_vectorlEJMcLcLSimTTHitgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSimTTHitgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSimTTHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSimTTHitgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSimTTHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SimTTHit> >()));

      ::ROOT::AddClassAlternate("vector<JM::SimTTHit>","std::vector<JM::SimTTHit, std::allocator<JM::SimTTHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SimTTHit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSimTTHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SimTTHit>*)nullptr)->GetClass();
      vectorlEJMcLcLSimTTHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSimTTHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSimTTHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTTHit> : new vector<JM::SimTTHit>;
   }
   static void *newArray_vectorlEJMcLcLSimTTHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTTHit>[nElements] : new vector<JM::SimTTHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSimTTHitgR(void *p) {
      delete ((vector<JM::SimTTHit>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSimTTHitgR(void *p) {
      delete [] ((vector<JM::SimTTHit>*)p);
   }
   static void destruct_vectorlEJMcLcLSimTTHitgR(void *p) {
      typedef vector<JM::SimTTHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SimTTHit>

namespace {
  void TriggerDictionaryInitialization_SimTTHitDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTTHit.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SimTTHitDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTTHit.h")))  SimTTHit;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SimTTHitDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTTHit.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::SimTTHit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SimTTHitDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SimTTHitDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SimTTHitDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SimTTHitDict() {
  TriggerDictionaryInitialization_SimTTHitDict_Impl();
}

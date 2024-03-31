// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecTruthdIsrcdITtElecTruthEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth/Event/TtElecTruthEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLTtElecTruthEvt(void *p = nullptr);
   static void *newArray_JMcLcLTtElecTruthEvt(Long_t size, void *p);
   static void delete_JMcLcLTtElecTruthEvt(void *p);
   static void deleteArray_JMcLcLTtElecTruthEvt(void *p);
   static void destruct_JMcLcLTtElecTruthEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::TtElecTruthEvt*)
   {
      ::JM::TtElecTruthEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::TtElecTruthEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::TtElecTruthEvt", ::JM::TtElecTruthEvt::Class_Version(), "Event/TtElecTruthEvt.h", 39,
                  typeid(::JM::TtElecTruthEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::TtElecTruthEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::TtElecTruthEvt) );
      instance.SetNew(&new_JMcLcLTtElecTruthEvt);
      instance.SetNewArray(&newArray_JMcLcLTtElecTruthEvt);
      instance.SetDelete(&delete_JMcLcLTtElecTruthEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLTtElecTruthEvt);
      instance.SetDestructor(&destruct_JMcLcLTtElecTruthEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::TtElecTruthEvt*)
   {
      return GenerateInitInstanceLocal((::JM::TtElecTruthEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::TtElecTruthEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr TtElecTruthEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TtElecTruthEvt::Class_Name()
{
   return "JM::TtElecTruthEvt";
}

//______________________________________________________________________________
const char *TtElecTruthEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruthEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TtElecTruthEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruthEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TtElecTruthEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruthEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TtElecTruthEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtElecTruthEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void TtElecTruthEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::TtElecTruthEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::TtElecTruthEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::TtElecTruthEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLTtElecTruthEvt(void *p) {
      return  p ? new(p) ::JM::TtElecTruthEvt : new ::JM::TtElecTruthEvt;
   }
   static void *newArray_JMcLcLTtElecTruthEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::TtElecTruthEvt[nElements] : new ::JM::TtElecTruthEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLTtElecTruthEvt(void *p) {
      delete ((::JM::TtElecTruthEvt*)p);
   }
   static void deleteArray_JMcLcLTtElecTruthEvt(void *p) {
      delete [] ((::JM::TtElecTruthEvt*)p);
   }
   static void destruct_JMcLcLTtElecTruthEvt(void *p) {
      typedef ::JM::TtElecTruthEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::TtElecTruthEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLTtElecTruthgR_Dictionary();
   static void vectorlEJMcLcLTtElecTruthgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLTtElecTruthgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLTtElecTruthgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLTtElecTruthgR(void *p);
   static void deleteArray_vectorlEJMcLcLTtElecTruthgR(void *p);
   static void destruct_vectorlEJMcLcLTtElecTruthgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::TtElecTruth>*)
   {
      vector<JM::TtElecTruth> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::TtElecTruth>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::TtElecTruth>", -2, "vector", 389,
                  typeid(vector<JM::TtElecTruth>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLTtElecTruthgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::TtElecTruth>) );
      instance.SetNew(&new_vectorlEJMcLcLTtElecTruthgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLTtElecTruthgR);
      instance.SetDelete(&delete_vectorlEJMcLcLTtElecTruthgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLTtElecTruthgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLTtElecTruthgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::TtElecTruth> >()));

      ::ROOT::AddClassAlternate("vector<JM::TtElecTruth>","std::vector<JM::TtElecTruth, std::allocator<JM::TtElecTruth> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::TtElecTruth>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLTtElecTruthgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::TtElecTruth>*)nullptr)->GetClass();
      vectorlEJMcLcLTtElecTruthgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLTtElecTruthgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLTtElecTruthgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::TtElecTruth> : new vector<JM::TtElecTruth>;
   }
   static void *newArray_vectorlEJMcLcLTtElecTruthgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::TtElecTruth>[nElements] : new vector<JM::TtElecTruth>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLTtElecTruthgR(void *p) {
      delete ((vector<JM::TtElecTruth>*)p);
   }
   static void deleteArray_vectorlEJMcLcLTtElecTruthgR(void *p) {
      delete [] ((vector<JM::TtElecTruth>*)p);
   }
   static void destruct_vectorlEJMcLcLTtElecTruthgR(void *p) {
      typedef vector<JM::TtElecTruth> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::TtElecTruth>

namespace {
  void TriggerDictionaryInitialization_TtElecTruthEvtDict_Impl() {
    static const char* headers[] = {
"Event/TtElecTruthEvt.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecTruth",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecTruth/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TtElecTruthEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/TtElecTruthEvt.h")))  TtElecTruthEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TtElecTruthEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/TtElecTruthEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::TtElecTruthEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TtElecTruthEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TtElecTruthEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TtElecTruthEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TtElecTruthEvtDict() {
  TriggerDictionaryInitialization_TtElecTruthEvtDict_Impl();
}

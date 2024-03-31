// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecTruthdIsrcdICdSpmtElecTruthEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth/Event/CdSpmtElecTruthEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdSpmtElecTruthEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdSpmtElecTruthEvt(Long_t size, void *p);
   static void delete_JMcLcLCdSpmtElecTruthEvt(void *p);
   static void deleteArray_JMcLcLCdSpmtElecTruthEvt(void *p);
   static void destruct_JMcLcLCdSpmtElecTruthEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdSpmtElecTruthEvt*)
   {
      ::JM::CdSpmtElecTruthEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdSpmtElecTruthEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdSpmtElecTruthEvt", ::JM::CdSpmtElecTruthEvt::Class_Version(), "Event/CdSpmtElecTruthEvt.h", 39,
                  typeid(::JM::CdSpmtElecTruthEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdSpmtElecTruthEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdSpmtElecTruthEvt) );
      instance.SetNew(&new_JMcLcLCdSpmtElecTruthEvt);
      instance.SetNewArray(&newArray_JMcLcLCdSpmtElecTruthEvt);
      instance.SetDelete(&delete_JMcLcLCdSpmtElecTruthEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdSpmtElecTruthEvt);
      instance.SetDestructor(&destruct_JMcLcLCdSpmtElecTruthEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdSpmtElecTruthEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdSpmtElecTruthEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdSpmtElecTruthEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdSpmtElecTruthEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdSpmtElecTruthEvt::Class_Name()
{
   return "JM::CdSpmtElecTruthEvt";
}

//______________________________________________________________________________
const char *CdSpmtElecTruthEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecTruthEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdSpmtElecTruthEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecTruthEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdSpmtElecTruthEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecTruthEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdSpmtElecTruthEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecTruthEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdSpmtElecTruthEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdSpmtElecTruthEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdSpmtElecTruthEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdSpmtElecTruthEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdSpmtElecTruthEvt(void *p) {
      return  p ? new(p) ::JM::CdSpmtElecTruthEvt : new ::JM::CdSpmtElecTruthEvt;
   }
   static void *newArray_JMcLcLCdSpmtElecTruthEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdSpmtElecTruthEvt[nElements] : new ::JM::CdSpmtElecTruthEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdSpmtElecTruthEvt(void *p) {
      delete ((::JM::CdSpmtElecTruthEvt*)p);
   }
   static void deleteArray_JMcLcLCdSpmtElecTruthEvt(void *p) {
      delete [] ((::JM::CdSpmtElecTruthEvt*)p);
   }
   static void destruct_JMcLcLCdSpmtElecTruthEvt(void *p) {
      typedef ::JM::CdSpmtElecTruthEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdSpmtElecTruthEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLSpmtElecTruthgR_Dictionary();
   static void vectorlEJMcLcLSpmtElecTruthgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSpmtElecTruthgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSpmtElecTruthgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSpmtElecTruthgR(void *p);
   static void deleteArray_vectorlEJMcLcLSpmtElecTruthgR(void *p);
   static void destruct_vectorlEJMcLcLSpmtElecTruthgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SpmtElecTruth>*)
   {
      vector<JM::SpmtElecTruth> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SpmtElecTruth>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SpmtElecTruth>", -2, "vector", 389,
                  typeid(vector<JM::SpmtElecTruth>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSpmtElecTruthgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SpmtElecTruth>) );
      instance.SetNew(&new_vectorlEJMcLcLSpmtElecTruthgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSpmtElecTruthgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSpmtElecTruthgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSpmtElecTruthgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSpmtElecTruthgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SpmtElecTruth> >()));

      ::ROOT::AddClassAlternate("vector<JM::SpmtElecTruth>","std::vector<JM::SpmtElecTruth, std::allocator<JM::SpmtElecTruth> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SpmtElecTruth>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSpmtElecTruthgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SpmtElecTruth>*)nullptr)->GetClass();
      vectorlEJMcLcLSpmtElecTruthgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSpmtElecTruthgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSpmtElecTruthgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecTruth> : new vector<JM::SpmtElecTruth>;
   }
   static void *newArray_vectorlEJMcLcLSpmtElecTruthgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecTruth>[nElements] : new vector<JM::SpmtElecTruth>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSpmtElecTruthgR(void *p) {
      delete ((vector<JM::SpmtElecTruth>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSpmtElecTruthgR(void *p) {
      delete [] ((vector<JM::SpmtElecTruth>*)p);
   }
   static void destruct_vectorlEJMcLcLSpmtElecTruthgR(void *p) {
      typedef vector<JM::SpmtElecTruth> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SpmtElecTruth>

namespace {
  void TriggerDictionaryInitialization_CdSpmtElecTruthEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdSpmtElecTruthEvt.h",
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
#line 1 "CdSpmtElecTruthEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdSpmtElecTruthEvt.h")))  CdSpmtElecTruthEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdSpmtElecTruthEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdSpmtElecTruthEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdSpmtElecTruthEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdSpmtElecTruthEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdSpmtElecTruthEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdSpmtElecTruthEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdSpmtElecTruthEvtDict() {
  TriggerDictionaryInitialization_CdSpmtElecTruthEvtDict_Impl();
}

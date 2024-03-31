// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecTruthdIsrcdICdLpmtElecTruthEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth/Event/CdLpmtElecTruthEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdLpmtElecTruthEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdLpmtElecTruthEvt(Long_t size, void *p);
   static void delete_JMcLcLCdLpmtElecTruthEvt(void *p);
   static void deleteArray_JMcLcLCdLpmtElecTruthEvt(void *p);
   static void destruct_JMcLcLCdLpmtElecTruthEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdLpmtElecTruthEvt*)
   {
      ::JM::CdLpmtElecTruthEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdLpmtElecTruthEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdLpmtElecTruthEvt", ::JM::CdLpmtElecTruthEvt::Class_Version(), "Event/CdLpmtElecTruthEvt.h", 39,
                  typeid(::JM::CdLpmtElecTruthEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdLpmtElecTruthEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdLpmtElecTruthEvt) );
      instance.SetNew(&new_JMcLcLCdLpmtElecTruthEvt);
      instance.SetNewArray(&newArray_JMcLcLCdLpmtElecTruthEvt);
      instance.SetDelete(&delete_JMcLcLCdLpmtElecTruthEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdLpmtElecTruthEvt);
      instance.SetDestructor(&destruct_JMcLcLCdLpmtElecTruthEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdLpmtElecTruthEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdLpmtElecTruthEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdLpmtElecTruthEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdLpmtElecTruthEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdLpmtElecTruthEvt::Class_Name()
{
   return "JM::CdLpmtElecTruthEvt";
}

//______________________________________________________________________________
const char *CdLpmtElecTruthEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecTruthEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdLpmtElecTruthEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecTruthEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdLpmtElecTruthEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecTruthEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdLpmtElecTruthEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecTruthEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdLpmtElecTruthEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdLpmtElecTruthEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdLpmtElecTruthEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdLpmtElecTruthEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdLpmtElecTruthEvt(void *p) {
      return  p ? new(p) ::JM::CdLpmtElecTruthEvt : new ::JM::CdLpmtElecTruthEvt;
   }
   static void *newArray_JMcLcLCdLpmtElecTruthEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdLpmtElecTruthEvt[nElements] : new ::JM::CdLpmtElecTruthEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdLpmtElecTruthEvt(void *p) {
      delete ((::JM::CdLpmtElecTruthEvt*)p);
   }
   static void deleteArray_JMcLcLCdLpmtElecTruthEvt(void *p) {
      delete [] ((::JM::CdLpmtElecTruthEvt*)p);
   }
   static void destruct_JMcLcLCdLpmtElecTruthEvt(void *p) {
      typedef ::JM::CdLpmtElecTruthEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdLpmtElecTruthEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLLpmtElecTruthgR_Dictionary();
   static void vectorlEJMcLcLLpmtElecTruthgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLLpmtElecTruthgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLLpmtElecTruthgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLLpmtElecTruthgR(void *p);
   static void deleteArray_vectorlEJMcLcLLpmtElecTruthgR(void *p);
   static void destruct_vectorlEJMcLcLLpmtElecTruthgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::LpmtElecTruth>*)
   {
      vector<JM::LpmtElecTruth> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::LpmtElecTruth>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::LpmtElecTruth>", -2, "vector", 389,
                  typeid(vector<JM::LpmtElecTruth>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLLpmtElecTruthgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::LpmtElecTruth>) );
      instance.SetNew(&new_vectorlEJMcLcLLpmtElecTruthgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLLpmtElecTruthgR);
      instance.SetDelete(&delete_vectorlEJMcLcLLpmtElecTruthgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLLpmtElecTruthgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLLpmtElecTruthgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::LpmtElecTruth> >()));

      ::ROOT::AddClassAlternate("vector<JM::LpmtElecTruth>","std::vector<JM::LpmtElecTruth, std::allocator<JM::LpmtElecTruth> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::LpmtElecTruth>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLLpmtElecTruthgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::LpmtElecTruth>*)nullptr)->GetClass();
      vectorlEJMcLcLLpmtElecTruthgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLLpmtElecTruthgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLLpmtElecTruthgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::LpmtElecTruth> : new vector<JM::LpmtElecTruth>;
   }
   static void *newArray_vectorlEJMcLcLLpmtElecTruthgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::LpmtElecTruth>[nElements] : new vector<JM::LpmtElecTruth>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLLpmtElecTruthgR(void *p) {
      delete ((vector<JM::LpmtElecTruth>*)p);
   }
   static void deleteArray_vectorlEJMcLcLLpmtElecTruthgR(void *p) {
      delete [] ((vector<JM::LpmtElecTruth>*)p);
   }
   static void destruct_vectorlEJMcLcLLpmtElecTruthgR(void *p) {
      typedef vector<JM::LpmtElecTruth> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::LpmtElecTruth>

namespace {
  void TriggerDictionaryInitialization_CdLpmtElecTruthEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdLpmtElecTruthEvt.h",
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
#line 1 "CdLpmtElecTruthEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdLpmtElecTruthEvt.h")))  CdLpmtElecTruthEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdLpmtElecTruthEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdLpmtElecTruthEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdLpmtElecTruthEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdLpmtElecTruthEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdLpmtElecTruthEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdLpmtElecTruthEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdLpmtElecTruthEvtDict() {
  TriggerDictionaryInitialization_CdLpmtElecTruthEvtDict_Impl();
}

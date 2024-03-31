// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecTruthdIsrcdIWpElecTruthEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecTruth/Event/WpElecTruthEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLWpElecTruthEvt(void *p = nullptr);
   static void *newArray_JMcLcLWpElecTruthEvt(Long_t size, void *p);
   static void delete_JMcLcLWpElecTruthEvt(void *p);
   static void deleteArray_JMcLcLWpElecTruthEvt(void *p);
   static void destruct_JMcLcLWpElecTruthEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::WpElecTruthEvt*)
   {
      ::JM::WpElecTruthEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::WpElecTruthEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::WpElecTruthEvt", ::JM::WpElecTruthEvt::Class_Version(), "Event/WpElecTruthEvt.h", 39,
                  typeid(::JM::WpElecTruthEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::WpElecTruthEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::WpElecTruthEvt) );
      instance.SetNew(&new_JMcLcLWpElecTruthEvt);
      instance.SetNewArray(&newArray_JMcLcLWpElecTruthEvt);
      instance.SetDelete(&delete_JMcLcLWpElecTruthEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLWpElecTruthEvt);
      instance.SetDestructor(&destruct_JMcLcLWpElecTruthEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::WpElecTruthEvt*)
   {
      return GenerateInitInstanceLocal((::JM::WpElecTruthEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::WpElecTruthEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr WpElecTruthEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WpElecTruthEvt::Class_Name()
{
   return "JM::WpElecTruthEvt";
}

//______________________________________________________________________________
const char *WpElecTruthEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::WpElecTruthEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WpElecTruthEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::WpElecTruthEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WpElecTruthEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::WpElecTruthEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WpElecTruthEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::WpElecTruthEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void WpElecTruthEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::WpElecTruthEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::WpElecTruthEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::WpElecTruthEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLWpElecTruthEvt(void *p) {
      return  p ? new(p) ::JM::WpElecTruthEvt : new ::JM::WpElecTruthEvt;
   }
   static void *newArray_JMcLcLWpElecTruthEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::WpElecTruthEvt[nElements] : new ::JM::WpElecTruthEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLWpElecTruthEvt(void *p) {
      delete ((::JM::WpElecTruthEvt*)p);
   }
   static void deleteArray_JMcLcLWpElecTruthEvt(void *p) {
      delete [] ((::JM::WpElecTruthEvt*)p);
   }
   static void destruct_JMcLcLWpElecTruthEvt(void *p) {
      typedef ::JM::WpElecTruthEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::WpElecTruthEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLWpElecTruthgR_Dictionary();
   static void vectorlEJMcLcLWpElecTruthgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLWpElecTruthgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLWpElecTruthgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLWpElecTruthgR(void *p);
   static void deleteArray_vectorlEJMcLcLWpElecTruthgR(void *p);
   static void destruct_vectorlEJMcLcLWpElecTruthgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::WpElecTruth>*)
   {
      vector<JM::WpElecTruth> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::WpElecTruth>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::WpElecTruth>", -2, "vector", 389,
                  typeid(vector<JM::WpElecTruth>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLWpElecTruthgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::WpElecTruth>) );
      instance.SetNew(&new_vectorlEJMcLcLWpElecTruthgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLWpElecTruthgR);
      instance.SetDelete(&delete_vectorlEJMcLcLWpElecTruthgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLWpElecTruthgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLWpElecTruthgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::WpElecTruth> >()));

      ::ROOT::AddClassAlternate("vector<JM::WpElecTruth>","std::vector<JM::WpElecTruth, std::allocator<JM::WpElecTruth> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::WpElecTruth>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLWpElecTruthgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::WpElecTruth>*)nullptr)->GetClass();
      vectorlEJMcLcLWpElecTruthgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLWpElecTruthgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLWpElecTruthgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::WpElecTruth> : new vector<JM::WpElecTruth>;
   }
   static void *newArray_vectorlEJMcLcLWpElecTruthgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::WpElecTruth>[nElements] : new vector<JM::WpElecTruth>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLWpElecTruthgR(void *p) {
      delete ((vector<JM::WpElecTruth>*)p);
   }
   static void deleteArray_vectorlEJMcLcLWpElecTruthgR(void *p) {
      delete [] ((vector<JM::WpElecTruth>*)p);
   }
   static void destruct_vectorlEJMcLcLWpElecTruthgR(void *p) {
      typedef vector<JM::WpElecTruth> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::WpElecTruth>

namespace {
  void TriggerDictionaryInitialization_WpElecTruthEvtDict_Impl() {
    static const char* headers[] = {
"Event/WpElecTruthEvt.h",
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
#line 1 "WpElecTruthEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/WpElecTruthEvt.h")))  WpElecTruthEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "WpElecTruthEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/WpElecTruthEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::WpElecTruthEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("WpElecTruthEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_WpElecTruthEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_WpElecTruthEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_WpElecTruthEvtDict() {
  TriggerDictionaryInitialization_WpElecTruthEvtDict_Impl();
}

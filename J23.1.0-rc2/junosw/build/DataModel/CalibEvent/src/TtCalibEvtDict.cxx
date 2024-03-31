// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldICalibEventdIsrcdITtCalibEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/TtCalibEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLTtCalibEvt(void *p = nullptr);
   static void *newArray_JMcLcLTtCalibEvt(Long_t size, void *p);
   static void delete_JMcLcLTtCalibEvt(void *p);
   static void deleteArray_JMcLcLTtCalibEvt(void *p);
   static void destruct_JMcLcLTtCalibEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::TtCalibEvt*)
   {
      ::JM::TtCalibEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::TtCalibEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::TtCalibEvt", ::JM::TtCalibEvt::Class_Version(), "Event/TtCalibEvt.h", 39,
                  typeid(::JM::TtCalibEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::TtCalibEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::TtCalibEvt) );
      instance.SetNew(&new_JMcLcLTtCalibEvt);
      instance.SetNewArray(&newArray_JMcLcLTtCalibEvt);
      instance.SetDelete(&delete_JMcLcLTtCalibEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLTtCalibEvt);
      instance.SetDestructor(&destruct_JMcLcLTtCalibEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::TtCalibEvt*)
   {
      return GenerateInitInstanceLocal((::JM::TtCalibEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::TtCalibEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr TtCalibEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TtCalibEvt::Class_Name()
{
   return "JM::TtCalibEvt";
}

//______________________________________________________________________________
const char *TtCalibEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtCalibEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TtCalibEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::TtCalibEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TtCalibEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtCalibEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TtCalibEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::TtCalibEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void TtCalibEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::TtCalibEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::TtCalibEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::TtCalibEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLTtCalibEvt(void *p) {
      return  p ? new(p) ::JM::TtCalibEvt : new ::JM::TtCalibEvt;
   }
   static void *newArray_JMcLcLTtCalibEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::TtCalibEvt[nElements] : new ::JM::TtCalibEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLTtCalibEvt(void *p) {
      delete ((::JM::TtCalibEvt*)p);
   }
   static void deleteArray_JMcLcLTtCalibEvt(void *p) {
      delete [] ((::JM::TtCalibEvt*)p);
   }
   static void destruct_JMcLcLTtCalibEvt(void *p) {
      typedef ::JM::TtCalibEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::TtCalibEvt

namespace ROOT {
   static TClass *listlEJMcLcLCalibTtChannelmUgR_Dictionary();
   static void listlEJMcLcLCalibTtChannelmUgR_TClassManip(TClass*);
   static void *new_listlEJMcLcLCalibTtChannelmUgR(void *p = nullptr);
   static void *newArray_listlEJMcLcLCalibTtChannelmUgR(Long_t size, void *p);
   static void delete_listlEJMcLcLCalibTtChannelmUgR(void *p);
   static void deleteArray_listlEJMcLcLCalibTtChannelmUgR(void *p);
   static void destruct_listlEJMcLcLCalibTtChannelmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<JM::CalibTtChannel*>*)
   {
      list<JM::CalibTtChannel*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<JM::CalibTtChannel*>));
      static ::ROOT::TGenericClassInfo 
         instance("list<JM::CalibTtChannel*>", -2, "list", 556,
                  typeid(list<JM::CalibTtChannel*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEJMcLcLCalibTtChannelmUgR_Dictionary, isa_proxy, 0,
                  sizeof(list<JM::CalibTtChannel*>) );
      instance.SetNew(&new_listlEJMcLcLCalibTtChannelmUgR);
      instance.SetNewArray(&newArray_listlEJMcLcLCalibTtChannelmUgR);
      instance.SetDelete(&delete_listlEJMcLcLCalibTtChannelmUgR);
      instance.SetDeleteArray(&deleteArray_listlEJMcLcLCalibTtChannelmUgR);
      instance.SetDestructor(&destruct_listlEJMcLcLCalibTtChannelmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<JM::CalibTtChannel*> >()));

      ::ROOT::AddClassAlternate("list<JM::CalibTtChannel*>","std::__cxx11::list<JM::CalibTtChannel*, std::allocator<JM::CalibTtChannel*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<JM::CalibTtChannel*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEJMcLcLCalibTtChannelmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<JM::CalibTtChannel*>*)nullptr)->GetClass();
      listlEJMcLcLCalibTtChannelmUgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEJMcLcLCalibTtChannelmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEJMcLcLCalibTtChannelmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<JM::CalibTtChannel*> : new list<JM::CalibTtChannel*>;
   }
   static void *newArray_listlEJMcLcLCalibTtChannelmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<JM::CalibTtChannel*>[nElements] : new list<JM::CalibTtChannel*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEJMcLcLCalibTtChannelmUgR(void *p) {
      delete ((list<JM::CalibTtChannel*>*)p);
   }
   static void deleteArray_listlEJMcLcLCalibTtChannelmUgR(void *p) {
      delete [] ((list<JM::CalibTtChannel*>*)p);
   }
   static void destruct_listlEJMcLcLCalibTtChannelmUgR(void *p) {
      typedef list<JM::CalibTtChannel*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<JM::CalibTtChannel*>

namespace {
  void TriggerDictionaryInitialization_TtCalibEvtDict_Impl() {
    static const char* headers[] = {
"Event/TtCalibEvt.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/CalibEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/CalibEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TtCalibEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/TtCalibEvt.h")))  TtCalibEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TtCalibEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/TtCalibEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::TtCalibEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TtCalibEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TtCalibEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TtCalibEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TtCalibEvtDict() {
  TriggerDictionaryInitialization_TtCalibEvtDict_Impl();
}

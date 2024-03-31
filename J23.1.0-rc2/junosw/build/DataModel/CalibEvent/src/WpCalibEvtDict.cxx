// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldICalibEventdIsrcdIWpCalibEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/WpCalibEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLWpCalibEvt(void *p = nullptr);
   static void *newArray_JMcLcLWpCalibEvt(Long_t size, void *p);
   static void delete_JMcLcLWpCalibEvt(void *p);
   static void deleteArray_JMcLcLWpCalibEvt(void *p);
   static void destruct_JMcLcLWpCalibEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::WpCalibEvt*)
   {
      ::JM::WpCalibEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::WpCalibEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::WpCalibEvt", ::JM::WpCalibEvt::Class_Version(), "Event/WpCalibEvt.h", 39,
                  typeid(::JM::WpCalibEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::WpCalibEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::WpCalibEvt) );
      instance.SetNew(&new_JMcLcLWpCalibEvt);
      instance.SetNewArray(&newArray_JMcLcLWpCalibEvt);
      instance.SetDelete(&delete_JMcLcLWpCalibEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLWpCalibEvt);
      instance.SetDestructor(&destruct_JMcLcLWpCalibEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::WpCalibEvt*)
   {
      return GenerateInitInstanceLocal((::JM::WpCalibEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::WpCalibEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr WpCalibEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WpCalibEvt::Class_Name()
{
   return "JM::WpCalibEvt";
}

//______________________________________________________________________________
const char *WpCalibEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::WpCalibEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WpCalibEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::WpCalibEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WpCalibEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::WpCalibEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WpCalibEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::WpCalibEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void WpCalibEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::WpCalibEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::WpCalibEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::WpCalibEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLWpCalibEvt(void *p) {
      return  p ? new(p) ::JM::WpCalibEvt : new ::JM::WpCalibEvt;
   }
   static void *newArray_JMcLcLWpCalibEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::WpCalibEvt[nElements] : new ::JM::WpCalibEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLWpCalibEvt(void *p) {
      delete ((::JM::WpCalibEvt*)p);
   }
   static void deleteArray_JMcLcLWpCalibEvt(void *p) {
      delete [] ((::JM::WpCalibEvt*)p);
   }
   static void destruct_JMcLcLWpCalibEvt(void *p) {
      typedef ::JM::WpCalibEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::WpCalibEvt

namespace ROOT {
   static TClass *listlEJMcLcLCalibPmtChannelmUgR_Dictionary();
   static void listlEJMcLcLCalibPmtChannelmUgR_TClassManip(TClass*);
   static void *new_listlEJMcLcLCalibPmtChannelmUgR(void *p = nullptr);
   static void *newArray_listlEJMcLcLCalibPmtChannelmUgR(Long_t size, void *p);
   static void delete_listlEJMcLcLCalibPmtChannelmUgR(void *p);
   static void deleteArray_listlEJMcLcLCalibPmtChannelmUgR(void *p);
   static void destruct_listlEJMcLcLCalibPmtChannelmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<JM::CalibPmtChannel*>*)
   {
      list<JM::CalibPmtChannel*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<JM::CalibPmtChannel*>));
      static ::ROOT::TGenericClassInfo 
         instance("list<JM::CalibPmtChannel*>", -2, "list", 556,
                  typeid(list<JM::CalibPmtChannel*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEJMcLcLCalibPmtChannelmUgR_Dictionary, isa_proxy, 0,
                  sizeof(list<JM::CalibPmtChannel*>) );
      instance.SetNew(&new_listlEJMcLcLCalibPmtChannelmUgR);
      instance.SetNewArray(&newArray_listlEJMcLcLCalibPmtChannelmUgR);
      instance.SetDelete(&delete_listlEJMcLcLCalibPmtChannelmUgR);
      instance.SetDeleteArray(&deleteArray_listlEJMcLcLCalibPmtChannelmUgR);
      instance.SetDestructor(&destruct_listlEJMcLcLCalibPmtChannelmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<JM::CalibPmtChannel*> >()));

      ::ROOT::AddClassAlternate("list<JM::CalibPmtChannel*>","std::__cxx11::list<JM::CalibPmtChannel*, std::allocator<JM::CalibPmtChannel*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<JM::CalibPmtChannel*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEJMcLcLCalibPmtChannelmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<JM::CalibPmtChannel*>*)nullptr)->GetClass();
      listlEJMcLcLCalibPmtChannelmUgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEJMcLcLCalibPmtChannelmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEJMcLcLCalibPmtChannelmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<JM::CalibPmtChannel*> : new list<JM::CalibPmtChannel*>;
   }
   static void *newArray_listlEJMcLcLCalibPmtChannelmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<JM::CalibPmtChannel*>[nElements] : new list<JM::CalibPmtChannel*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEJMcLcLCalibPmtChannelmUgR(void *p) {
      delete ((list<JM::CalibPmtChannel*>*)p);
   }
   static void deleteArray_listlEJMcLcLCalibPmtChannelmUgR(void *p) {
      delete [] ((list<JM::CalibPmtChannel*>*)p);
   }
   static void destruct_listlEJMcLcLCalibPmtChannelmUgR(void *p) {
      typedef list<JM::CalibPmtChannel*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<JM::CalibPmtChannel*>

namespace {
  void TriggerDictionaryInitialization_WpCalibEvtDict_Impl() {
    static const char* headers[] = {
"Event/WpCalibEvt.h",
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
#line 1 "WpCalibEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/WpCalibEvt.h")))  WpCalibEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "WpCalibEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/WpCalibEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::WpCalibEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("WpCalibEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_WpCalibEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_WpCalibEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_WpCalibEvtDict() {
  TriggerDictionaryInitialization_WpCalibEvtDict_Impl();
}

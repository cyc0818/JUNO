// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldICalibEventdIsrcdICdLpmtCalibEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/CdLpmtCalibEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdLpmtCalibEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdLpmtCalibEvt(Long_t size, void *p);
   static void delete_JMcLcLCdLpmtCalibEvt(void *p);
   static void deleteArray_JMcLcLCdLpmtCalibEvt(void *p);
   static void destruct_JMcLcLCdLpmtCalibEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdLpmtCalibEvt*)
   {
      ::JM::CdLpmtCalibEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdLpmtCalibEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdLpmtCalibEvt", ::JM::CdLpmtCalibEvt::Class_Version(), "Event/CdLpmtCalibEvt.h", 39,
                  typeid(::JM::CdLpmtCalibEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdLpmtCalibEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdLpmtCalibEvt) );
      instance.SetNew(&new_JMcLcLCdLpmtCalibEvt);
      instance.SetNewArray(&newArray_JMcLcLCdLpmtCalibEvt);
      instance.SetDelete(&delete_JMcLcLCdLpmtCalibEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdLpmtCalibEvt);
      instance.SetDestructor(&destruct_JMcLcLCdLpmtCalibEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdLpmtCalibEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdLpmtCalibEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdLpmtCalibEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdLpmtCalibEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdLpmtCalibEvt::Class_Name()
{
   return "JM::CdLpmtCalibEvt";
}

//______________________________________________________________________________
const char *CdLpmtCalibEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtCalibEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdLpmtCalibEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtCalibEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdLpmtCalibEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtCalibEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdLpmtCalibEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtCalibEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdLpmtCalibEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdLpmtCalibEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdLpmtCalibEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdLpmtCalibEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdLpmtCalibEvt(void *p) {
      return  p ? new(p) ::JM::CdLpmtCalibEvt : new ::JM::CdLpmtCalibEvt;
   }
   static void *newArray_JMcLcLCdLpmtCalibEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdLpmtCalibEvt[nElements] : new ::JM::CdLpmtCalibEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdLpmtCalibEvt(void *p) {
      delete ((::JM::CdLpmtCalibEvt*)p);
   }
   static void deleteArray_JMcLcLCdLpmtCalibEvt(void *p) {
      delete [] ((::JM::CdLpmtCalibEvt*)p);
   }
   static void destruct_JMcLcLCdLpmtCalibEvt(void *p) {
      typedef ::JM::CdLpmtCalibEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdLpmtCalibEvt

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
  void TriggerDictionaryInitialization_CdLpmtCalibEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdLpmtCalibEvt.h",
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
#line 1 "CdLpmtCalibEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdLpmtCalibEvt.h")))  CdLpmtCalibEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdLpmtCalibEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdLpmtCalibEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdLpmtCalibEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdLpmtCalibEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdLpmtCalibEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdLpmtCalibEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdLpmtCalibEvtDict() {
  TriggerDictionaryInitialization_CdLpmtCalibEvtDict_Impl();
}

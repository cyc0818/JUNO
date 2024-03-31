// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIRecEventdIsrcdIWpRecEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent/Event/WpRecEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLWpRecEvt(void *p = nullptr);
   static void *newArray_JMcLcLWpRecEvt(Long_t size, void *p);
   static void delete_JMcLcLWpRecEvt(void *p);
   static void deleteArray_JMcLcLWpRecEvt(void *p);
   static void destruct_JMcLcLWpRecEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::WpRecEvt*)
   {
      ::JM::WpRecEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::WpRecEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::WpRecEvt", ::JM::WpRecEvt::Class_Version(), "Event/WpRecEvt.h", 38,
                  typeid(::JM::WpRecEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::WpRecEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::WpRecEvt) );
      instance.SetNew(&new_JMcLcLWpRecEvt);
      instance.SetNewArray(&newArray_JMcLcLWpRecEvt);
      instance.SetDelete(&delete_JMcLcLWpRecEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLWpRecEvt);
      instance.SetDestructor(&destruct_JMcLcLWpRecEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::WpRecEvt*)
   {
      return GenerateInitInstanceLocal((::JM::WpRecEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::WpRecEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr WpRecEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WpRecEvt::Class_Name()
{
   return "JM::WpRecEvt";
}

//______________________________________________________________________________
const char *WpRecEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::WpRecEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WpRecEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::WpRecEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WpRecEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::WpRecEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WpRecEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::WpRecEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void WpRecEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::WpRecEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::WpRecEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::WpRecEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLWpRecEvt(void *p) {
      return  p ? new(p) ::JM::WpRecEvt : new ::JM::WpRecEvt;
   }
   static void *newArray_JMcLcLWpRecEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::WpRecEvt[nElements] : new ::JM::WpRecEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLWpRecEvt(void *p) {
      delete ((::JM::WpRecEvt*)p);
   }
   static void deleteArray_JMcLcLWpRecEvt(void *p) {
      delete [] ((::JM::WpRecEvt*)p);
   }
   static void destruct_JMcLcLWpRecEvt(void *p) {
      typedef ::JM::WpRecEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::WpRecEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLRecTrackmUgR_Dictionary();
   static void vectorlEJMcLcLRecTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLRecTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLRecTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLRecTrackmUgR(void *p);
   static void deleteArray_vectorlEJMcLcLRecTrackmUgR(void *p);
   static void destruct_vectorlEJMcLcLRecTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::RecTrack*>*)
   {
      vector<JM::RecTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::RecTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::RecTrack*>", -2, "vector", 389,
                  typeid(vector<JM::RecTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLRecTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::RecTrack*>) );
      instance.SetNew(&new_vectorlEJMcLcLRecTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLRecTrackmUgR);
      instance.SetDelete(&delete_vectorlEJMcLcLRecTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLRecTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLRecTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::RecTrack*> >()));

      ::ROOT::AddClassAlternate("vector<JM::RecTrack*>","std::vector<JM::RecTrack*, std::allocator<JM::RecTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::RecTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLRecTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::RecTrack*>*)nullptr)->GetClass();
      vectorlEJMcLcLRecTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLRecTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLRecTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::RecTrack*> : new vector<JM::RecTrack*>;
   }
   static void *newArray_vectorlEJMcLcLRecTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::RecTrack*>[nElements] : new vector<JM::RecTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLRecTrackmUgR(void *p) {
      delete ((vector<JM::RecTrack*>*)p);
   }
   static void deleteArray_vectorlEJMcLcLRecTrackmUgR(void *p) {
      delete [] ((vector<JM::RecTrack*>*)p);
   }
   static void destruct_vectorlEJMcLcLRecTrackmUgR(void *p) {
      typedef vector<JM::RecTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::RecTrack*>

namespace {
  void TriggerDictionaryInitialization_WpRecEvtDict_Impl() {
    static const char* headers[] = {
"Event/WpRecEvt.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/RecEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/RecEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "WpRecEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/WpRecEvt.h")))  WpRecEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "WpRecEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/WpRecEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::WpRecEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("WpRecEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_WpRecEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_WpRecEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_WpRecEvtDict() {
  TriggerDictionaryInitialization_WpRecEvtDict_Impl();
}

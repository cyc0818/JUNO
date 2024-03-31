// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIRecEventdIsrcdICdTrackRecEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent/Event/CdTrackRecEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdTrackRecEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdTrackRecEvt(Long_t size, void *p);
   static void delete_JMcLcLCdTrackRecEvt(void *p);
   static void deleteArray_JMcLcLCdTrackRecEvt(void *p);
   static void destruct_JMcLcLCdTrackRecEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdTrackRecEvt*)
   {
      ::JM::CdTrackRecEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdTrackRecEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdTrackRecEvt", ::JM::CdTrackRecEvt::Class_Version(), "Event/CdTrackRecEvt.h", 38,
                  typeid(::JM::CdTrackRecEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdTrackRecEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdTrackRecEvt) );
      instance.SetNew(&new_JMcLcLCdTrackRecEvt);
      instance.SetNewArray(&newArray_JMcLcLCdTrackRecEvt);
      instance.SetDelete(&delete_JMcLcLCdTrackRecEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdTrackRecEvt);
      instance.SetDestructor(&destruct_JMcLcLCdTrackRecEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdTrackRecEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdTrackRecEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdTrackRecEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdTrackRecEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdTrackRecEvt::Class_Name()
{
   return "JM::CdTrackRecEvt";
}

//______________________________________________________________________________
const char *CdTrackRecEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdTrackRecEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdTrackRecEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdTrackRecEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdTrackRecEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdTrackRecEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdTrackRecEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdTrackRecEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdTrackRecEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdTrackRecEvt(void *p) {
      return  p ? new(p) ::JM::CdTrackRecEvt : new ::JM::CdTrackRecEvt;
   }
   static void *newArray_JMcLcLCdTrackRecEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdTrackRecEvt[nElements] : new ::JM::CdTrackRecEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdTrackRecEvt(void *p) {
      delete ((::JM::CdTrackRecEvt*)p);
   }
   static void deleteArray_JMcLcLCdTrackRecEvt(void *p) {
      delete [] ((::JM::CdTrackRecEvt*)p);
   }
   static void destruct_JMcLcLCdTrackRecEvt(void *p) {
      typedef ::JM::CdTrackRecEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdTrackRecEvt

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
  void TriggerDictionaryInitialization_CdTrackRecEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdTrackRecEvt.h",
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
#line 1 "CdTrackRecEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdTrackRecEvt.h")))  CdTrackRecEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdTrackRecEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdTrackRecEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdTrackRecEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdTrackRecEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdTrackRecEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdTrackRecEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdTrackRecEvtDict() {
  TriggerDictionaryInitialization_CdTrackRecEvtDict_Impl();
}
// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIRecEventdIsrcdICdVertexRecEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/RecEvent/Event/CdVertexRecEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdVertexRecEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdVertexRecEvt(Long_t size, void *p);
   static void delete_JMcLcLCdVertexRecEvt(void *p);
   static void deleteArray_JMcLcLCdVertexRecEvt(void *p);
   static void destruct_JMcLcLCdVertexRecEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdVertexRecEvt*)
   {
      ::JM::CdVertexRecEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdVertexRecEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdVertexRecEvt", ::JM::CdVertexRecEvt::Class_Version(), "Event/CdVertexRecEvt.h", 38,
                  typeid(::JM::CdVertexRecEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdVertexRecEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdVertexRecEvt) );
      instance.SetNew(&new_JMcLcLCdVertexRecEvt);
      instance.SetNewArray(&newArray_JMcLcLCdVertexRecEvt);
      instance.SetDelete(&delete_JMcLcLCdVertexRecEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdVertexRecEvt);
      instance.SetDestructor(&destruct_JMcLcLCdVertexRecEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdVertexRecEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdVertexRecEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdVertexRecEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdVertexRecEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdVertexRecEvt::Class_Name()
{
   return "JM::CdVertexRecEvt";
}

//______________________________________________________________________________
const char *CdVertexRecEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdVertexRecEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdVertexRecEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdVertexRecEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdVertexRecEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdVertexRecEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdVertexRecEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdVertexRecEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdVertexRecEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdVertexRecEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdVertexRecEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdVertexRecEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdVertexRecEvt(void *p) {
      return  p ? new(p) ::JM::CdVertexRecEvt : new ::JM::CdVertexRecEvt;
   }
   static void *newArray_JMcLcLCdVertexRecEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdVertexRecEvt[nElements] : new ::JM::CdVertexRecEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdVertexRecEvt(void *p) {
      delete ((::JM::CdVertexRecEvt*)p);
   }
   static void deleteArray_JMcLcLCdVertexRecEvt(void *p) {
      delete [] ((::JM::CdVertexRecEvt*)p);
   }
   static void destruct_JMcLcLCdVertexRecEvt(void *p) {
      typedef ::JM::CdVertexRecEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdVertexRecEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLRecVertexmUgR_Dictionary();
   static void vectorlEJMcLcLRecVertexmUgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLRecVertexmUgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLRecVertexmUgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLRecVertexmUgR(void *p);
   static void deleteArray_vectorlEJMcLcLRecVertexmUgR(void *p);
   static void destruct_vectorlEJMcLcLRecVertexmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::RecVertex*>*)
   {
      vector<JM::RecVertex*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::RecVertex*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::RecVertex*>", -2, "vector", 389,
                  typeid(vector<JM::RecVertex*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLRecVertexmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::RecVertex*>) );
      instance.SetNew(&new_vectorlEJMcLcLRecVertexmUgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLRecVertexmUgR);
      instance.SetDelete(&delete_vectorlEJMcLcLRecVertexmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLRecVertexmUgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLRecVertexmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::RecVertex*> >()));

      ::ROOT::AddClassAlternate("vector<JM::RecVertex*>","std::vector<JM::RecVertex*, std::allocator<JM::RecVertex*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::RecVertex*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLRecVertexmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::RecVertex*>*)nullptr)->GetClass();
      vectorlEJMcLcLRecVertexmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLRecVertexmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLRecVertexmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::RecVertex*> : new vector<JM::RecVertex*>;
   }
   static void *newArray_vectorlEJMcLcLRecVertexmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::RecVertex*>[nElements] : new vector<JM::RecVertex*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLRecVertexmUgR(void *p) {
      delete ((vector<JM::RecVertex*>*)p);
   }
   static void deleteArray_vectorlEJMcLcLRecVertexmUgR(void *p) {
      delete [] ((vector<JM::RecVertex*>*)p);
   }
   static void destruct_vectorlEJMcLcLRecVertexmUgR(void *p) {
      typedef vector<JM::RecVertex*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::RecVertex*>

namespace {
  void TriggerDictionaryInitialization_CdVertexRecEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdVertexRecEvt.h",
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
#line 1 "CdVertexRecEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdVertexRecEvt.h")))  CdVertexRecEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdVertexRecEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdVertexRecEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdVertexRecEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdVertexRecEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdVertexRecEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdVertexRecEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdVertexRecEvtDict() {
  TriggerDictionaryInitialization_CdVertexRecEvtDict_Impl();
}

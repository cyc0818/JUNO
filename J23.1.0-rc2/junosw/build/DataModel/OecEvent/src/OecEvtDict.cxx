// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIOecEventdIsrcdIOecEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLOecEvt(void *p = nullptr);
   static void *newArray_JMcLcLOecEvt(Long_t size, void *p);
   static void delete_JMcLcLOecEvt(void *p);
   static void deleteArray_JMcLcLOecEvt(void *p);
   static void destruct_JMcLcLOecEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::OecEvt*)
   {
      ::JM::OecEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::OecEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::OecEvt", ::JM::OecEvt::Class_Version(), "Event/OecEvt.h", 11,
                  typeid(::JM::OecEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::OecEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::OecEvt) );
      instance.SetNew(&new_JMcLcLOecEvt);
      instance.SetNewArray(&newArray_JMcLcLOecEvt);
      instance.SetDelete(&delete_JMcLcLOecEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLOecEvt);
      instance.SetDestructor(&destruct_JMcLcLOecEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::OecEvt*)
   {
      return GenerateInitInstanceLocal((::JM::OecEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::OecEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr OecEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *OecEvt::Class_Name()
{
   return "JM::OecEvt";
}

//______________________________________________________________________________
const char *OecEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OecEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int OecEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OecEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *OecEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OecEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *OecEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OecEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void OecEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::OecEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::OecEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::OecEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLOecEvt(void *p) {
      return  p ? new(p) ::JM::OecEvt : new ::JM::OecEvt;
   }
   static void *newArray_JMcLcLOecEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::OecEvt[nElements] : new ::JM::OecEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLOecEvt(void *p) {
      delete ((::JM::OecEvt*)p);
   }
   static void deleteArray_JMcLcLOecEvt(void *p) {
      delete [] ((::JM::OecEvt*)p);
   }
   static void destruct_JMcLcLOecEvt(void *p) {
      typedef ::JM::OecEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::OecEvt

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = nullptr);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 389,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      ::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)nullptr)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = nullptr);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 389,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      ::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)nullptr)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace {
  void TriggerDictionaryInitialization_OecEvtDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecEvt.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/OecEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/OecEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "OecEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecEvt.h")))  OecEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "OecEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/OecEvent/Event/OecEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::OecEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("OecEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_OecEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_OecEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_OecEvtDict() {
  TriggerDictionaryInitialization_OecEvtDict_Impl();
}

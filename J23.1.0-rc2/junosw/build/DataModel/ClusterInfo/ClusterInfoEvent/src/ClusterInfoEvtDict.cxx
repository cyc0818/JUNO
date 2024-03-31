// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIClusterInfodIClusterInfoEventdIsrcdIClusterInfoEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterInfoEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLClusterInfoEvt(void *p = nullptr);
   static void *newArray_JMcLcLClusterInfoEvt(Long_t size, void *p);
   static void delete_JMcLcLClusterInfoEvt(void *p);
   static void deleteArray_JMcLcLClusterInfoEvt(void *p);
   static void destruct_JMcLcLClusterInfoEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::ClusterInfoEvt*)
   {
      ::JM::ClusterInfoEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::ClusterInfoEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::ClusterInfoEvt", ::JM::ClusterInfoEvt::Class_Version(), "Event/ClusterInfoEvt.h", 24,
                  typeid(::JM::ClusterInfoEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::ClusterInfoEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::ClusterInfoEvt) );
      instance.SetNew(&new_JMcLcLClusterInfoEvt);
      instance.SetNewArray(&newArray_JMcLcLClusterInfoEvt);
      instance.SetDelete(&delete_JMcLcLClusterInfoEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLClusterInfoEvt);
      instance.SetDestructor(&destruct_JMcLcLClusterInfoEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::ClusterInfoEvt*)
   {
      return GenerateInitInstanceLocal((::JM::ClusterInfoEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::ClusterInfoEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr ClusterInfoEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ClusterInfoEvt::Class_Name()
{
   return "JM::ClusterInfoEvt";
}

//______________________________________________________________________________
const char *ClusterInfoEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterInfoEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ClusterInfoEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterInfoEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ClusterInfoEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterInfoEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ClusterInfoEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterInfoEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void ClusterInfoEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::ClusterInfoEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::ClusterInfoEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::ClusterInfoEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLClusterInfoEvt(void *p) {
      return  p ? new(p) ::JM::ClusterInfoEvt : new ::JM::ClusterInfoEvt;
   }
   static void *newArray_JMcLcLClusterInfoEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::ClusterInfoEvt[nElements] : new ::JM::ClusterInfoEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLClusterInfoEvt(void *p) {
      delete ((::JM::ClusterInfoEvt*)p);
   }
   static void deleteArray_JMcLcLClusterInfoEvt(void *p) {
      delete [] ((::JM::ClusterInfoEvt*)p);
   }
   static void destruct_JMcLcLClusterInfoEvt(void *p) {
      typedef ::JM::ClusterInfoEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::ClusterInfoEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLClusterHitColmUgR_Dictionary();
   static void vectorlEJMcLcLClusterHitColmUgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLClusterHitColmUgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLClusterHitColmUgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLClusterHitColmUgR(void *p);
   static void deleteArray_vectorlEJMcLcLClusterHitColmUgR(void *p);
   static void destruct_vectorlEJMcLcLClusterHitColmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::ClusterHitCol*>*)
   {
      vector<JM::ClusterHitCol*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::ClusterHitCol*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::ClusterHitCol*>", -2, "vector", 389,
                  typeid(vector<JM::ClusterHitCol*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLClusterHitColmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::ClusterHitCol*>) );
      instance.SetNew(&new_vectorlEJMcLcLClusterHitColmUgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLClusterHitColmUgR);
      instance.SetDelete(&delete_vectorlEJMcLcLClusterHitColmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLClusterHitColmUgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLClusterHitColmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::ClusterHitCol*> >()));

      ::ROOT::AddClassAlternate("vector<JM::ClusterHitCol*>","std::vector<JM::ClusterHitCol*, std::allocator<JM::ClusterHitCol*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::ClusterHitCol*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLClusterHitColmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::ClusterHitCol*>*)nullptr)->GetClass();
      vectorlEJMcLcLClusterHitColmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLClusterHitColmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLClusterHitColmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::ClusterHitCol*> : new vector<JM::ClusterHitCol*>;
   }
   static void *newArray_vectorlEJMcLcLClusterHitColmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::ClusterHitCol*>[nElements] : new vector<JM::ClusterHitCol*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLClusterHitColmUgR(void *p) {
      delete ((vector<JM::ClusterHitCol*>*)p);
   }
   static void deleteArray_vectorlEJMcLcLClusterHitColmUgR(void *p) {
      delete [] ((vector<JM::ClusterHitCol*>*)p);
   }
   static void destruct_vectorlEJMcLcLClusterHitColmUgR(void *p) {
      typedef vector<JM::ClusterHitCol*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::ClusterHitCol*>

namespace {
  void TriggerDictionaryInitialization_ClusterInfoEvtDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterInfoEvt.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ClusterInfo/ClusterInfoEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ClusterInfo/ClusterInfoEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ClusterInfoEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterInfoEvt.h")))  ClusterInfoEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ClusterInfoEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterInfoEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::ClusterInfoEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ClusterInfoEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ClusterInfoEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ClusterInfoEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ClusterInfoEvtDict() {
  TriggerDictionaryInitialization_ClusterInfoEvtDict_Impl();
}

// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldISimEventdIsrcdISimVertexDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimVertex.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLSimVertex(void *p = nullptr);
   static void *newArray_JMcLcLSimVertex(Long_t size, void *p);
   static void delete_JMcLcLSimVertex(void *p);
   static void deleteArray_JMcLcLSimVertex(void *p);
   static void destruct_JMcLcLSimVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::SimVertex*)
   {
      ::JM::SimVertex *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::SimVertex >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::SimVertex", ::JM::SimVertex::Class_Version(), "Event/SimVertex.h", 23,
                  typeid(::JM::SimVertex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::SimVertex::Dictionary, isa_proxy, 4,
                  sizeof(::JM::SimVertex) );
      instance.SetNew(&new_JMcLcLSimVertex);
      instance.SetNewArray(&newArray_JMcLcLSimVertex);
      instance.SetDelete(&delete_JMcLcLSimVertex);
      instance.SetDeleteArray(&deleteArray_JMcLcLSimVertex);
      instance.SetDestructor(&destruct_JMcLcLSimVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::SimVertex*)
   {
      return GenerateInitInstanceLocal((::JM::SimVertex*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::SimVertex*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr SimVertex::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SimVertex::Class_Name()
{
   return "JM::SimVertex";
}

//______________________________________________________________________________
const char *SimVertex::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimVertex*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SimVertex::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimVertex*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimVertex::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimVertex*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimVertex::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimVertex*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void SimVertex::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::SimVertex.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::SimVertex::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::SimVertex::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLSimVertex(void *p) {
      return  p ? new(p) ::JM::SimVertex : new ::JM::SimVertex;
   }
   static void *newArray_JMcLcLSimVertex(Long_t nElements, void *p) {
      return p ? new(p) ::JM::SimVertex[nElements] : new ::JM::SimVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLSimVertex(void *p) {
      delete ((::JM::SimVertex*)p);
   }
   static void deleteArray_JMcLcLSimVertex(void *p) {
      delete [] ((::JM::SimVertex*)p);
   }
   static void destruct_JMcLcLSimVertex(void *p) {
      typedef ::JM::SimVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::SimVertex

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
   static TClass *vectorlEJMcLcLSimVertexgR_Dictionary();
   static void vectorlEJMcLcLSimVertexgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSimVertexgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSimVertexgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSimVertexgR(void *p);
   static void deleteArray_vectorlEJMcLcLSimVertexgR(void *p);
   static void destruct_vectorlEJMcLcLSimVertexgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SimVertex>*)
   {
      vector<JM::SimVertex> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SimVertex>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SimVertex>", -2, "vector", 389,
                  typeid(vector<JM::SimVertex>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSimVertexgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<JM::SimVertex>) );
      instance.SetNew(&new_vectorlEJMcLcLSimVertexgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSimVertexgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSimVertexgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSimVertexgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSimVertexgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SimVertex> >()));

      ::ROOT::AddClassAlternate("vector<JM::SimVertex>","std::vector<JM::SimVertex, std::allocator<JM::SimVertex> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SimVertex>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSimVertexgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SimVertex>*)nullptr)->GetClass();
      vectorlEJMcLcLSimVertexgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSimVertexgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSimVertexgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimVertex> : new vector<JM::SimVertex>;
   }
   static void *newArray_vectorlEJMcLcLSimVertexgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimVertex>[nElements] : new vector<JM::SimVertex>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSimVertexgR(void *p) {
      delete ((vector<JM::SimVertex>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSimVertexgR(void *p) {
      delete [] ((vector<JM::SimVertex>*)p);
   }
   static void destruct_vectorlEJMcLcLSimVertexgR(void *p) {
      typedef vector<JM::SimVertex> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SimVertex>

namespace {
  void TriggerDictionaryInitialization_SimVertexDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimVertex.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SimVertexDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimVertex.h")))  SimVertex;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SimVertexDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimVertex.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::SimVertex", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SimVertexDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SimVertexDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SimVertexDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SimVertexDict() {
  TriggerDictionaryInitialization_SimVertexDict_Impl();
}

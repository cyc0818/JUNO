// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIClusterInfodIClusterInfoEventdIsrcdIClusterHitColDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterHitCol.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLClusterHitCol(void *p = nullptr);
   static void *newArray_JMcLcLClusterHitCol(Long_t size, void *p);
   static void delete_JMcLcLClusterHitCol(void *p);
   static void deleteArray_JMcLcLClusterHitCol(void *p);
   static void destruct_JMcLcLClusterHitCol(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::ClusterHitCol*)
   {
      ::JM::ClusterHitCol *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::ClusterHitCol >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::ClusterHitCol", ::JM::ClusterHitCol::Class_Version(), "Event/ClusterHitCol.h", 22,
                  typeid(::JM::ClusterHitCol), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::ClusterHitCol::Dictionary, isa_proxy, 4,
                  sizeof(::JM::ClusterHitCol) );
      instance.SetNew(&new_JMcLcLClusterHitCol);
      instance.SetNewArray(&newArray_JMcLcLClusterHitCol);
      instance.SetDelete(&delete_JMcLcLClusterHitCol);
      instance.SetDeleteArray(&deleteArray_JMcLcLClusterHitCol);
      instance.SetDestructor(&destruct_JMcLcLClusterHitCol);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::ClusterHitCol*)
   {
      return GenerateInitInstanceLocal((::JM::ClusterHitCol*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::ClusterHitCol*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr ClusterHitCol::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ClusterHitCol::Class_Name()
{
   return "JM::ClusterHitCol";
}

//______________________________________________________________________________
const char *ClusterHitCol::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterHitCol*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ClusterHitCol::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterHitCol*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ClusterHitCol::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterHitCol*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ClusterHitCol::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::ClusterHitCol*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void ClusterHitCol::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::ClusterHitCol.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::ClusterHitCol::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::ClusterHitCol::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLClusterHitCol(void *p) {
      return  p ? new(p) ::JM::ClusterHitCol : new ::JM::ClusterHitCol;
   }
   static void *newArray_JMcLcLClusterHitCol(Long_t nElements, void *p) {
      return p ? new(p) ::JM::ClusterHitCol[nElements] : new ::JM::ClusterHitCol[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLClusterHitCol(void *p) {
      delete ((::JM::ClusterHitCol*)p);
   }
   static void deleteArray_JMcLcLClusterHitCol(void *p) {
      delete [] ((::JM::ClusterHitCol*)p);
   }
   static void destruct_JMcLcLClusterHitCol(void *p) {
      typedef ::JM::ClusterHitCol current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::ClusterHitCol

namespace ROOT {
   static TClass *vectorlEJMcLcLClusterHitIdxgR_Dictionary();
   static void vectorlEJMcLcLClusterHitIdxgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLClusterHitIdxgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLClusterHitIdxgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLClusterHitIdxgR(void *p);
   static void deleteArray_vectorlEJMcLcLClusterHitIdxgR(void *p);
   static void destruct_vectorlEJMcLcLClusterHitIdxgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::ClusterHitIdx>*)
   {
      vector<JM::ClusterHitIdx> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::ClusterHitIdx>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::ClusterHitIdx>", -2, "vector", 389,
                  typeid(vector<JM::ClusterHitIdx>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLClusterHitIdxgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::ClusterHitIdx>) );
      instance.SetNew(&new_vectorlEJMcLcLClusterHitIdxgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLClusterHitIdxgR);
      instance.SetDelete(&delete_vectorlEJMcLcLClusterHitIdxgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLClusterHitIdxgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLClusterHitIdxgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::ClusterHitIdx> >()));

      ::ROOT::AddClassAlternate("vector<JM::ClusterHitIdx>","std::vector<JM::ClusterHitIdx, std::allocator<JM::ClusterHitIdx> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::ClusterHitIdx>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLClusterHitIdxgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::ClusterHitIdx>*)nullptr)->GetClass();
      vectorlEJMcLcLClusterHitIdxgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLClusterHitIdxgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLClusterHitIdxgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::ClusterHitIdx> : new vector<JM::ClusterHitIdx>;
   }
   static void *newArray_vectorlEJMcLcLClusterHitIdxgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::ClusterHitIdx>[nElements] : new vector<JM::ClusterHitIdx>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLClusterHitIdxgR(void *p) {
      delete ((vector<JM::ClusterHitIdx>*)p);
   }
   static void deleteArray_vectorlEJMcLcLClusterHitIdxgR(void *p) {
      delete [] ((vector<JM::ClusterHitIdx>*)p);
   }
   static void destruct_vectorlEJMcLcLClusterHitIdxgR(void *p) {
      typedef vector<JM::ClusterHitIdx> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::ClusterHitIdx>

namespace {
  void TriggerDictionaryInitialization_ClusterHitColDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterHitCol.h",
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
#line 1 "ClusterHitColDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterHitCol.h")))  ClusterHitCol;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ClusterHitColDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ClusterInfo/ClusterInfoEvent/Event/ClusterHitCol.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::ClusterHitCol", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ClusterHitColDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ClusterHitColDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ClusterHitColDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ClusterHitColDict() {
  TriggerDictionaryInitialization_ClusterHitColDict_Impl();
}

// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldISimEventdIsrcdISimTrackDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTrack.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLSimTrack(void *p = nullptr);
   static void *newArray_JMcLcLSimTrack(Long_t size, void *p);
   static void delete_JMcLcLSimTrack(void *p);
   static void deleteArray_JMcLcLSimTrack(void *p);
   static void destruct_JMcLcLSimTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::SimTrack*)
   {
      ::JM::SimTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::SimTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::SimTrack", ::JM::SimTrack::Class_Version(), "Event/SimTrack.h", 22,
                  typeid(::JM::SimTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::SimTrack::Dictionary, isa_proxy, 4,
                  sizeof(::JM::SimTrack) );
      instance.SetNew(&new_JMcLcLSimTrack);
      instance.SetNewArray(&newArray_JMcLcLSimTrack);
      instance.SetDelete(&delete_JMcLcLSimTrack);
      instance.SetDeleteArray(&deleteArray_JMcLcLSimTrack);
      instance.SetDestructor(&destruct_JMcLcLSimTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::SimTrack*)
   {
      return GenerateInitInstanceLocal((::JM::SimTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::SimTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr SimTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SimTrack::Class_Name()
{
   return "JM::SimTrack";
}

//______________________________________________________________________________
const char *SimTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SimTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void SimTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::SimTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::SimTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::SimTrack::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLSimTrack(void *p) {
      return  p ? new(p) ::JM::SimTrack : new ::JM::SimTrack;
   }
   static void *newArray_JMcLcLSimTrack(Long_t nElements, void *p) {
      return p ? new(p) ::JM::SimTrack[nElements] : new ::JM::SimTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLSimTrack(void *p) {
      delete ((::JM::SimTrack*)p);
   }
   static void deleteArray_JMcLcLSimTrack(void *p) {
      delete [] ((::JM::SimTrack*)p);
   }
   static void destruct_JMcLcLSimTrack(void *p) {
      typedef ::JM::SimTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::SimTrack

namespace ROOT {
   static TClass *vectorlEJMcLcLSimTrackgR_Dictionary();
   static void vectorlEJMcLcLSimTrackgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSimTrackgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSimTrackgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSimTrackgR(void *p);
   static void deleteArray_vectorlEJMcLcLSimTrackgR(void *p);
   static void destruct_vectorlEJMcLcLSimTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SimTrack>*)
   {
      vector<JM::SimTrack> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SimTrack>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SimTrack>", -2, "vector", 389,
                  typeid(vector<JM::SimTrack>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSimTrackgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<JM::SimTrack>) );
      instance.SetNew(&new_vectorlEJMcLcLSimTrackgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSimTrackgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSimTrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSimTrackgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSimTrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SimTrack> >()));

      ::ROOT::AddClassAlternate("vector<JM::SimTrack>","std::vector<JM::SimTrack, std::allocator<JM::SimTrack> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SimTrack>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSimTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SimTrack>*)nullptr)->GetClass();
      vectorlEJMcLcLSimTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSimTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSimTrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTrack> : new vector<JM::SimTrack>;
   }
   static void *newArray_vectorlEJMcLcLSimTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTrack>[nElements] : new vector<JM::SimTrack>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSimTrackgR(void *p) {
      delete ((vector<JM::SimTrack>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSimTrackgR(void *p) {
      delete [] ((vector<JM::SimTrack>*)p);
   }
   static void destruct_vectorlEJMcLcLSimTrackgR(void *p) {
      typedef vector<JM::SimTrack> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SimTrack>

namespace {
  void TriggerDictionaryInitialization_SimTrackDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTrack.h",
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
#line 1 "SimTrackDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTrack.h")))  SimTrack;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SimTrackDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTrack.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::SimTrack", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SimTrackDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SimTrackDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SimTrackDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SimTrackDict() {
  TriggerDictionaryInitialization_SimTrackDict_Impl();
}

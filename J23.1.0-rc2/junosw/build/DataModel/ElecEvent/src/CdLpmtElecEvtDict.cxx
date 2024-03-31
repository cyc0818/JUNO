// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecEventdIsrcdICdLpmtElecEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdLpmtElecEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdLpmtElecEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdLpmtElecEvt(Long_t size, void *p);
   static void delete_JMcLcLCdLpmtElecEvt(void *p);
   static void deleteArray_JMcLcLCdLpmtElecEvt(void *p);
   static void destruct_JMcLcLCdLpmtElecEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdLpmtElecEvt*)
   {
      ::JM::CdLpmtElecEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdLpmtElecEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdLpmtElecEvt", ::JM::CdLpmtElecEvt::Class_Version(), "Event/CdLpmtElecEvt.h", 39,
                  typeid(::JM::CdLpmtElecEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdLpmtElecEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdLpmtElecEvt) );
      instance.SetNew(&new_JMcLcLCdLpmtElecEvt);
      instance.SetNewArray(&newArray_JMcLcLCdLpmtElecEvt);
      instance.SetDelete(&delete_JMcLcLCdLpmtElecEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdLpmtElecEvt);
      instance.SetDestructor(&destruct_JMcLcLCdLpmtElecEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdLpmtElecEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdLpmtElecEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdLpmtElecEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdLpmtElecEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdLpmtElecEvt::Class_Name()
{
   return "JM::CdLpmtElecEvt";
}

//______________________________________________________________________________
const char *CdLpmtElecEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdLpmtElecEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdLpmtElecEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdLpmtElecEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdLpmtElecEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdLpmtElecEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdLpmtElecEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdLpmtElecEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdLpmtElecEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdLpmtElecEvt(void *p) {
      return  p ? new(p) ::JM::CdLpmtElecEvt : new ::JM::CdLpmtElecEvt;
   }
   static void *newArray_JMcLcLCdLpmtElecEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdLpmtElecEvt[nElements] : new ::JM::CdLpmtElecEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdLpmtElecEvt(void *p) {
      delete ((::JM::CdLpmtElecEvt*)p);
   }
   static void deleteArray_JMcLcLCdLpmtElecEvt(void *p) {
      delete [] ((::JM::CdLpmtElecEvt*)p);
   }
   static void destruct_JMcLcLCdLpmtElecEvt(void *p) {
      typedef ::JM::CdLpmtElecEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdLpmtElecEvt

namespace ROOT {
   static TClass *maplEintcOJMcLcLElecChannelmUgR_Dictionary();
   static void maplEintcOJMcLcLElecChannelmUgR_TClassManip(TClass*);
   static void *new_maplEintcOJMcLcLElecChannelmUgR(void *p = nullptr);
   static void *newArray_maplEintcOJMcLcLElecChannelmUgR(Long_t size, void *p);
   static void delete_maplEintcOJMcLcLElecChannelmUgR(void *p);
   static void deleteArray_maplEintcOJMcLcLElecChannelmUgR(void *p);
   static void destruct_maplEintcOJMcLcLElecChannelmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,JM::ElecChannel*>*)
   {
      map<int,JM::ElecChannel*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,JM::ElecChannel*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,JM::ElecChannel*>", -2, "map", 100,
                  typeid(map<int,JM::ElecChannel*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOJMcLcLElecChannelmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,JM::ElecChannel*>) );
      instance.SetNew(&new_maplEintcOJMcLcLElecChannelmUgR);
      instance.SetNewArray(&newArray_maplEintcOJMcLcLElecChannelmUgR);
      instance.SetDelete(&delete_maplEintcOJMcLcLElecChannelmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOJMcLcLElecChannelmUgR);
      instance.SetDestructor(&destruct_maplEintcOJMcLcLElecChannelmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,JM::ElecChannel*> >()));

      ::ROOT::AddClassAlternate("map<int,JM::ElecChannel*>","std::map<int, JM::ElecChannel*, std::less<int>, std::allocator<std::pair<int const, JM::ElecChannel*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,JM::ElecChannel*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOJMcLcLElecChannelmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,JM::ElecChannel*>*)nullptr)->GetClass();
      maplEintcOJMcLcLElecChannelmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOJMcLcLElecChannelmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOJMcLcLElecChannelmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,JM::ElecChannel*> : new map<int,JM::ElecChannel*>;
   }
   static void *newArray_maplEintcOJMcLcLElecChannelmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,JM::ElecChannel*>[nElements] : new map<int,JM::ElecChannel*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOJMcLcLElecChannelmUgR(void *p) {
      delete ((map<int,JM::ElecChannel*>*)p);
   }
   static void deleteArray_maplEintcOJMcLcLElecChannelmUgR(void *p) {
      delete [] ((map<int,JM::ElecChannel*>*)p);
   }
   static void destruct_maplEintcOJMcLcLElecChannelmUgR(void *p) {
      typedef map<int,JM::ElecChannel*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,JM::ElecChannel*>

namespace {
  void TriggerDictionaryInitialization_CdLpmtElecEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdLpmtElecEvt.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecEvent",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecEvent/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "CdLpmtElecEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdLpmtElecEvt.h")))  CdLpmtElecEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdLpmtElecEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdLpmtElecEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdLpmtElecEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdLpmtElecEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdLpmtElecEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdLpmtElecEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdLpmtElecEvtDict() {
  TriggerDictionaryInitialization_CdLpmtElecEvtDict_Impl();
}

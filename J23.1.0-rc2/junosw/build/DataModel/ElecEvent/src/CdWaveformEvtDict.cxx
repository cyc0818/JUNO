// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecEventdIsrcdICdWaveformEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdWaveformEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdWaveformEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdWaveformEvt(Long_t size, void *p);
   static void delete_JMcLcLCdWaveformEvt(void *p);
   static void deleteArray_JMcLcLCdWaveformEvt(void *p);
   static void destruct_JMcLcLCdWaveformEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdWaveformEvt*)
   {
      ::JM::CdWaveformEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdWaveformEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdWaveformEvt", ::JM::CdWaveformEvt::Class_Version(), "Event/CdWaveformEvt.h", 41,
                  typeid(::JM::CdWaveformEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdWaveformEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdWaveformEvt) );
      instance.SetNew(&new_JMcLcLCdWaveformEvt);
      instance.SetNewArray(&newArray_JMcLcLCdWaveformEvt);
      instance.SetDelete(&delete_JMcLcLCdWaveformEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdWaveformEvt);
      instance.SetDestructor(&destruct_JMcLcLCdWaveformEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdWaveformEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdWaveformEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdWaveformEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdWaveformEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdWaveformEvt::Class_Name()
{
   return "JM::CdWaveformEvt";
}

//______________________________________________________________________________
const char *CdWaveformEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdWaveformEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdWaveformEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdWaveformEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdWaveformEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdWaveformEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdWaveformEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdWaveformEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdWaveformEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdWaveformEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdWaveformEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdWaveformEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdWaveformEvt(void *p) {
      return  p ? new(p) ::JM::CdWaveformEvt : new ::JM::CdWaveformEvt;
   }
   static void *newArray_JMcLcLCdWaveformEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdWaveformEvt[nElements] : new ::JM::CdWaveformEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdWaveformEvt(void *p) {
      delete ((::JM::CdWaveformEvt*)p);
   }
   static void deleteArray_JMcLcLCdWaveformEvt(void *p) {
      delete [] ((::JM::CdWaveformEvt*)p);
   }
   static void destruct_JMcLcLCdWaveformEvt(void *p) {
      typedef ::JM::CdWaveformEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdWaveformEvt

namespace ROOT {
   static TClass *maplEintcOJMcLcLElecWaveformmUgR_Dictionary();
   static void maplEintcOJMcLcLElecWaveformmUgR_TClassManip(TClass*);
   static void *new_maplEintcOJMcLcLElecWaveformmUgR(void *p = nullptr);
   static void *newArray_maplEintcOJMcLcLElecWaveformmUgR(Long_t size, void *p);
   static void delete_maplEintcOJMcLcLElecWaveformmUgR(void *p);
   static void deleteArray_maplEintcOJMcLcLElecWaveformmUgR(void *p);
   static void destruct_maplEintcOJMcLcLElecWaveformmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,JM::ElecWaveform*>*)
   {
      map<int,JM::ElecWaveform*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,JM::ElecWaveform*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,JM::ElecWaveform*>", -2, "map", 100,
                  typeid(map<int,JM::ElecWaveform*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOJMcLcLElecWaveformmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,JM::ElecWaveform*>) );
      instance.SetNew(&new_maplEintcOJMcLcLElecWaveformmUgR);
      instance.SetNewArray(&newArray_maplEintcOJMcLcLElecWaveformmUgR);
      instance.SetDelete(&delete_maplEintcOJMcLcLElecWaveformmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOJMcLcLElecWaveformmUgR);
      instance.SetDestructor(&destruct_maplEintcOJMcLcLElecWaveformmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,JM::ElecWaveform*> >()));

      ::ROOT::AddClassAlternate("map<int,JM::ElecWaveform*>","std::map<int, JM::ElecWaveform*, std::less<int>, std::allocator<std::pair<int const, JM::ElecWaveform*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,JM::ElecWaveform*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOJMcLcLElecWaveformmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,JM::ElecWaveform*>*)nullptr)->GetClass();
      maplEintcOJMcLcLElecWaveformmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOJMcLcLElecWaveformmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOJMcLcLElecWaveformmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,JM::ElecWaveform*> : new map<int,JM::ElecWaveform*>;
   }
   static void *newArray_maplEintcOJMcLcLElecWaveformmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,JM::ElecWaveform*>[nElements] : new map<int,JM::ElecWaveform*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOJMcLcLElecWaveformmUgR(void *p) {
      delete ((map<int,JM::ElecWaveform*>*)p);
   }
   static void deleteArray_maplEintcOJMcLcLElecWaveformmUgR(void *p) {
      delete [] ((map<int,JM::ElecWaveform*>*)p);
   }
   static void destruct_maplEintcOJMcLcLElecWaveformmUgR(void *p) {
      typedef map<int,JM::ElecWaveform*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,JM::ElecWaveform*>

namespace {
  void TriggerDictionaryInitialization_CdWaveformEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdWaveformEvt.h",
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
#line 1 "CdWaveformEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdWaveformEvt.h")))  CdWaveformEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdWaveformEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdWaveformEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdWaveformEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdWaveformEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdWaveformEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdWaveformEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdWaveformEvtDict() {
  TriggerDictionaryInitialization_CdWaveformEvtDict_Impl();
}

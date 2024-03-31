// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIContextdIsrcdITimeStampDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Context/Context/TimeStamp.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TimeStamp(void *p = nullptr);
   static void *newArray_TimeStamp(Long_t size, void *p);
   static void delete_TimeStamp(void *p);
   static void deleteArray_TimeStamp(void *p);
   static void destruct_TimeStamp(void *p);
   static void streamer_TimeStamp(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TimeStamp*)
   {
      ::TimeStamp *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TimeStamp >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TimeStamp", ::TimeStamp::Class_Version(), "Context/TimeStamp.h", 85,
                  typeid(::TimeStamp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TimeStamp::Dictionary, isa_proxy, 16,
                  sizeof(::TimeStamp) );
      instance.SetNew(&new_TimeStamp);
      instance.SetNewArray(&newArray_TimeStamp);
      instance.SetDelete(&delete_TimeStamp);
      instance.SetDeleteArray(&deleteArray_TimeStamp);
      instance.SetDestructor(&destruct_TimeStamp);
      instance.SetStreamerFunc(&streamer_TimeStamp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TimeStamp*)
   {
      return GenerateInitInstanceLocal((::TimeStamp*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TimeStamp*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TimeStamp::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TimeStamp::Class_Name()
{
   return "TimeStamp";
}

//______________________________________________________________________________
const char *TimeStamp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimeStamp*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TimeStamp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimeStamp*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TimeStamp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimeStamp*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TimeStamp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimeStamp*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TimeStamp::Streamer(TBuffer &R__b)
{
   // Stream an object of class TimeStamp.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> mSec;
      R__b >> mNanoSec;
      R__b.CheckByteCount(R__s, R__c, TimeStamp::IsA());
   } else {
      R__c = R__b.WriteVersion(TimeStamp::IsA(), kTRUE);
      R__b << mSec;
      R__b << mNanoSec;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TimeStamp(void *p) {
      return  p ? new(p) ::TimeStamp : new ::TimeStamp;
   }
   static void *newArray_TimeStamp(Long_t nElements, void *p) {
      return p ? new(p) ::TimeStamp[nElements] : new ::TimeStamp[nElements];
   }
   // Wrapper around operator delete
   static void delete_TimeStamp(void *p) {
      delete ((::TimeStamp*)p);
   }
   static void deleteArray_TimeStamp(void *p) {
      delete [] ((::TimeStamp*)p);
   }
   static void destruct_TimeStamp(void *p) {
      typedef ::TimeStamp current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TimeStamp(TBuffer &buf, void *obj) {
      ((::TimeStamp*)obj)->::TimeStamp::Streamer(buf);
   }
} // end of namespace ROOT for class ::TimeStamp

namespace {
  void TriggerDictionaryInitialization_TimeStampDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Context/Context/TimeStamp.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Context",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Context/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/Context",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/Context/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TimeStampDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Context/Context/TimeStamp.h")))  TimeStamp;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TimeStampDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Context/Context/TimeStamp.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TimeStamp", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TimeStampDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TimeStampDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TimeStampDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TimeStampDict() {
  TriggerDictionaryInitialization_TimeStampDict_Impl();
}

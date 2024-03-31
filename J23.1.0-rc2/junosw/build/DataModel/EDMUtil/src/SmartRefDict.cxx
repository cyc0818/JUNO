// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIEDMUtildIsrcdISmartRefDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil/EDMUtil/SmartRef.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLSmartRef(void *p = nullptr);
   static void *newArray_JMcLcLSmartRef(Long_t size, void *p);
   static void delete_JMcLcLSmartRef(void *p);
   static void deleteArray_JMcLcLSmartRef(void *p);
   static void destruct_JMcLcLSmartRef(void *p);
   static void streamer_JMcLcLSmartRef(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::SmartRef*)
   {
      ::JM::SmartRef *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::SmartRef >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::SmartRef", ::JM::SmartRef::Class_Version(), "EDMUtil/SmartRef.h", 16,
                  typeid(::JM::SmartRef), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::SmartRef::Dictionary, isa_proxy, 17,
                  sizeof(::JM::SmartRef) );
      instance.SetNew(&new_JMcLcLSmartRef);
      instance.SetNewArray(&newArray_JMcLcLSmartRef);
      instance.SetDelete(&delete_JMcLcLSmartRef);
      instance.SetDeleteArray(&deleteArray_JMcLcLSmartRef);
      instance.SetDestructor(&destruct_JMcLcLSmartRef);
      instance.SetStreamerFunc(&streamer_JMcLcLSmartRef);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::SmartRef*)
   {
      return GenerateInitInstanceLocal((::JM::SmartRef*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::SmartRef*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr SmartRef::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SmartRef::Class_Name()
{
   return "JM::SmartRef";
}

//______________________________________________________________________________
const char *SmartRef::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SmartRef*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SmartRef::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SmartRef*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SmartRef::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SmartRef*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SmartRef::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SmartRef*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLSmartRef(void *p) {
      return  p ? new(p) ::JM::SmartRef : new ::JM::SmartRef;
   }
   static void *newArray_JMcLcLSmartRef(Long_t nElements, void *p) {
      return p ? new(p) ::JM::SmartRef[nElements] : new ::JM::SmartRef[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLSmartRef(void *p) {
      delete ((::JM::SmartRef*)p);
   }
   static void deleteArray_JMcLcLSmartRef(void *p) {
      delete [] ((::JM::SmartRef*)p);
   }
   static void destruct_JMcLcLSmartRef(void *p) {
      typedef ::JM::SmartRef current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_JMcLcLSmartRef(TBuffer &buf, void *obj) {
      ((::JM::SmartRef*)obj)->::JM::SmartRef::Streamer(buf);
   }
} // end of namespace ROOT for class ::JM::SmartRef

namespace {
  void TriggerDictionaryInitialization_SmartRefDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil/EDMUtil/SmartRef.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/EDMUtil",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/EDMUtil/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SmartRefDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil/EDMUtil/SmartRef.h")))  SmartRef;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SmartRefDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil/EDMUtil/SmartRef.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::SmartRef", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SmartRefDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SmartRefDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SmartRefDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SmartRefDict() {
  TriggerDictionaryInitialization_SmartRefDict_Impl();
}

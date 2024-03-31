// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecEventdIsrcdISpmtElecSpecialWordDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/SpmtElecSpecialWord.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLSpmtElecSpecialWord(void *p = nullptr);
   static void *newArray_JMcLcLSpmtElecSpecialWord(Long_t size, void *p);
   static void delete_JMcLcLSpmtElecSpecialWord(void *p);
   static void deleteArray_JMcLcLSpmtElecSpecialWord(void *p);
   static void destruct_JMcLcLSpmtElecSpecialWord(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::SpmtElecSpecialWord*)
   {
      ::JM::SpmtElecSpecialWord *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::SpmtElecSpecialWord >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::SpmtElecSpecialWord", ::JM::SpmtElecSpecialWord::Class_Version(), "Event/SpmtElecSpecialWord.h", 37,
                  typeid(::JM::SpmtElecSpecialWord), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::SpmtElecSpecialWord::Dictionary, isa_proxy, 4,
                  sizeof(::JM::SpmtElecSpecialWord) );
      instance.SetNew(&new_JMcLcLSpmtElecSpecialWord);
      instance.SetNewArray(&newArray_JMcLcLSpmtElecSpecialWord);
      instance.SetDelete(&delete_JMcLcLSpmtElecSpecialWord);
      instance.SetDeleteArray(&deleteArray_JMcLcLSpmtElecSpecialWord);
      instance.SetDestructor(&destruct_JMcLcLSpmtElecSpecialWord);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::SpmtElecSpecialWord*)
   {
      return GenerateInitInstanceLocal((::JM::SpmtElecSpecialWord*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::SpmtElecSpecialWord*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr SpmtElecSpecialWord::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SpmtElecSpecialWord::Class_Name()
{
   return "JM::SpmtElecSpecialWord";
}

//______________________________________________________________________________
const char *SpmtElecSpecialWord::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecSpecialWord*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SpmtElecSpecialWord::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecSpecialWord*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SpmtElecSpecialWord::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecSpecialWord*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SpmtElecSpecialWord::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SpmtElecSpecialWord*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void SpmtElecSpecialWord::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::SpmtElecSpecialWord.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::SpmtElecSpecialWord::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::SpmtElecSpecialWord::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLSpmtElecSpecialWord(void *p) {
      return  p ? new(p) ::JM::SpmtElecSpecialWord : new ::JM::SpmtElecSpecialWord;
   }
   static void *newArray_JMcLcLSpmtElecSpecialWord(Long_t nElements, void *p) {
      return p ? new(p) ::JM::SpmtElecSpecialWord[nElements] : new ::JM::SpmtElecSpecialWord[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLSpmtElecSpecialWord(void *p) {
      delete ((::JM::SpmtElecSpecialWord*)p);
   }
   static void deleteArray_JMcLcLSpmtElecSpecialWord(void *p) {
      delete [] ((::JM::SpmtElecSpecialWord*)p);
   }
   static void destruct_JMcLcLSpmtElecSpecialWord(void *p) {
      typedef ::JM::SpmtElecSpecialWord current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::SpmtElecSpecialWord

namespace {
  void TriggerDictionaryInitialization_SpmtElecSpecialWordDict_Impl() {
    static const char* headers[] = {
"Event/SpmtElecSpecialWord.h",
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
#line 1 "SpmtElecSpecialWordDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/SpmtElecSpecialWord.h")))  SpmtElecSpecialWord;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SpmtElecSpecialWordDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/SpmtElecSpecialWord.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::SpmtElecSpecialWord", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SpmtElecSpecialWordDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SpmtElecSpecialWordDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SpmtElecSpecialWordDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SpmtElecSpecialWordDict() {
  TriggerDictionaryInitialization_SpmtElecSpecialWordDict_Impl();
}

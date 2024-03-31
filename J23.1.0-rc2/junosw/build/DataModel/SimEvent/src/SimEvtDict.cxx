// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldISimEventdIsrcdISimEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLSimEvt(void *p = nullptr);
   static void *newArray_JMcLcLSimEvt(Long_t size, void *p);
   static void delete_JMcLcLSimEvt(void *p);
   static void deleteArray_JMcLcLSimEvt(void *p);
   static void destruct_JMcLcLSimEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::SimEvt*)
   {
      ::JM::SimEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::SimEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::SimEvt", ::JM::SimEvt::Class_Version(), "Event/SimEvt.h", 30,
                  typeid(::JM::SimEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::SimEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::SimEvt) );
      instance.SetNew(&new_JMcLcLSimEvt);
      instance.SetNewArray(&newArray_JMcLcLSimEvt);
      instance.SetDelete(&delete_JMcLcLSimEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLSimEvt);
      instance.SetDestructor(&destruct_JMcLcLSimEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::SimEvt*)
   {
      return GenerateInitInstanceLocal((::JM::SimEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::SimEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr SimEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SimEvt::Class_Name()
{
   return "JM::SimEvt";
}

//______________________________________________________________________________
const char *SimEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SimEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::SimEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::SimEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void SimEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::SimEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::SimEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::SimEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLSimEvt(void *p) {
      return  p ? new(p) ::JM::SimEvt : new ::JM::SimEvt;
   }
   static void *newArray_JMcLcLSimEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::SimEvt[nElements] : new ::JM::SimEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLSimEvt(void *p) {
      delete ((::JM::SimEvt*)p);
   }
   static void deleteArray_JMcLcLSimEvt(void *p) {
      delete [] ((::JM::SimEvt*)p);
   }
   static void destruct_JMcLcLSimEvt(void *p) {
      typedef ::JM::SimEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::SimEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLSimVertexmUgR_Dictionary();
   static void vectorlEJMcLcLSimVertexmUgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSimVertexmUgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSimVertexmUgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSimVertexmUgR(void *p);
   static void deleteArray_vectorlEJMcLcLSimVertexmUgR(void *p);
   static void destruct_vectorlEJMcLcLSimVertexmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SimVertex*>*)
   {
      vector<JM::SimVertex*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SimVertex*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SimVertex*>", -2, "vector", 389,
                  typeid(vector<JM::SimVertex*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSimVertexmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SimVertex*>) );
      instance.SetNew(&new_vectorlEJMcLcLSimVertexmUgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSimVertexmUgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSimVertexmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSimVertexmUgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSimVertexmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SimVertex*> >()));

      ::ROOT::AddClassAlternate("vector<JM::SimVertex*>","std::vector<JM::SimVertex*, std::allocator<JM::SimVertex*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SimVertex*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSimVertexmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SimVertex*>*)nullptr)->GetClass();
      vectorlEJMcLcLSimVertexmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSimVertexmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSimVertexmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimVertex*> : new vector<JM::SimVertex*>;
   }
   static void *newArray_vectorlEJMcLcLSimVertexmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimVertex*>[nElements] : new vector<JM::SimVertex*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSimVertexmUgR(void *p) {
      delete ((vector<JM::SimVertex*>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSimVertexmUgR(void *p) {
      delete [] ((vector<JM::SimVertex*>*)p);
   }
   static void destruct_vectorlEJMcLcLSimVertexmUgR(void *p) {
      typedef vector<JM::SimVertex*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SimVertex*>

namespace ROOT {
   static TClass *vectorlEJMcLcLSimTrackmUgR_Dictionary();
   static void vectorlEJMcLcLSimTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSimTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSimTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSimTrackmUgR(void *p);
   static void deleteArray_vectorlEJMcLcLSimTrackmUgR(void *p);
   static void destruct_vectorlEJMcLcLSimTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SimTrack*>*)
   {
      vector<JM::SimTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SimTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SimTrack*>", -2, "vector", 389,
                  typeid(vector<JM::SimTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSimTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SimTrack*>) );
      instance.SetNew(&new_vectorlEJMcLcLSimTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSimTrackmUgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSimTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSimTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSimTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SimTrack*> >()));

      ::ROOT::AddClassAlternate("vector<JM::SimTrack*>","std::vector<JM::SimTrack*, std::allocator<JM::SimTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SimTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSimTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SimTrack*>*)nullptr)->GetClass();
      vectorlEJMcLcLSimTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSimTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSimTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTrack*> : new vector<JM::SimTrack*>;
   }
   static void *newArray_vectorlEJMcLcLSimTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTrack*>[nElements] : new vector<JM::SimTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSimTrackmUgR(void *p) {
      delete ((vector<JM::SimTrack*>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSimTrackmUgR(void *p) {
      delete [] ((vector<JM::SimTrack*>*)p);
   }
   static void destruct_vectorlEJMcLcLSimTrackmUgR(void *p) {
      typedef vector<JM::SimTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SimTrack*>

namespace ROOT {
   static TClass *vectorlEJMcLcLSimTTHitmUgR_Dictionary();
   static void vectorlEJMcLcLSimTTHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSimTTHitmUgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSimTTHitmUgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSimTTHitmUgR(void *p);
   static void deleteArray_vectorlEJMcLcLSimTTHitmUgR(void *p);
   static void destruct_vectorlEJMcLcLSimTTHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SimTTHit*>*)
   {
      vector<JM::SimTTHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SimTTHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SimTTHit*>", -2, "vector", 389,
                  typeid(vector<JM::SimTTHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSimTTHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SimTTHit*>) );
      instance.SetNew(&new_vectorlEJMcLcLSimTTHitmUgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSimTTHitmUgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSimTTHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSimTTHitmUgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSimTTHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SimTTHit*> >()));

      ::ROOT::AddClassAlternate("vector<JM::SimTTHit*>","std::vector<JM::SimTTHit*, std::allocator<JM::SimTTHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SimTTHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSimTTHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SimTTHit*>*)nullptr)->GetClass();
      vectorlEJMcLcLSimTTHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSimTTHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSimTTHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTTHit*> : new vector<JM::SimTTHit*>;
   }
   static void *newArray_vectorlEJMcLcLSimTTHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimTTHit*>[nElements] : new vector<JM::SimTTHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSimTTHitmUgR(void *p) {
      delete ((vector<JM::SimTTHit*>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSimTTHitmUgR(void *p) {
      delete [] ((vector<JM::SimTTHit*>*)p);
   }
   static void destruct_vectorlEJMcLcLSimTTHitmUgR(void *p) {
      typedef vector<JM::SimTTHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SimTTHit*>

namespace ROOT {
   static TClass *vectorlEJMcLcLSimPMTHitmUgR_Dictionary();
   static void vectorlEJMcLcLSimPMTHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSimPMTHitmUgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSimPMTHitmUgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSimPMTHitmUgR(void *p);
   static void deleteArray_vectorlEJMcLcLSimPMTHitmUgR(void *p);
   static void destruct_vectorlEJMcLcLSimPMTHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SimPMTHit*>*)
   {
      vector<JM::SimPMTHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SimPMTHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SimPMTHit*>", -2, "vector", 389,
                  typeid(vector<JM::SimPMTHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSimPMTHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SimPMTHit*>) );
      instance.SetNew(&new_vectorlEJMcLcLSimPMTHitmUgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSimPMTHitmUgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSimPMTHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSimPMTHitmUgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSimPMTHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SimPMTHit*> >()));

      ::ROOT::AddClassAlternate("vector<JM::SimPMTHit*>","std::vector<JM::SimPMTHit*, std::allocator<JM::SimPMTHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SimPMTHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSimPMTHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SimPMTHit*>*)nullptr)->GetClass();
      vectorlEJMcLcLSimPMTHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSimPMTHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSimPMTHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimPMTHit*> : new vector<JM::SimPMTHit*>;
   }
   static void *newArray_vectorlEJMcLcLSimPMTHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SimPMTHit*>[nElements] : new vector<JM::SimPMTHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSimPMTHitmUgR(void *p) {
      delete ((vector<JM::SimPMTHit*>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSimPMTHitmUgR(void *p) {
      delete [] ((vector<JM::SimPMTHit*>*)p);
   }
   static void destruct_vectorlEJMcLcLSimPMTHitmUgR(void *p) {
      typedef vector<JM::SimPMTHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SimPMTHit*>

namespace {
  void TriggerDictionaryInitialization_SimEvtDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimEvt.h",
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
#line 1 "SimEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimEvt.h")))  SimEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SimEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::SimEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SimEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SimEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SimEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SimEvtDict() {
  TriggerDictionaryInitialization_SimEvtDict_Impl();
}

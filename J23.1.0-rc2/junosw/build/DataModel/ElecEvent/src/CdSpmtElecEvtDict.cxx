// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIElecEventdIsrcdICdSpmtElecEvtDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdSpmtElecEvt.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_JMcLcLCdSpmtElecEvt(void *p = nullptr);
   static void *newArray_JMcLcLCdSpmtElecEvt(Long_t size, void *p);
   static void delete_JMcLcLCdSpmtElecEvt(void *p);
   static void deleteArray_JMcLcLCdSpmtElecEvt(void *p);
   static void destruct_JMcLcLCdSpmtElecEvt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::CdSpmtElecEvt*)
   {
      ::JM::CdSpmtElecEvt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::CdSpmtElecEvt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JM::CdSpmtElecEvt", ::JM::CdSpmtElecEvt::Class_Version(), "Event/CdSpmtElecEvt.h", 42,
                  typeid(::JM::CdSpmtElecEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::CdSpmtElecEvt::Dictionary, isa_proxy, 4,
                  sizeof(::JM::CdSpmtElecEvt) );
      instance.SetNew(&new_JMcLcLCdSpmtElecEvt);
      instance.SetNewArray(&newArray_JMcLcLCdSpmtElecEvt);
      instance.SetDelete(&delete_JMcLcLCdSpmtElecEvt);
      instance.SetDeleteArray(&deleteArray_JMcLcLCdSpmtElecEvt);
      instance.SetDestructor(&destruct_JMcLcLCdSpmtElecEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::CdSpmtElecEvt*)
   {
      return GenerateInitInstanceLocal((::JM::CdSpmtElecEvt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::CdSpmtElecEvt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr CdSpmtElecEvt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CdSpmtElecEvt::Class_Name()
{
   return "JM::CdSpmtElecEvt";
}

//______________________________________________________________________________
const char *CdSpmtElecEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecEvt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CdSpmtElecEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecEvt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CdSpmtElecEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecEvt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CdSpmtElecEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::CdSpmtElecEvt*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void CdSpmtElecEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::CdSpmtElecEvt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::CdSpmtElecEvt::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::CdSpmtElecEvt::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLCdSpmtElecEvt(void *p) {
      return  p ? new(p) ::JM::CdSpmtElecEvt : new ::JM::CdSpmtElecEvt;
   }
   static void *newArray_JMcLcLCdSpmtElecEvt(Long_t nElements, void *p) {
      return p ? new(p) ::JM::CdSpmtElecEvt[nElements] : new ::JM::CdSpmtElecEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLCdSpmtElecEvt(void *p) {
      delete ((::JM::CdSpmtElecEvt*)p);
   }
   static void deleteArray_JMcLcLCdSpmtElecEvt(void *p) {
      delete [] ((::JM::CdSpmtElecEvt*)p);
   }
   static void destruct_JMcLcLCdSpmtElecEvt(void *p) {
      typedef ::JM::CdSpmtElecEvt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::CdSpmtElecEvt

namespace ROOT {
   static TClass *vectorlEJMcLcLSpmtElecSpecialWordgR_Dictionary();
   static void vectorlEJMcLcLSpmtElecSpecialWordgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSpmtElecSpecialWordgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p);
   static void deleteArray_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p);
   static void destruct_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SpmtElecSpecialWord>*)
   {
      vector<JM::SpmtElecSpecialWord> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SpmtElecSpecialWord>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SpmtElecSpecialWord>", -2, "vector", 389,
                  typeid(vector<JM::SpmtElecSpecialWord>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSpmtElecSpecialWordgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SpmtElecSpecialWord>) );
      instance.SetNew(&new_vectorlEJMcLcLSpmtElecSpecialWordgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSpmtElecSpecialWordgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSpmtElecSpecialWordgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSpmtElecSpecialWordgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSpmtElecSpecialWordgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SpmtElecSpecialWord> >()));

      ::ROOT::AddClassAlternate("vector<JM::SpmtElecSpecialWord>","std::vector<JM::SpmtElecSpecialWord, std::allocator<JM::SpmtElecSpecialWord> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SpmtElecSpecialWord>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSpmtElecSpecialWordgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SpmtElecSpecialWord>*)nullptr)->GetClass();
      vectorlEJMcLcLSpmtElecSpecialWordgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSpmtElecSpecialWordgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecSpecialWord> : new vector<JM::SpmtElecSpecialWord>;
   }
   static void *newArray_vectorlEJMcLcLSpmtElecSpecialWordgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecSpecialWord>[nElements] : new vector<JM::SpmtElecSpecialWord>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p) {
      delete ((vector<JM::SpmtElecSpecialWord>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p) {
      delete [] ((vector<JM::SpmtElecSpecialWord>*)p);
   }
   static void destruct_vectorlEJMcLcLSpmtElecSpecialWordgR(void *p) {
      typedef vector<JM::SpmtElecSpecialWord> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SpmtElecSpecialWord>

namespace ROOT {
   static TClass *vectorlEJMcLcLSpmtElecDiscrWordgR_Dictionary();
   static void vectorlEJMcLcLSpmtElecDiscrWordgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSpmtElecDiscrWordgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p);
   static void deleteArray_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p);
   static void destruct_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SpmtElecDiscrWord>*)
   {
      vector<JM::SpmtElecDiscrWord> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SpmtElecDiscrWord>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SpmtElecDiscrWord>", -2, "vector", 389,
                  typeid(vector<JM::SpmtElecDiscrWord>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSpmtElecDiscrWordgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SpmtElecDiscrWord>) );
      instance.SetNew(&new_vectorlEJMcLcLSpmtElecDiscrWordgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSpmtElecDiscrWordgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSpmtElecDiscrWordgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSpmtElecDiscrWordgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSpmtElecDiscrWordgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SpmtElecDiscrWord> >()));

      ::ROOT::AddClassAlternate("vector<JM::SpmtElecDiscrWord>","std::vector<JM::SpmtElecDiscrWord, std::allocator<JM::SpmtElecDiscrWord> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SpmtElecDiscrWord>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSpmtElecDiscrWordgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SpmtElecDiscrWord>*)nullptr)->GetClass();
      vectorlEJMcLcLSpmtElecDiscrWordgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSpmtElecDiscrWordgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecDiscrWord> : new vector<JM::SpmtElecDiscrWord>;
   }
   static void *newArray_vectorlEJMcLcLSpmtElecDiscrWordgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecDiscrWord>[nElements] : new vector<JM::SpmtElecDiscrWord>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p) {
      delete ((vector<JM::SpmtElecDiscrWord>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p) {
      delete [] ((vector<JM::SpmtElecDiscrWord>*)p);
   }
   static void destruct_vectorlEJMcLcLSpmtElecDiscrWordgR(void *p) {
      typedef vector<JM::SpmtElecDiscrWord> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SpmtElecDiscrWord>

namespace ROOT {
   static TClass *vectorlEJMcLcLSpmtElecAbcBlockgR_Dictionary();
   static void vectorlEJMcLcLSpmtElecAbcBlockgR_TClassManip(TClass*);
   static void *new_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p = nullptr);
   static void *newArray_vectorlEJMcLcLSpmtElecAbcBlockgR(Long_t size, void *p);
   static void delete_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p);
   static void deleteArray_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p);
   static void destruct_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JM::SpmtElecAbcBlock>*)
   {
      vector<JM::SpmtElecAbcBlock> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JM::SpmtElecAbcBlock>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JM::SpmtElecAbcBlock>", -2, "vector", 389,
                  typeid(vector<JM::SpmtElecAbcBlock>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJMcLcLSpmtElecAbcBlockgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JM::SpmtElecAbcBlock>) );
      instance.SetNew(&new_vectorlEJMcLcLSpmtElecAbcBlockgR);
      instance.SetNewArray(&newArray_vectorlEJMcLcLSpmtElecAbcBlockgR);
      instance.SetDelete(&delete_vectorlEJMcLcLSpmtElecAbcBlockgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJMcLcLSpmtElecAbcBlockgR);
      instance.SetDestructor(&destruct_vectorlEJMcLcLSpmtElecAbcBlockgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JM::SpmtElecAbcBlock> >()));

      ::ROOT::AddClassAlternate("vector<JM::SpmtElecAbcBlock>","std::vector<JM::SpmtElecAbcBlock, std::allocator<JM::SpmtElecAbcBlock> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JM::SpmtElecAbcBlock>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJMcLcLSpmtElecAbcBlockgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JM::SpmtElecAbcBlock>*)nullptr)->GetClass();
      vectorlEJMcLcLSpmtElecAbcBlockgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJMcLcLSpmtElecAbcBlockgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecAbcBlock> : new vector<JM::SpmtElecAbcBlock>;
   }
   static void *newArray_vectorlEJMcLcLSpmtElecAbcBlockgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JM::SpmtElecAbcBlock>[nElements] : new vector<JM::SpmtElecAbcBlock>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p) {
      delete ((vector<JM::SpmtElecAbcBlock>*)p);
   }
   static void deleteArray_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p) {
      delete [] ((vector<JM::SpmtElecAbcBlock>*)p);
   }
   static void destruct_vectorlEJMcLcLSpmtElecAbcBlockgR(void *p) {
      typedef vector<JM::SpmtElecAbcBlock> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JM::SpmtElecAbcBlock>

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
  void TriggerDictionaryInitialization_CdSpmtElecEvtDict_Impl() {
    static const char* headers[] = {
"Event/CdSpmtElecEvt.h",
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
#line 1 "CdSpmtElecEvtDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$Event/CdSpmtElecEvt.h")))  CdSpmtElecEvt;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CdSpmtElecEvtDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Event/CdSpmtElecEvt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::CdSpmtElecEvt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CdSpmtElecEvtDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CdSpmtElecEvtDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CdSpmtElecEvtDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CdSpmtElecEvtDict() {
  TriggerDictionaryInitialization_CdSpmtElecEvtDict_Impl();
}

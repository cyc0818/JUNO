// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIDictdIHepMCDictdIsrcdIHepMCIncDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/HepMCDict/src/HepMCInc.h"
#include "HepMC/Flow.h"
#include "HepMC/GenCrossSection.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenRanges.h"
#include "HepMC/GenVertex.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/IO_AsciiParticles.h"
#include "HepMC/IO_BaseClass.h"
#include "HepMC/IO_Exception.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/Polarization.h"
#include "HepMC/SimpleVector.h"
#include "HepMC/StreamInfo.h"
#include "HepMC/TempParticleMap.h"
#include "HepMC/WeightContainer.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *pairlEintcOHepMCcLcLGenParticlemUgR_Dictionary();
   static void pairlEintcOHepMCcLcLGenParticlemUgR_TClassManip(TClass*);
   static void *new_pairlEintcOHepMCcLcLGenParticlemUgR(void *p = nullptr);
   static void *newArray_pairlEintcOHepMCcLcLGenParticlemUgR(Long_t size, void *p);
   static void delete_pairlEintcOHepMCcLcLGenParticlemUgR(void *p);
   static void deleteArray_pairlEintcOHepMCcLcLGenParticlemUgR(void *p);
   static void destruct_pairlEintcOHepMCcLcLGenParticlemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const pair<int,HepMC::GenParticle*>*)
   {
      pair<int,HepMC::GenParticle*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(pair<int,HepMC::GenParticle*>));
      static ::ROOT::TGenericClassInfo 
         instance("pair<int,HepMC::GenParticle*>", "string", 211,
                  typeid(pair<int,HepMC::GenParticle*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &pairlEintcOHepMCcLcLGenParticlemUgR_Dictionary, isa_proxy, 4,
                  sizeof(pair<int,HepMC::GenParticle*>) );
      instance.SetNew(&new_pairlEintcOHepMCcLcLGenParticlemUgR);
      instance.SetNewArray(&newArray_pairlEintcOHepMCcLcLGenParticlemUgR);
      instance.SetDelete(&delete_pairlEintcOHepMCcLcLGenParticlemUgR);
      instance.SetDeleteArray(&deleteArray_pairlEintcOHepMCcLcLGenParticlemUgR);
      instance.SetDestructor(&destruct_pairlEintcOHepMCcLcLGenParticlemUgR);

      ::ROOT::AddClassAlternate("pair<int,HepMC::GenParticle*>","std::pair<int, HepMC::GenParticle*>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const pair<int,HepMC::GenParticle*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *pairlEintcOHepMCcLcLGenParticlemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const pair<int,HepMC::GenParticle*>*)nullptr)->GetClass();
      pairlEintcOHepMCcLcLGenParticlemUgR_TClassManip(theClass);
   return theClass;
   }

   static void pairlEintcOHepMCcLcLGenParticlemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *pairlEintcOHepMCcLcLGenVertexmUgR_Dictionary();
   static void pairlEintcOHepMCcLcLGenVertexmUgR_TClassManip(TClass*);
   static void *new_pairlEintcOHepMCcLcLGenVertexmUgR(void *p = nullptr);
   static void *newArray_pairlEintcOHepMCcLcLGenVertexmUgR(Long_t size, void *p);
   static void delete_pairlEintcOHepMCcLcLGenVertexmUgR(void *p);
   static void deleteArray_pairlEintcOHepMCcLcLGenVertexmUgR(void *p);
   static void destruct_pairlEintcOHepMCcLcLGenVertexmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const pair<int,HepMC::GenVertex*>*)
   {
      pair<int,HepMC::GenVertex*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(pair<int,HepMC::GenVertex*>));
      static ::ROOT::TGenericClassInfo 
         instance("pair<int,HepMC::GenVertex*>", "string", 211,
                  typeid(pair<int,HepMC::GenVertex*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &pairlEintcOHepMCcLcLGenVertexmUgR_Dictionary, isa_proxy, 4,
                  sizeof(pair<int,HepMC::GenVertex*>) );
      instance.SetNew(&new_pairlEintcOHepMCcLcLGenVertexmUgR);
      instance.SetNewArray(&newArray_pairlEintcOHepMCcLcLGenVertexmUgR);
      instance.SetDelete(&delete_pairlEintcOHepMCcLcLGenVertexmUgR);
      instance.SetDeleteArray(&deleteArray_pairlEintcOHepMCcLcLGenVertexmUgR);
      instance.SetDestructor(&destruct_pairlEintcOHepMCcLcLGenVertexmUgR);

      ::ROOT::AddClassAlternate("pair<int,HepMC::GenVertex*>","std::pair<int, HepMC::GenVertex*>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const pair<int,HepMC::GenVertex*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *pairlEintcOHepMCcLcLGenVertexmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const pair<int,HepMC::GenVertex*>*)nullptr)->GetClass();
      pairlEintcOHepMCcLcLGenVertexmUgR_TClassManip(theClass);
   return theClass;
   }

   static void pairlEintcOHepMCcLcLGenVertexmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *pairlEstringcOunsignedsPlonggR_Dictionary();
   static void pairlEstringcOunsignedsPlonggR_TClassManip(TClass*);
   static void *new_pairlEstringcOunsignedsPlonggR(void *p = nullptr);
   static void *newArray_pairlEstringcOunsignedsPlonggR(Long_t size, void *p);
   static void delete_pairlEstringcOunsignedsPlonggR(void *p);
   static void deleteArray_pairlEstringcOunsignedsPlonggR(void *p);
   static void destruct_pairlEstringcOunsignedsPlonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const pair<string,unsigned long>*)
   {
      pair<string,unsigned long> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(pair<string,unsigned long>));
      static ::ROOT::TGenericClassInfo 
         instance("pair<string,unsigned long>", "string", 211,
                  typeid(pair<string,unsigned long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &pairlEstringcOunsignedsPlonggR_Dictionary, isa_proxy, 4,
                  sizeof(pair<string,unsigned long>) );
      instance.SetNew(&new_pairlEstringcOunsignedsPlonggR);
      instance.SetNewArray(&newArray_pairlEstringcOunsignedsPlonggR);
      instance.SetDelete(&delete_pairlEstringcOunsignedsPlonggR);
      instance.SetDeleteArray(&deleteArray_pairlEstringcOunsignedsPlonggR);
      instance.SetDestructor(&destruct_pairlEstringcOunsignedsPlonggR);

      ::ROOT::AddClassAlternate("pair<string,unsigned long>","std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, unsigned long>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const pair<string,unsigned long>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *pairlEstringcOunsignedsPlonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const pair<string,unsigned long>*)nullptr)->GetClass();
      pairlEstringcOunsignedsPlonggR_TClassManip(theClass);
   return theClass;
   }

   static void pairlEstringcOunsignedsPlonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *greaterlEintgR_Dictionary();
   static void greaterlEintgR_TClassManip(TClass*);
   static void *new_greaterlEintgR(void *p = nullptr);
   static void *newArray_greaterlEintgR(Long_t size, void *p);
   static void delete_greaterlEintgR(void *p);
   static void deleteArray_greaterlEintgR(void *p);
   static void destruct_greaterlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const greater<int>*)
   {
      greater<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(greater<int>));
      static ::ROOT::TGenericClassInfo 
         instance("greater<int>", "string", 371,
                  typeid(greater<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &greaterlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(greater<int>) );
      instance.SetNew(&new_greaterlEintgR);
      instance.SetNewArray(&newArray_greaterlEintgR);
      instance.SetDelete(&delete_greaterlEintgR);
      instance.SetDeleteArray(&deleteArray_greaterlEintgR);
      instance.SetDestructor(&destruct_greaterlEintgR);

      ::ROOT::AddClassAlternate("greater<int>","std::greater<int>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const greater<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *greaterlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const greater<int>*)nullptr)->GetClass();
      greaterlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void greaterlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *lesslEintgR_Dictionary();
   static void lesslEintgR_TClassManip(TClass*);
   static void *new_lesslEintgR(void *p = nullptr);
   static void *newArray_lesslEintgR(Long_t size, void *p);
   static void delete_lesslEintgR(void *p);
   static void deleteArray_lesslEintgR(void *p);
   static void destruct_lesslEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const less<int>*)
   {
      less<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(less<int>));
      static ::ROOT::TGenericClassInfo 
         instance("less<int>", "string", 381,
                  typeid(less<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &lesslEintgR_Dictionary, isa_proxy, 4,
                  sizeof(less<int>) );
      instance.SetNew(&new_lesslEintgR);
      instance.SetNewArray(&newArray_lesslEintgR);
      instance.SetDelete(&delete_lesslEintgR);
      instance.SetDeleteArray(&deleteArray_lesslEintgR);
      instance.SetDestructor(&destruct_lesslEintgR);

      ::ROOT::AddClassAlternate("less<int>","std::less<int>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const less<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *lesslEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const less<int>*)nullptr)->GetClass();
      lesslEintgR_TClassManip(theClass);
   return theClass;
   }

   static void lesslEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLFlow_Dictionary();
   static void HepMCcLcLFlow_TClassManip(TClass*);
   static void *new_HepMCcLcLFlow(void *p = nullptr);
   static void *newArray_HepMCcLcLFlow(Long_t size, void *p);
   static void delete_HepMCcLcLFlow(void *p);
   static void deleteArray_HepMCcLcLFlow(void *p);
   static void destruct_HepMCcLcLFlow(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::Flow*)
   {
      ::HepMC::Flow *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::Flow));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::Flow", "HepMC/Flow.h", 66,
                  typeid(::HepMC::Flow), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLFlow_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::Flow) );
      instance.SetNew(&new_HepMCcLcLFlow);
      instance.SetNewArray(&newArray_HepMCcLcLFlow);
      instance.SetDelete(&delete_HepMCcLcLFlow);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLFlow);
      instance.SetDestructor(&destruct_HepMCcLcLFlow);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::Flow*)
   {
      return GenerateInitInstanceLocal((::HepMC::Flow*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::Flow*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLFlow_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::Flow*)nullptr)->GetClass();
      HepMCcLcLFlow_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLFlow_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenCrossSection_Dictionary();
   static void HepMCcLcLGenCrossSection_TClassManip(TClass*);
   static void *new_HepMCcLcLGenCrossSection(void *p = nullptr);
   static void *newArray_HepMCcLcLGenCrossSection(Long_t size, void *p);
   static void delete_HepMCcLcLGenCrossSection(void *p);
   static void deleteArray_HepMCcLcLGenCrossSection(void *p);
   static void destruct_HepMCcLcLGenCrossSection(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenCrossSection*)
   {
      ::HepMC::GenCrossSection *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenCrossSection));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenCrossSection", "HepMC/GenCrossSection.h", 32,
                  typeid(::HepMC::GenCrossSection), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenCrossSection_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenCrossSection) );
      instance.SetNew(&new_HepMCcLcLGenCrossSection);
      instance.SetNewArray(&newArray_HepMCcLcLGenCrossSection);
      instance.SetDelete(&delete_HepMCcLcLGenCrossSection);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenCrossSection);
      instance.SetDestructor(&destruct_HepMCcLcLGenCrossSection);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenCrossSection*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenCrossSection*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenCrossSection*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenCrossSection_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenCrossSection*)nullptr)->GetClass();
      HepMCcLcLGenCrossSection_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenCrossSection_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLWeightContainer_Dictionary();
   static void HepMCcLcLWeightContainer_TClassManip(TClass*);
   static void *new_HepMCcLcLWeightContainer(void *p = nullptr);
   static void *newArray_HepMCcLcLWeightContainer(Long_t size, void *p);
   static void delete_HepMCcLcLWeightContainer(void *p);
   static void deleteArray_HepMCcLcLWeightContainer(void *p);
   static void destruct_HepMCcLcLWeightContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::WeightContainer*)
   {
      ::HepMC::WeightContainer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::WeightContainer));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::WeightContainer", "HepMC/WeightContainer.h", 30,
                  typeid(::HepMC::WeightContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLWeightContainer_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::WeightContainer) );
      instance.SetNew(&new_HepMCcLcLWeightContainer);
      instance.SetNewArray(&newArray_HepMCcLcLWeightContainer);
      instance.SetDelete(&delete_HepMCcLcLWeightContainer);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLWeightContainer);
      instance.SetDestructor(&destruct_HepMCcLcLWeightContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::WeightContainer*)
   {
      return GenerateInitInstanceLocal((::HepMC::WeightContainer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::WeightContainer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLWeightContainer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::WeightContainer*)nullptr)->GetClass();
      HepMCcLcLWeightContainer_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLWeightContainer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLFourVector_Dictionary();
   static void HepMCcLcLFourVector_TClassManip(TClass*);
   static void *new_HepMCcLcLFourVector(void *p = nullptr);
   static void *newArray_HepMCcLcLFourVector(Long_t size, void *p);
   static void delete_HepMCcLcLFourVector(void *p);
   static void deleteArray_HepMCcLcLFourVector(void *p);
   static void destruct_HepMCcLcLFourVector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::FourVector*)
   {
      ::HepMC::FourVector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::FourVector));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::FourVector", "HepMC/SimpleVector.h", 42,
                  typeid(::HepMC::FourVector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLFourVector_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::FourVector) );
      instance.SetNew(&new_HepMCcLcLFourVector);
      instance.SetNewArray(&newArray_HepMCcLcLFourVector);
      instance.SetDelete(&delete_HepMCcLcLFourVector);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLFourVector);
      instance.SetDestructor(&destruct_HepMCcLcLFourVector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::FourVector*)
   {
      return GenerateInitInstanceLocal((::HepMC::FourVector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::FourVector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLFourVector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::FourVector*)nullptr)->GetClass();
      HepMCcLcLFourVector_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLFourVector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLThreeVector_Dictionary();
   static void HepMCcLcLThreeVector_TClassManip(TClass*);
   static void *new_HepMCcLcLThreeVector(void *p = nullptr);
   static void *newArray_HepMCcLcLThreeVector(Long_t size, void *p);
   static void delete_HepMCcLcLThreeVector(void *p);
   static void deleteArray_HepMCcLcLThreeVector(void *p);
   static void destruct_HepMCcLcLThreeVector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::ThreeVector*)
   {
      ::HepMC::ThreeVector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::ThreeVector));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::ThreeVector", "HepMC/SimpleVector.h", 131,
                  typeid(::HepMC::ThreeVector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLThreeVector_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::ThreeVector) );
      instance.SetNew(&new_HepMCcLcLThreeVector);
      instance.SetNewArray(&newArray_HepMCcLcLThreeVector);
      instance.SetDelete(&delete_HepMCcLcLThreeVector);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLThreeVector);
      instance.SetDestructor(&destruct_HepMCcLcLThreeVector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::ThreeVector*)
   {
      return GenerateInitInstanceLocal((::HepMC::ThreeVector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::ThreeVector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLThreeVector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::ThreeVector*)nullptr)->GetClass();
      HepMCcLcLThreeVector_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLThreeVector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenVertex_Dictionary();
   static void HepMCcLcLGenVertex_TClassManip(TClass*);
   static void *new_HepMCcLcLGenVertex(void *p = nullptr);
   static void *newArray_HepMCcLcLGenVertex(Long_t size, void *p);
   static void delete_HepMCcLcLGenVertex(void *p);
   static void deleteArray_HepMCcLcLGenVertex(void *p);
   static void destruct_HepMCcLcLGenVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenVertex*)
   {
      ::HepMC::GenVertex *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenVertex));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenVertex", "HepMC/GenVertex.h", 52,
                  typeid(::HepMC::GenVertex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenVertex_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenVertex) );
      instance.SetNew(&new_HepMCcLcLGenVertex);
      instance.SetNewArray(&newArray_HepMCcLcLGenVertex);
      instance.SetDelete(&delete_HepMCcLcLGenVertex);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenVertex);
      instance.SetDestructor(&destruct_HepMCcLcLGenVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenVertex*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenVertex*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenVertex*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenVertex_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenVertex*)nullptr)->GetClass();
      HepMCcLcLGenVertex_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenVertex_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLPolarization_Dictionary();
   static void HepMCcLcLPolarization_TClassManip(TClass*);
   static void *new_HepMCcLcLPolarization(void *p = nullptr);
   static void *newArray_HepMCcLcLPolarization(Long_t size, void *p);
   static void delete_HepMCcLcLPolarization(void *p);
   static void deleteArray_HepMCcLcLPolarization(void *p);
   static void destruct_HepMCcLcLPolarization(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::Polarization*)
   {
      ::HepMC::Polarization *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::Polarization));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::Polarization", "HepMC/Polarization.h", 29,
                  typeid(::HepMC::Polarization), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLPolarization_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::Polarization) );
      instance.SetNew(&new_HepMCcLcLPolarization);
      instance.SetNewArray(&newArray_HepMCcLcLPolarization);
      instance.SetDelete(&delete_HepMCcLcLPolarization);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLPolarization);
      instance.SetDestructor(&destruct_HepMCcLcLPolarization);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::Polarization*)
   {
      return GenerateInitInstanceLocal((::HepMC::Polarization*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::Polarization*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLPolarization_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::Polarization*)nullptr)->GetClass();
      HepMCcLcLPolarization_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLPolarization_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenParticle_Dictionary();
   static void HepMCcLcLGenParticle_TClassManip(TClass*);
   static void *new_HepMCcLcLGenParticle(void *p = nullptr);
   static void *newArray_HepMCcLcLGenParticle(Long_t size, void *p);
   static void delete_HepMCcLcLGenParticle(void *p);
   static void deleteArray_HepMCcLcLGenParticle(void *p);
   static void destruct_HepMCcLcLGenParticle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenParticle*)
   {
      ::HepMC::GenParticle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenParticle));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenParticle", "HepMC/GenParticle.h", 60,
                  typeid(::HepMC::GenParticle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenParticle_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenParticle) );
      instance.SetNew(&new_HepMCcLcLGenParticle);
      instance.SetNewArray(&newArray_HepMCcLcLGenParticle);
      instance.SetDelete(&delete_HepMCcLcLGenParticle);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenParticle);
      instance.SetDestructor(&destruct_HepMCcLcLGenParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenParticle*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenParticle*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenParticle*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenParticle_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenParticle*)nullptr)->GetClass();
      HepMCcLcLGenParticle_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenParticle_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLHeavyIon_Dictionary();
   static void HepMCcLcLHeavyIon_TClassManip(TClass*);
   static void *new_HepMCcLcLHeavyIon(void *p = nullptr);
   static void *newArray_HepMCcLcLHeavyIon(Long_t size, void *p);
   static void delete_HepMCcLcLHeavyIon(void *p);
   static void deleteArray_HepMCcLcLHeavyIon(void *p);
   static void destruct_HepMCcLcLHeavyIon(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::HeavyIon*)
   {
      ::HepMC::HeavyIon *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::HeavyIon));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::HeavyIon", "HepMC/HeavyIon.h", 51,
                  typeid(::HepMC::HeavyIon), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLHeavyIon_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::HeavyIon) );
      instance.SetNew(&new_HepMCcLcLHeavyIon);
      instance.SetNewArray(&newArray_HepMCcLcLHeavyIon);
      instance.SetDelete(&delete_HepMCcLcLHeavyIon);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLHeavyIon);
      instance.SetDestructor(&destruct_HepMCcLcLHeavyIon);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::HeavyIon*)
   {
      return GenerateInitInstanceLocal((::HepMC::HeavyIon*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::HeavyIon*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLHeavyIon_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::HeavyIon*)nullptr)->GetClass();
      HepMCcLcLHeavyIon_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLHeavyIon_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLPdfInfo_Dictionary();
   static void HepMCcLcLPdfInfo_TClassManip(TClass*);
   static void *new_HepMCcLcLPdfInfo(void *p = nullptr);
   static void *newArray_HepMCcLcLPdfInfo(Long_t size, void *p);
   static void delete_HepMCcLcLPdfInfo(void *p);
   static void deleteArray_HepMCcLcLPdfInfo(void *p);
   static void destruct_HepMCcLcLPdfInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::PdfInfo*)
   {
      ::HepMC::PdfInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::PdfInfo));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::PdfInfo", "HepMC/PdfInfo.h", 37,
                  typeid(::HepMC::PdfInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLPdfInfo_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::PdfInfo) );
      instance.SetNew(&new_HepMCcLcLPdfInfo);
      instance.SetNewArray(&newArray_HepMCcLcLPdfInfo);
      instance.SetDelete(&delete_HepMCcLcLPdfInfo);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLPdfInfo);
      instance.SetDestructor(&destruct_HepMCcLcLPdfInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::PdfInfo*)
   {
      return GenerateInitInstanceLocal((::HepMC::PdfInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::PdfInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLPdfInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::PdfInfo*)nullptr)->GetClass();
      HepMCcLcLPdfInfo_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLPdfInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenEvent_Dictionary();
   static void HepMCcLcLGenEvent_TClassManip(TClass*);
   static void *new_HepMCcLcLGenEvent(void *p = nullptr);
   static void *newArray_HepMCcLcLGenEvent(Long_t size, void *p);
   static void delete_HepMCcLcLGenEvent(void *p);
   static void deleteArray_HepMCcLcLGenEvent(void *p);
   static void destruct_HepMCcLcLGenEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenEvent*)
   {
      ::HepMC::GenEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenEvent));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenEvent", "HepMC/GenEvent.h", 155,
                  typeid(::HepMC::GenEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenEvent_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenEvent) );
      instance.SetNew(&new_HepMCcLcLGenEvent);
      instance.SetNewArray(&newArray_HepMCcLcLGenEvent);
      instance.SetDelete(&delete_HepMCcLcLGenEvent);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenEvent);
      instance.SetDestructor(&destruct_HepMCcLcLGenEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenEvent*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenEvent*)nullptr)->GetClass();
      HepMCcLcLGenEvent_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenEventVertexRange_Dictionary();
   static void HepMCcLcLGenEventVertexRange_TClassManip(TClass*);
   static void delete_HepMCcLcLGenEventVertexRange(void *p);
   static void deleteArray_HepMCcLcLGenEventVertexRange(void *p);
   static void destruct_HepMCcLcLGenEventVertexRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenEventVertexRange*)
   {
      ::HepMC::GenEventVertexRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenEventVertexRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenEventVertexRange", "HepMC/GenRanges.h", 26,
                  typeid(::HepMC::GenEventVertexRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenEventVertexRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenEventVertexRange) );
      instance.SetDelete(&delete_HepMCcLcLGenEventVertexRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenEventVertexRange);
      instance.SetDestructor(&destruct_HepMCcLcLGenEventVertexRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenEventVertexRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenEventVertexRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenEventVertexRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenEventVertexRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenEventVertexRange*)nullptr)->GetClass();
      HepMCcLcLGenEventVertexRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenEventVertexRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLConstGenEventVertexRange_Dictionary();
   static void HepMCcLcLConstGenEventVertexRange_TClassManip(TClass*);
   static void delete_HepMCcLcLConstGenEventVertexRange(void *p);
   static void deleteArray_HepMCcLcLConstGenEventVertexRange(void *p);
   static void destruct_HepMCcLcLConstGenEventVertexRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::ConstGenEventVertexRange*)
   {
      ::HepMC::ConstGenEventVertexRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::ConstGenEventVertexRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::ConstGenEventVertexRange", "HepMC/GenRanges.h", 55,
                  typeid(::HepMC::ConstGenEventVertexRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLConstGenEventVertexRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::ConstGenEventVertexRange) );
      instance.SetDelete(&delete_HepMCcLcLConstGenEventVertexRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLConstGenEventVertexRange);
      instance.SetDestructor(&destruct_HepMCcLcLConstGenEventVertexRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::ConstGenEventVertexRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::ConstGenEventVertexRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::ConstGenEventVertexRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLConstGenEventVertexRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::ConstGenEventVertexRange*)nullptr)->GetClass();
      HepMCcLcLConstGenEventVertexRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLConstGenEventVertexRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenEventParticleRange_Dictionary();
   static void HepMCcLcLGenEventParticleRange_TClassManip(TClass*);
   static void delete_HepMCcLcLGenEventParticleRange(void *p);
   static void deleteArray_HepMCcLcLGenEventParticleRange(void *p);
   static void destruct_HepMCcLcLGenEventParticleRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenEventParticleRange*)
   {
      ::HepMC::GenEventParticleRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenEventParticleRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenEventParticleRange", "HepMC/GenRanges.h", 83,
                  typeid(::HepMC::GenEventParticleRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenEventParticleRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenEventParticleRange) );
      instance.SetDelete(&delete_HepMCcLcLGenEventParticleRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenEventParticleRange);
      instance.SetDestructor(&destruct_HepMCcLcLGenEventParticleRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenEventParticleRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenEventParticleRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenEventParticleRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenEventParticleRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenEventParticleRange*)nullptr)->GetClass();
      HepMCcLcLGenEventParticleRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenEventParticleRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLConstGenEventParticleRange_Dictionary();
   static void HepMCcLcLConstGenEventParticleRange_TClassManip(TClass*);
   static void delete_HepMCcLcLConstGenEventParticleRange(void *p);
   static void deleteArray_HepMCcLcLConstGenEventParticleRange(void *p);
   static void destruct_HepMCcLcLConstGenEventParticleRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::ConstGenEventParticleRange*)
   {
      ::HepMC::ConstGenEventParticleRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::ConstGenEventParticleRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::ConstGenEventParticleRange", "HepMC/GenRanges.h", 112,
                  typeid(::HepMC::ConstGenEventParticleRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLConstGenEventParticleRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::ConstGenEventParticleRange) );
      instance.SetDelete(&delete_HepMCcLcLConstGenEventParticleRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLConstGenEventParticleRange);
      instance.SetDestructor(&destruct_HepMCcLcLConstGenEventParticleRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::ConstGenEventParticleRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::ConstGenEventParticleRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::ConstGenEventParticleRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLConstGenEventParticleRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::ConstGenEventParticleRange*)nullptr)->GetClass();
      HepMCcLcLConstGenEventParticleRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLConstGenEventParticleRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenVertexParticleRange_Dictionary();
   static void HepMCcLcLGenVertexParticleRange_TClassManip(TClass*);
   static void delete_HepMCcLcLGenVertexParticleRange(void *p);
   static void deleteArray_HepMCcLcLGenVertexParticleRange(void *p);
   static void destruct_HepMCcLcLGenVertexParticleRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenVertexParticleRange*)
   {
      ::HepMC::GenVertexParticleRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenVertexParticleRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenVertexParticleRange", "HepMC/GenRanges.h", 140,
                  typeid(::HepMC::GenVertexParticleRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenVertexParticleRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenVertexParticleRange) );
      instance.SetDelete(&delete_HepMCcLcLGenVertexParticleRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenVertexParticleRange);
      instance.SetDestructor(&destruct_HepMCcLcLGenVertexParticleRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenVertexParticleRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenVertexParticleRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenVertexParticleRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenVertexParticleRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenVertexParticleRange*)nullptr)->GetClass();
      HepMCcLcLGenVertexParticleRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenVertexParticleRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenParticleProductionRange_Dictionary();
   static void HepMCcLcLGenParticleProductionRange_TClassManip(TClass*);
   static void delete_HepMCcLcLGenParticleProductionRange(void *p);
   static void deleteArray_HepMCcLcLGenParticleProductionRange(void *p);
   static void destruct_HepMCcLcLGenParticleProductionRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenParticleProductionRange*)
   {
      ::HepMC::GenParticleProductionRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenParticleProductionRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenParticleProductionRange", "HepMC/GenRanges.h", 170,
                  typeid(::HepMC::GenParticleProductionRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenParticleProductionRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenParticleProductionRange) );
      instance.SetDelete(&delete_HepMCcLcLGenParticleProductionRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenParticleProductionRange);
      instance.SetDestructor(&destruct_HepMCcLcLGenParticleProductionRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenParticleProductionRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenParticleProductionRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenParticleProductionRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenParticleProductionRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenParticleProductionRange*)nullptr)->GetClass();
      HepMCcLcLGenParticleProductionRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenParticleProductionRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLConstGenParticleProductionRange_Dictionary();
   static void HepMCcLcLConstGenParticleProductionRange_TClassManip(TClass*);
   static void delete_HepMCcLcLConstGenParticleProductionRange(void *p);
   static void deleteArray_HepMCcLcLConstGenParticleProductionRange(void *p);
   static void destruct_HepMCcLcLConstGenParticleProductionRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::ConstGenParticleProductionRange*)
   {
      ::HepMC::ConstGenParticleProductionRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::ConstGenParticleProductionRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::ConstGenParticleProductionRange", "HepMC/GenRanges.h", 193,
                  typeid(::HepMC::ConstGenParticleProductionRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLConstGenParticleProductionRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::ConstGenParticleProductionRange) );
      instance.SetDelete(&delete_HepMCcLcLConstGenParticleProductionRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLConstGenParticleProductionRange);
      instance.SetDestructor(&destruct_HepMCcLcLConstGenParticleProductionRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::ConstGenParticleProductionRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::ConstGenParticleProductionRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::ConstGenParticleProductionRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLConstGenParticleProductionRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::ConstGenParticleProductionRange*)nullptr)->GetClass();
      HepMCcLcLConstGenParticleProductionRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLConstGenParticleProductionRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLGenParticleEndRange_Dictionary();
   static void HepMCcLcLGenParticleEndRange_TClassManip(TClass*);
   static void delete_HepMCcLcLGenParticleEndRange(void *p);
   static void deleteArray_HepMCcLcLGenParticleEndRange(void *p);
   static void destruct_HepMCcLcLGenParticleEndRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::GenParticleEndRange*)
   {
      ::HepMC::GenParticleEndRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::GenParticleEndRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::GenParticleEndRange", "HepMC/GenRanges.h", 224,
                  typeid(::HepMC::GenParticleEndRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLGenParticleEndRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::GenParticleEndRange) );
      instance.SetDelete(&delete_HepMCcLcLGenParticleEndRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLGenParticleEndRange);
      instance.SetDestructor(&destruct_HepMCcLcLGenParticleEndRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::GenParticleEndRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::GenParticleEndRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::GenParticleEndRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLGenParticleEndRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::GenParticleEndRange*)nullptr)->GetClass();
      HepMCcLcLGenParticleEndRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLGenParticleEndRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLConstGenParticleEndRange_Dictionary();
   static void HepMCcLcLConstGenParticleEndRange_TClassManip(TClass*);
   static void delete_HepMCcLcLConstGenParticleEndRange(void *p);
   static void deleteArray_HepMCcLcLConstGenParticleEndRange(void *p);
   static void destruct_HepMCcLcLConstGenParticleEndRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::ConstGenParticleEndRange*)
   {
      ::HepMC::ConstGenParticleEndRange *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::ConstGenParticleEndRange));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::ConstGenParticleEndRange", "HepMC/GenRanges.h", 247,
                  typeid(::HepMC::ConstGenParticleEndRange), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLConstGenParticleEndRange_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::ConstGenParticleEndRange) );
      instance.SetDelete(&delete_HepMCcLcLConstGenParticleEndRange);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLConstGenParticleEndRange);
      instance.SetDestructor(&destruct_HepMCcLcLConstGenParticleEndRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::ConstGenParticleEndRange*)
   {
      return GenerateInitInstanceLocal((::HepMC::ConstGenParticleEndRange*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::ConstGenParticleEndRange*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLConstGenParticleEndRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::ConstGenParticleEndRange*)nullptr)->GetClass();
      HepMCcLcLConstGenParticleEndRange_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLConstGenParticleEndRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLIO_BaseClass_Dictionary();
   static void HepMCcLcLIO_BaseClass_TClassManip(TClass*);
   static void delete_HepMCcLcLIO_BaseClass(void *p);
   static void deleteArray_HepMCcLcLIO_BaseClass(void *p);
   static void destruct_HepMCcLcLIO_BaseClass(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::IO_BaseClass*)
   {
      ::HepMC::IO_BaseClass *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::IO_BaseClass));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::IO_BaseClass", "HepMC/IO_BaseClass.h", 34,
                  typeid(::HepMC::IO_BaseClass), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLIO_BaseClass_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::IO_BaseClass) );
      instance.SetDelete(&delete_HepMCcLcLIO_BaseClass);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLIO_BaseClass);
      instance.SetDestructor(&destruct_HepMCcLcLIO_BaseClass);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::IO_BaseClass*)
   {
      return GenerateInitInstanceLocal((::HepMC::IO_BaseClass*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::IO_BaseClass*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLIO_BaseClass_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::IO_BaseClass*)nullptr)->GetClass();
      HepMCcLcLIO_BaseClass_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLIO_BaseClass_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLIO_AsciiParticles_Dictionary();
   static void HepMCcLcLIO_AsciiParticles_TClassManip(TClass*);
   static void *new_HepMCcLcLIO_AsciiParticles(void *p = nullptr);
   static void *newArray_HepMCcLcLIO_AsciiParticles(Long_t size, void *p);
   static void delete_HepMCcLcLIO_AsciiParticles(void *p);
   static void deleteArray_HepMCcLcLIO_AsciiParticles(void *p);
   static void destruct_HepMCcLcLIO_AsciiParticles(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::IO_AsciiParticles*)
   {
      ::HepMC::IO_AsciiParticles *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::IO_AsciiParticles));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::IO_AsciiParticles", "HepMC/IO_AsciiParticles.h", 54,
                  typeid(::HepMC::IO_AsciiParticles), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLIO_AsciiParticles_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::IO_AsciiParticles) );
      instance.SetNew(&new_HepMCcLcLIO_AsciiParticles);
      instance.SetNewArray(&newArray_HepMCcLcLIO_AsciiParticles);
      instance.SetDelete(&delete_HepMCcLcLIO_AsciiParticles);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLIO_AsciiParticles);
      instance.SetDestructor(&destruct_HepMCcLcLIO_AsciiParticles);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::IO_AsciiParticles*)
   {
      return GenerateInitInstanceLocal((::HepMC::IO_AsciiParticles*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::IO_AsciiParticles*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLIO_AsciiParticles_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::IO_AsciiParticles*)nullptr)->GetClass();
      HepMCcLcLIO_AsciiParticles_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLIO_AsciiParticles_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLIO_Exception_Dictionary();
   static void HepMCcLcLIO_Exception_TClassManip(TClass*);
   static void delete_HepMCcLcLIO_Exception(void *p);
   static void deleteArray_HepMCcLcLIO_Exception(void *p);
   static void destruct_HepMCcLcLIO_Exception(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::IO_Exception*)
   {
      ::HepMC::IO_Exception *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::IO_Exception));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::IO_Exception", "HepMC/IO_Exception.h", 28,
                  typeid(::HepMC::IO_Exception), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLIO_Exception_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::IO_Exception) );
      instance.SetDelete(&delete_HepMCcLcLIO_Exception);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLIO_Exception);
      instance.SetDestructor(&destruct_HepMCcLcLIO_Exception);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::IO_Exception*)
   {
      return GenerateInitInstanceLocal((::HepMC::IO_Exception*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::IO_Exception*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLIO_Exception_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::IO_Exception*)nullptr)->GetClass();
      HepMCcLcLIO_Exception_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLIO_Exception_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLIO_GenEvent_Dictionary();
   static void HepMCcLcLIO_GenEvent_TClassManip(TClass*);
   static void *new_HepMCcLcLIO_GenEvent(void *p = nullptr);
   static void *newArray_HepMCcLcLIO_GenEvent(Long_t size, void *p);
   static void delete_HepMCcLcLIO_GenEvent(void *p);
   static void deleteArray_HepMCcLcLIO_GenEvent(void *p);
   static void destruct_HepMCcLcLIO_GenEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::IO_GenEvent*)
   {
      ::HepMC::IO_GenEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::IO_GenEvent));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::IO_GenEvent", "HepMC/IO_GenEvent.h", 63,
                  typeid(::HepMC::IO_GenEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLIO_GenEvent_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::IO_GenEvent) );
      instance.SetNew(&new_HepMCcLcLIO_GenEvent);
      instance.SetNewArray(&newArray_HepMCcLcLIO_GenEvent);
      instance.SetDelete(&delete_HepMCcLcLIO_GenEvent);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLIO_GenEvent);
      instance.SetDestructor(&destruct_HepMCcLcLIO_GenEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::IO_GenEvent*)
   {
      return GenerateInitInstanceLocal((::HepMC::IO_GenEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::IO_GenEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLIO_GenEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::IO_GenEvent*)nullptr)->GetClass();
      HepMCcLcLIO_GenEvent_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLIO_GenEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLStreamInfo_Dictionary();
   static void HepMCcLcLStreamInfo_TClassManip(TClass*);
   static void *new_HepMCcLcLStreamInfo(void *p = nullptr);
   static void *newArray_HepMCcLcLStreamInfo(Long_t size, void *p);
   static void delete_HepMCcLcLStreamInfo(void *p);
   static void deleteArray_HepMCcLcLStreamInfo(void *p);
   static void destruct_HepMCcLcLStreamInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::StreamInfo*)
   {
      ::HepMC::StreamInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::StreamInfo));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::StreamInfo", "HepMC/StreamInfo.h", 26,
                  typeid(::HepMC::StreamInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLStreamInfo_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::StreamInfo) );
      instance.SetNew(&new_HepMCcLcLStreamInfo);
      instance.SetNewArray(&newArray_HepMCcLcLStreamInfo);
      instance.SetDelete(&delete_HepMCcLcLStreamInfo);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLStreamInfo);
      instance.SetDestructor(&destruct_HepMCcLcLStreamInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::StreamInfo*)
   {
      return GenerateInitInstanceLocal((::HepMC::StreamInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::StreamInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLStreamInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::StreamInfo*)nullptr)->GetClass();
      HepMCcLcLStreamInfo_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLStreamInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *HepMCcLcLTempParticleMap_Dictionary();
   static void HepMCcLcLTempParticleMap_TClassManip(TClass*);
   static void *new_HepMCcLcLTempParticleMap(void *p = nullptr);
   static void *newArray_HepMCcLcLTempParticleMap(Long_t size, void *p);
   static void delete_HepMCcLcLTempParticleMap(void *p);
   static void deleteArray_HepMCcLcLTempParticleMap(void *p);
   static void destruct_HepMCcLcLTempParticleMap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HepMC::TempParticleMap*)
   {
      ::HepMC::TempParticleMap *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HepMC::TempParticleMap));
      static ::ROOT::TGenericClassInfo 
         instance("HepMC::TempParticleMap", "HepMC/TempParticleMap.h", 24,
                  typeid(::HepMC::TempParticleMap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &HepMCcLcLTempParticleMap_Dictionary, isa_proxy, 4,
                  sizeof(::HepMC::TempParticleMap) );
      instance.SetNew(&new_HepMCcLcLTempParticleMap);
      instance.SetNewArray(&newArray_HepMCcLcLTempParticleMap);
      instance.SetDelete(&delete_HepMCcLcLTempParticleMap);
      instance.SetDeleteArray(&deleteArray_HepMCcLcLTempParticleMap);
      instance.SetDestructor(&destruct_HepMCcLcLTempParticleMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HepMC::TempParticleMap*)
   {
      return GenerateInitInstanceLocal((::HepMC::TempParticleMap*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HepMC::TempParticleMap*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *HepMCcLcLTempParticleMap_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::HepMC::TempParticleMap*)nullptr)->GetClass();
      HepMCcLcLTempParticleMap_TClassManip(theClass);
   return theClass;
   }

   static void HepMCcLcLTempParticleMap_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_pairlEintcOHepMCcLcLGenParticlemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<int,HepMC::GenParticle*> : new pair<int,HepMC::GenParticle*>;
   }
   static void *newArray_pairlEintcOHepMCcLcLGenParticlemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<int,HepMC::GenParticle*>[nElements] : new pair<int,HepMC::GenParticle*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_pairlEintcOHepMCcLcLGenParticlemUgR(void *p) {
      delete ((pair<int,HepMC::GenParticle*>*)p);
   }
   static void deleteArray_pairlEintcOHepMCcLcLGenParticlemUgR(void *p) {
      delete [] ((pair<int,HepMC::GenParticle*>*)p);
   }
   static void destruct_pairlEintcOHepMCcLcLGenParticlemUgR(void *p) {
      typedef pair<int,HepMC::GenParticle*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class pair<int,HepMC::GenParticle*>

namespace ROOT {
   // Wrappers around operator new
   static void *new_pairlEintcOHepMCcLcLGenVertexmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<int,HepMC::GenVertex*> : new pair<int,HepMC::GenVertex*>;
   }
   static void *newArray_pairlEintcOHepMCcLcLGenVertexmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<int,HepMC::GenVertex*>[nElements] : new pair<int,HepMC::GenVertex*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_pairlEintcOHepMCcLcLGenVertexmUgR(void *p) {
      delete ((pair<int,HepMC::GenVertex*>*)p);
   }
   static void deleteArray_pairlEintcOHepMCcLcLGenVertexmUgR(void *p) {
      delete [] ((pair<int,HepMC::GenVertex*>*)p);
   }
   static void destruct_pairlEintcOHepMCcLcLGenVertexmUgR(void *p) {
      typedef pair<int,HepMC::GenVertex*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class pair<int,HepMC::GenVertex*>

namespace ROOT {
   // Wrappers around operator new
   static void *new_pairlEstringcOunsignedsPlonggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<string,unsigned long> : new pair<string,unsigned long>;
   }
   static void *newArray_pairlEstringcOunsignedsPlonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<string,unsigned long>[nElements] : new pair<string,unsigned long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_pairlEstringcOunsignedsPlonggR(void *p) {
      delete ((pair<string,unsigned long>*)p);
   }
   static void deleteArray_pairlEstringcOunsignedsPlonggR(void *p) {
      delete [] ((pair<string,unsigned long>*)p);
   }
   static void destruct_pairlEstringcOunsignedsPlonggR(void *p) {
      typedef pair<string,unsigned long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class pair<string,unsigned long>

namespace ROOT {
   // Wrappers around operator new
   static void *new_greaterlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) greater<int> : new greater<int>;
   }
   static void *newArray_greaterlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) greater<int>[nElements] : new greater<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_greaterlEintgR(void *p) {
      delete ((greater<int>*)p);
   }
   static void deleteArray_greaterlEintgR(void *p) {
      delete [] ((greater<int>*)p);
   }
   static void destruct_greaterlEintgR(void *p) {
      typedef greater<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class greater<int>

namespace ROOT {
   // Wrappers around operator new
   static void *new_lesslEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) less<int> : new less<int>;
   }
   static void *newArray_lesslEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) less<int>[nElements] : new less<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_lesslEintgR(void *p) {
      delete ((less<int>*)p);
   }
   static void deleteArray_lesslEintgR(void *p) {
      delete [] ((less<int>*)p);
   }
   static void destruct_lesslEintgR(void *p) {
      typedef less<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class less<int>

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLFlow(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::Flow : new ::HepMC::Flow;
   }
   static void *newArray_HepMCcLcLFlow(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::Flow[nElements] : new ::HepMC::Flow[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLFlow(void *p) {
      delete ((::HepMC::Flow*)p);
   }
   static void deleteArray_HepMCcLcLFlow(void *p) {
      delete [] ((::HepMC::Flow*)p);
   }
   static void destruct_HepMCcLcLFlow(void *p) {
      typedef ::HepMC::Flow current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::Flow

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLGenCrossSection(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenCrossSection : new ::HepMC::GenCrossSection;
   }
   static void *newArray_HepMCcLcLGenCrossSection(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenCrossSection[nElements] : new ::HepMC::GenCrossSection[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenCrossSection(void *p) {
      delete ((::HepMC::GenCrossSection*)p);
   }
   static void deleteArray_HepMCcLcLGenCrossSection(void *p) {
      delete [] ((::HepMC::GenCrossSection*)p);
   }
   static void destruct_HepMCcLcLGenCrossSection(void *p) {
      typedef ::HepMC::GenCrossSection current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenCrossSection

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLWeightContainer(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::WeightContainer : new ::HepMC::WeightContainer;
   }
   static void *newArray_HepMCcLcLWeightContainer(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::WeightContainer[nElements] : new ::HepMC::WeightContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLWeightContainer(void *p) {
      delete ((::HepMC::WeightContainer*)p);
   }
   static void deleteArray_HepMCcLcLWeightContainer(void *p) {
      delete [] ((::HepMC::WeightContainer*)p);
   }
   static void destruct_HepMCcLcLWeightContainer(void *p) {
      typedef ::HepMC::WeightContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::WeightContainer

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLFourVector(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::FourVector : new ::HepMC::FourVector;
   }
   static void *newArray_HepMCcLcLFourVector(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::FourVector[nElements] : new ::HepMC::FourVector[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLFourVector(void *p) {
      delete ((::HepMC::FourVector*)p);
   }
   static void deleteArray_HepMCcLcLFourVector(void *p) {
      delete [] ((::HepMC::FourVector*)p);
   }
   static void destruct_HepMCcLcLFourVector(void *p) {
      typedef ::HepMC::FourVector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::FourVector

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLThreeVector(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::ThreeVector : new ::HepMC::ThreeVector;
   }
   static void *newArray_HepMCcLcLThreeVector(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::ThreeVector[nElements] : new ::HepMC::ThreeVector[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLThreeVector(void *p) {
      delete ((::HepMC::ThreeVector*)p);
   }
   static void deleteArray_HepMCcLcLThreeVector(void *p) {
      delete [] ((::HepMC::ThreeVector*)p);
   }
   static void destruct_HepMCcLcLThreeVector(void *p) {
      typedef ::HepMC::ThreeVector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::ThreeVector

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLGenVertex(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenVertex : new ::HepMC::GenVertex;
   }
   static void *newArray_HepMCcLcLGenVertex(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenVertex[nElements] : new ::HepMC::GenVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenVertex(void *p) {
      delete ((::HepMC::GenVertex*)p);
   }
   static void deleteArray_HepMCcLcLGenVertex(void *p) {
      delete [] ((::HepMC::GenVertex*)p);
   }
   static void destruct_HepMCcLcLGenVertex(void *p) {
      typedef ::HepMC::GenVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenVertex

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLPolarization(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::Polarization : new ::HepMC::Polarization;
   }
   static void *newArray_HepMCcLcLPolarization(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::Polarization[nElements] : new ::HepMC::Polarization[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLPolarization(void *p) {
      delete ((::HepMC::Polarization*)p);
   }
   static void deleteArray_HepMCcLcLPolarization(void *p) {
      delete [] ((::HepMC::Polarization*)p);
   }
   static void destruct_HepMCcLcLPolarization(void *p) {
      typedef ::HepMC::Polarization current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::Polarization

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLGenParticle(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenParticle : new ::HepMC::GenParticle;
   }
   static void *newArray_HepMCcLcLGenParticle(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenParticle[nElements] : new ::HepMC::GenParticle[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenParticle(void *p) {
      delete ((::HepMC::GenParticle*)p);
   }
   static void deleteArray_HepMCcLcLGenParticle(void *p) {
      delete [] ((::HepMC::GenParticle*)p);
   }
   static void destruct_HepMCcLcLGenParticle(void *p) {
      typedef ::HepMC::GenParticle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenParticle

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLHeavyIon(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::HeavyIon : new ::HepMC::HeavyIon;
   }
   static void *newArray_HepMCcLcLHeavyIon(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::HeavyIon[nElements] : new ::HepMC::HeavyIon[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLHeavyIon(void *p) {
      delete ((::HepMC::HeavyIon*)p);
   }
   static void deleteArray_HepMCcLcLHeavyIon(void *p) {
      delete [] ((::HepMC::HeavyIon*)p);
   }
   static void destruct_HepMCcLcLHeavyIon(void *p) {
      typedef ::HepMC::HeavyIon current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::HeavyIon

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLPdfInfo(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::PdfInfo : new ::HepMC::PdfInfo;
   }
   static void *newArray_HepMCcLcLPdfInfo(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::PdfInfo[nElements] : new ::HepMC::PdfInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLPdfInfo(void *p) {
      delete ((::HepMC::PdfInfo*)p);
   }
   static void deleteArray_HepMCcLcLPdfInfo(void *p) {
      delete [] ((::HepMC::PdfInfo*)p);
   }
   static void destruct_HepMCcLcLPdfInfo(void *p) {
      typedef ::HepMC::PdfInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::PdfInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLGenEvent(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenEvent : new ::HepMC::GenEvent;
   }
   static void *newArray_HepMCcLcLGenEvent(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::GenEvent[nElements] : new ::HepMC::GenEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenEvent(void *p) {
      delete ((::HepMC::GenEvent*)p);
   }
   static void deleteArray_HepMCcLcLGenEvent(void *p) {
      delete [] ((::HepMC::GenEvent*)p);
   }
   static void destruct_HepMCcLcLGenEvent(void *p) {
      typedef ::HepMC::GenEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenEvent

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenEventVertexRange(void *p) {
      delete ((::HepMC::GenEventVertexRange*)p);
   }
   static void deleteArray_HepMCcLcLGenEventVertexRange(void *p) {
      delete [] ((::HepMC::GenEventVertexRange*)p);
   }
   static void destruct_HepMCcLcLGenEventVertexRange(void *p) {
      typedef ::HepMC::GenEventVertexRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenEventVertexRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLConstGenEventVertexRange(void *p) {
      delete ((::HepMC::ConstGenEventVertexRange*)p);
   }
   static void deleteArray_HepMCcLcLConstGenEventVertexRange(void *p) {
      delete [] ((::HepMC::ConstGenEventVertexRange*)p);
   }
   static void destruct_HepMCcLcLConstGenEventVertexRange(void *p) {
      typedef ::HepMC::ConstGenEventVertexRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::ConstGenEventVertexRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenEventParticleRange(void *p) {
      delete ((::HepMC::GenEventParticleRange*)p);
   }
   static void deleteArray_HepMCcLcLGenEventParticleRange(void *p) {
      delete [] ((::HepMC::GenEventParticleRange*)p);
   }
   static void destruct_HepMCcLcLGenEventParticleRange(void *p) {
      typedef ::HepMC::GenEventParticleRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenEventParticleRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLConstGenEventParticleRange(void *p) {
      delete ((::HepMC::ConstGenEventParticleRange*)p);
   }
   static void deleteArray_HepMCcLcLConstGenEventParticleRange(void *p) {
      delete [] ((::HepMC::ConstGenEventParticleRange*)p);
   }
   static void destruct_HepMCcLcLConstGenEventParticleRange(void *p) {
      typedef ::HepMC::ConstGenEventParticleRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::ConstGenEventParticleRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenVertexParticleRange(void *p) {
      delete ((::HepMC::GenVertexParticleRange*)p);
   }
   static void deleteArray_HepMCcLcLGenVertexParticleRange(void *p) {
      delete [] ((::HepMC::GenVertexParticleRange*)p);
   }
   static void destruct_HepMCcLcLGenVertexParticleRange(void *p) {
      typedef ::HepMC::GenVertexParticleRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenVertexParticleRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenParticleProductionRange(void *p) {
      delete ((::HepMC::GenParticleProductionRange*)p);
   }
   static void deleteArray_HepMCcLcLGenParticleProductionRange(void *p) {
      delete [] ((::HepMC::GenParticleProductionRange*)p);
   }
   static void destruct_HepMCcLcLGenParticleProductionRange(void *p) {
      typedef ::HepMC::GenParticleProductionRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenParticleProductionRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLConstGenParticleProductionRange(void *p) {
      delete ((::HepMC::ConstGenParticleProductionRange*)p);
   }
   static void deleteArray_HepMCcLcLConstGenParticleProductionRange(void *p) {
      delete [] ((::HepMC::ConstGenParticleProductionRange*)p);
   }
   static void destruct_HepMCcLcLConstGenParticleProductionRange(void *p) {
      typedef ::HepMC::ConstGenParticleProductionRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::ConstGenParticleProductionRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLGenParticleEndRange(void *p) {
      delete ((::HepMC::GenParticleEndRange*)p);
   }
   static void deleteArray_HepMCcLcLGenParticleEndRange(void *p) {
      delete [] ((::HepMC::GenParticleEndRange*)p);
   }
   static void destruct_HepMCcLcLGenParticleEndRange(void *p) {
      typedef ::HepMC::GenParticleEndRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::GenParticleEndRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLConstGenParticleEndRange(void *p) {
      delete ((::HepMC::ConstGenParticleEndRange*)p);
   }
   static void deleteArray_HepMCcLcLConstGenParticleEndRange(void *p) {
      delete [] ((::HepMC::ConstGenParticleEndRange*)p);
   }
   static void destruct_HepMCcLcLConstGenParticleEndRange(void *p) {
      typedef ::HepMC::ConstGenParticleEndRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::ConstGenParticleEndRange

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLIO_BaseClass(void *p) {
      delete ((::HepMC::IO_BaseClass*)p);
   }
   static void deleteArray_HepMCcLcLIO_BaseClass(void *p) {
      delete [] ((::HepMC::IO_BaseClass*)p);
   }
   static void destruct_HepMCcLcLIO_BaseClass(void *p) {
      typedef ::HepMC::IO_BaseClass current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::IO_BaseClass

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLIO_AsciiParticles(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::IO_AsciiParticles : new ::HepMC::IO_AsciiParticles;
   }
   static void *newArray_HepMCcLcLIO_AsciiParticles(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::IO_AsciiParticles[nElements] : new ::HepMC::IO_AsciiParticles[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLIO_AsciiParticles(void *p) {
      delete ((::HepMC::IO_AsciiParticles*)p);
   }
   static void deleteArray_HepMCcLcLIO_AsciiParticles(void *p) {
      delete [] ((::HepMC::IO_AsciiParticles*)p);
   }
   static void destruct_HepMCcLcLIO_AsciiParticles(void *p) {
      typedef ::HepMC::IO_AsciiParticles current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::IO_AsciiParticles

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HepMCcLcLIO_Exception(void *p) {
      delete ((::HepMC::IO_Exception*)p);
   }
   static void deleteArray_HepMCcLcLIO_Exception(void *p) {
      delete [] ((::HepMC::IO_Exception*)p);
   }
   static void destruct_HepMCcLcLIO_Exception(void *p) {
      typedef ::HepMC::IO_Exception current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::IO_Exception

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLIO_GenEvent(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::IO_GenEvent : new ::HepMC::IO_GenEvent;
   }
   static void *newArray_HepMCcLcLIO_GenEvent(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::IO_GenEvent[nElements] : new ::HepMC::IO_GenEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLIO_GenEvent(void *p) {
      delete ((::HepMC::IO_GenEvent*)p);
   }
   static void deleteArray_HepMCcLcLIO_GenEvent(void *p) {
      delete [] ((::HepMC::IO_GenEvent*)p);
   }
   static void destruct_HepMCcLcLIO_GenEvent(void *p) {
      typedef ::HepMC::IO_GenEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::IO_GenEvent

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLStreamInfo(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::StreamInfo : new ::HepMC::StreamInfo;
   }
   static void *newArray_HepMCcLcLStreamInfo(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::StreamInfo[nElements] : new ::HepMC::StreamInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLStreamInfo(void *p) {
      delete ((::HepMC::StreamInfo*)p);
   }
   static void deleteArray_HepMCcLcLStreamInfo(void *p) {
      delete [] ((::HepMC::StreamInfo*)p);
   }
   static void destruct_HepMCcLcLStreamInfo(void *p) {
      typedef ::HepMC::StreamInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::StreamInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_HepMCcLcLTempParticleMap(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::TempParticleMap : new ::HepMC::TempParticleMap;
   }
   static void *newArray_HepMCcLcLTempParticleMap(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::HepMC::TempParticleMap[nElements] : new ::HepMC::TempParticleMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_HepMCcLcLTempParticleMap(void *p) {
      delete ((::HepMC::TempParticleMap*)p);
   }
   static void deleteArray_HepMCcLcLTempParticleMap(void *p) {
      delete [] ((::HepMC::TempParticleMap*)p);
   }
   static void destruct_HepMCcLcLTempParticleMap(void *p) {
      typedef ::HepMC::TempParticleMap current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HepMC::TempParticleMap

namespace ROOT {
   static TClass *vectorlElonggR_Dictionary();
   static void vectorlElonggR_TClassManip(TClass*);
   static void *new_vectorlElonggR(void *p = nullptr);
   static void *newArray_vectorlElonggR(Long_t size, void *p);
   static void delete_vectorlElonggR(void *p);
   static void deleteArray_vectorlElonggR(void *p);
   static void destruct_vectorlElonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<long>*)
   {
      vector<long> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<long>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<long>", -2, "vector", 389,
                  typeid(vector<long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlElonggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<long>) );
      instance.SetNew(&new_vectorlElonggR);
      instance.SetNewArray(&newArray_vectorlElonggR);
      instance.SetDelete(&delete_vectorlElonggR);
      instance.SetDeleteArray(&deleteArray_vectorlElonggR);
      instance.SetDestructor(&destruct_vectorlElonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<long> >()));

      ::ROOT::AddClassAlternate("vector<long>","std::vector<long, std::allocator<long> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<long>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlElonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<long>*)nullptr)->GetClass();
      vectorlElonggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlElonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlElonggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<long> : new vector<long>;
   }
   static void *newArray_vectorlElonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<long>[nElements] : new vector<long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlElonggR(void *p) {
      delete ((vector<long>*)p);
   }
   static void deleteArray_vectorlElonggR(void *p) {
      delete [] ((vector<long>*)p);
   }
   static void destruct_vectorlElonggR(void *p) {
      typedef vector<long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<long>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = nullptr);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 389,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)nullptr)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEHepMCcLcLGenParticlemUgR_Dictionary();
   static void vectorlEHepMCcLcLGenParticlemUgR_TClassManip(TClass*);
   static void *new_vectorlEHepMCcLcLGenParticlemUgR(void *p = nullptr);
   static void *newArray_vectorlEHepMCcLcLGenParticlemUgR(Long_t size, void *p);
   static void delete_vectorlEHepMCcLcLGenParticlemUgR(void *p);
   static void deleteArray_vectorlEHepMCcLcLGenParticlemUgR(void *p);
   static void destruct_vectorlEHepMCcLcLGenParticlemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<HepMC::GenParticle*>*)
   {
      vector<HepMC::GenParticle*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<HepMC::GenParticle*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<HepMC::GenParticle*>", -2, "vector", 389,
                  typeid(vector<HepMC::GenParticle*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEHepMCcLcLGenParticlemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<HepMC::GenParticle*>) );
      instance.SetNew(&new_vectorlEHepMCcLcLGenParticlemUgR);
      instance.SetNewArray(&newArray_vectorlEHepMCcLcLGenParticlemUgR);
      instance.SetDelete(&delete_vectorlEHepMCcLcLGenParticlemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEHepMCcLcLGenParticlemUgR);
      instance.SetDestructor(&destruct_vectorlEHepMCcLcLGenParticlemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<HepMC::GenParticle*> >()));

      ::ROOT::AddClassAlternate("vector<HepMC::GenParticle*>","std::vector<HepMC::GenParticle*, std::allocator<HepMC::GenParticle*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<HepMC::GenParticle*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEHepMCcLcLGenParticlemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<HepMC::GenParticle*>*)nullptr)->GetClass();
      vectorlEHepMCcLcLGenParticlemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEHepMCcLcLGenParticlemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEHepMCcLcLGenParticlemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HepMC::GenParticle*> : new vector<HepMC::GenParticle*>;
   }
   static void *newArray_vectorlEHepMCcLcLGenParticlemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HepMC::GenParticle*>[nElements] : new vector<HepMC::GenParticle*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEHepMCcLcLGenParticlemUgR(void *p) {
      delete ((vector<HepMC::GenParticle*>*)p);
   }
   static void deleteArray_vectorlEHepMCcLcLGenParticlemUgR(void *p) {
      delete [] ((vector<HepMC::GenParticle*>*)p);
   }
   static void destruct_vectorlEHepMCcLcLGenParticlemUgR(void *p) {
      typedef vector<HepMC::GenParticle*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<HepMC::GenParticle*>

namespace ROOT {
   static TClass *maplEstringcOunsignedsPlonggR_Dictionary();
   static void maplEstringcOunsignedsPlonggR_TClassManip(TClass*);
   static void *new_maplEstringcOunsignedsPlonggR(void *p = nullptr);
   static void *newArray_maplEstringcOunsignedsPlonggR(Long_t size, void *p);
   static void delete_maplEstringcOunsignedsPlonggR(void *p);
   static void deleteArray_maplEstringcOunsignedsPlonggR(void *p);
   static void destruct_maplEstringcOunsignedsPlonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,unsigned long>*)
   {
      map<string,unsigned long> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,unsigned long>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,unsigned long>", -2, "map", 100,
                  typeid(map<string,unsigned long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOunsignedsPlonggR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,unsigned long>) );
      instance.SetNew(&new_maplEstringcOunsignedsPlonggR);
      instance.SetNewArray(&newArray_maplEstringcOunsignedsPlonggR);
      instance.SetDelete(&delete_maplEstringcOunsignedsPlonggR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOunsignedsPlonggR);
      instance.SetDestructor(&destruct_maplEstringcOunsignedsPlonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,unsigned long> >()));

      ::ROOT::AddClassAlternate("map<string,unsigned long>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, unsigned long, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, unsigned long> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,unsigned long>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOunsignedsPlonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,unsigned long>*)nullptr)->GetClass();
      maplEstringcOunsignedsPlonggR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOunsignedsPlonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOunsignedsPlonggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,unsigned long> : new map<string,unsigned long>;
   }
   static void *newArray_maplEstringcOunsignedsPlonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,unsigned long>[nElements] : new map<string,unsigned long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOunsignedsPlonggR(void *p) {
      delete ((map<string,unsigned long>*)p);
   }
   static void deleteArray_maplEstringcOunsignedsPlonggR(void *p) {
      delete [] ((map<string,unsigned long>*)p);
   }
   static void destruct_maplEstringcOunsignedsPlonggR(void *p) {
      typedef map<string,unsigned long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,unsigned long>

namespace ROOT {
   static TClass *maplEintcOintgR_Dictionary();
   static void maplEintcOintgR_TClassManip(TClass*);
   static void *new_maplEintcOintgR(void *p = nullptr);
   static void *newArray_maplEintcOintgR(Long_t size, void *p);
   static void delete_maplEintcOintgR(void *p);
   static void deleteArray_maplEintcOintgR(void *p);
   static void destruct_maplEintcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,int>*)
   {
      map<int,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,int>", -2, "map", 100,
                  typeid(map<int,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOintgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,int>) );
      instance.SetNew(&new_maplEintcOintgR);
      instance.SetNewArray(&newArray_maplEintcOintgR);
      instance.SetDelete(&delete_maplEintcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOintgR);
      instance.SetDestructor(&destruct_maplEintcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,int> >()));

      ::ROOT::AddClassAlternate("map<int,int>","std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,int>*)nullptr)->GetClass();
      maplEintcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int> : new map<int,int>;
   }
   static void *newArray_maplEintcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int>[nElements] : new map<int,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOintgR(void *p) {
      delete ((map<int,int>*)p);
   }
   static void deleteArray_maplEintcOintgR(void *p) {
      delete [] ((map<int,int>*)p);
   }
   static void destruct_maplEintcOintgR(void *p) {
      typedef map<int,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,int>

namespace ROOT {
   static TClass *maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR_Dictionary();
   static void maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p);
   static void deleteArray_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p);
   static void destruct_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,HepMC::GenVertex*,greater<int> >*)
   {
      map<int,HepMC::GenVertex*,greater<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,HepMC::GenVertex*,greater<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,HepMC::GenVertex*,greater<int> >", -2, "map", 100,
                  typeid(map<int,HepMC::GenVertex*,greater<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,HepMC::GenVertex*,greater<int> >) );
      instance.SetNew(&new_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR);
      instance.SetDelete(&delete_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,HepMC::GenVertex*,greater<int> > >()));

      ::ROOT::AddClassAlternate("map<int,HepMC::GenVertex*,greater<int> >","std::map<int, HepMC::GenVertex*, std::greater<int>, std::allocator<std::pair<int const, HepMC::GenVertex*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,HepMC::GenVertex*,greater<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,HepMC::GenVertex*,greater<int> >*)nullptr)->GetClass();
      maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,HepMC::GenVertex*,greater<int> > : new map<int,HepMC::GenVertex*,greater<int> >;
   }
   static void *newArray_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,HepMC::GenVertex*,greater<int> >[nElements] : new map<int,HepMC::GenVertex*,greater<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p) {
      delete ((map<int,HepMC::GenVertex*,greater<int> >*)p);
   }
   static void deleteArray_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p) {
      delete [] ((map<int,HepMC::GenVertex*,greater<int> >*)p);
   }
   static void destruct_maplEintcOHepMCcLcLGenVertexmUcOgreaterlEintgRsPgR(void *p) {
      typedef map<int,HepMC::GenVertex*,greater<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,HepMC::GenVertex*,greater<int> >

namespace ROOT {
   static TClass *maplEintcOHepMCcLcLGenParticlemUgR_Dictionary();
   static void maplEintcOHepMCcLcLGenParticlemUgR_TClassManip(TClass*);
   static void *new_maplEintcOHepMCcLcLGenParticlemUgR(void *p = nullptr);
   static void *newArray_maplEintcOHepMCcLcLGenParticlemUgR(Long_t size, void *p);
   static void delete_maplEintcOHepMCcLcLGenParticlemUgR(void *p);
   static void deleteArray_maplEintcOHepMCcLcLGenParticlemUgR(void *p);
   static void destruct_maplEintcOHepMCcLcLGenParticlemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,HepMC::GenParticle*>*)
   {
      map<int,HepMC::GenParticle*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,HepMC::GenParticle*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,HepMC::GenParticle*>", -2, "map", 100,
                  typeid(map<int,HepMC::GenParticle*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOHepMCcLcLGenParticlemUgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,HepMC::GenParticle*>) );
      instance.SetNew(&new_maplEintcOHepMCcLcLGenParticlemUgR);
      instance.SetNewArray(&newArray_maplEintcOHepMCcLcLGenParticlemUgR);
      instance.SetDelete(&delete_maplEintcOHepMCcLcLGenParticlemUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOHepMCcLcLGenParticlemUgR);
      instance.SetDestructor(&destruct_maplEintcOHepMCcLcLGenParticlemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,HepMC::GenParticle*> >()));

      ::ROOT::AddClassAlternate("map<int,HepMC::GenParticle*>","std::map<int, HepMC::GenParticle*, std::less<int>, std::allocator<std::pair<int const, HepMC::GenParticle*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,HepMC::GenParticle*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOHepMCcLcLGenParticlemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,HepMC::GenParticle*>*)nullptr)->GetClass();
      maplEintcOHepMCcLcLGenParticlemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOHepMCcLcLGenParticlemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOHepMCcLcLGenParticlemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,HepMC::GenParticle*> : new map<int,HepMC::GenParticle*>;
   }
   static void *newArray_maplEintcOHepMCcLcLGenParticlemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,HepMC::GenParticle*>[nElements] : new map<int,HepMC::GenParticle*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOHepMCcLcLGenParticlemUgR(void *p) {
      delete ((map<int,HepMC::GenParticle*>*)p);
   }
   static void deleteArray_maplEintcOHepMCcLcLGenParticlemUgR(void *p) {
      delete [] ((map<int,HepMC::GenParticle*>*)p);
   }
   static void destruct_maplEintcOHepMCcLcLGenParticlemUgR(void *p) {
      typedef map<int,HepMC::GenParticle*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,HepMC::GenParticle*>

namespace ROOT {
   static TClass *maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR_Dictionary();
   static void maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR_TClassManip(TClass*);
   static void *new_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p = nullptr);
   static void *newArray_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(Long_t size, void *p);
   static void delete_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p);
   static void deleteArray_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p);
   static void destruct_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<const HepMC::GenVertex*,HepMC::GenVertex*>*)
   {
      map<const HepMC::GenVertex*,HepMC::GenVertex*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<const HepMC::GenVertex*,HepMC::GenVertex*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<const HepMC::GenVertex*,HepMC::GenVertex*>", -2, "map", 100,
                  typeid(map<const HepMC::GenVertex*,HepMC::GenVertex*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR_Dictionary, isa_proxy, 4,
                  sizeof(map<const HepMC::GenVertex*,HepMC::GenVertex*>) );
      instance.SetNew(&new_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR);
      instance.SetNewArray(&newArray_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR);
      instance.SetDelete(&delete_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR);
      instance.SetDeleteArray(&deleteArray_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR);
      instance.SetDestructor(&destruct_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<const HepMC::GenVertex*,HepMC::GenVertex*> >()));

      ::ROOT::AddClassAlternate("map<const HepMC::GenVertex*,HepMC::GenVertex*>","std::map<HepMC::GenVertex const*, HepMC::GenVertex*, std::less<HepMC::GenVertex const*>, std::allocator<std::pair<HepMC::GenVertex const* const, HepMC::GenVertex*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<const HepMC::GenVertex*,HepMC::GenVertex*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<const HepMC::GenVertex*,HepMC::GenVertex*>*)nullptr)->GetClass();
      maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<const HepMC::GenVertex*,HepMC::GenVertex*> : new map<const HepMC::GenVertex*,HepMC::GenVertex*>;
   }
   static void *newArray_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<const HepMC::GenVertex*,HepMC::GenVertex*>[nElements] : new map<const HepMC::GenVertex*,HepMC::GenVertex*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p) {
      delete ((map<const HepMC::GenVertex*,HepMC::GenVertex*>*)p);
   }
   static void deleteArray_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p) {
      delete [] ((map<const HepMC::GenVertex*,HepMC::GenVertex*>*)p);
   }
   static void destruct_maplEconstsPHepMCcLcLGenVertexmUcOHepMCcLcLGenVertexmUgR(void *p) {
      typedef map<const HepMC::GenVertex*,HepMC::GenVertex*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<const HepMC::GenVertex*,HepMC::GenVertex*>

namespace ROOT {
   static TClass *maplEHepMCcLcLGenParticlemUcOintgR_Dictionary();
   static void maplEHepMCcLcLGenParticlemUcOintgR_TClassManip(TClass*);
   static void *new_maplEHepMCcLcLGenParticlemUcOintgR(void *p = nullptr);
   static void *newArray_maplEHepMCcLcLGenParticlemUcOintgR(Long_t size, void *p);
   static void delete_maplEHepMCcLcLGenParticlemUcOintgR(void *p);
   static void deleteArray_maplEHepMCcLcLGenParticlemUcOintgR(void *p);
   static void destruct_maplEHepMCcLcLGenParticlemUcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<HepMC::GenParticle*,int>*)
   {
      map<HepMC::GenParticle*,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<HepMC::GenParticle*,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<HepMC::GenParticle*,int>", -2, "map", 100,
                  typeid(map<HepMC::GenParticle*,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEHepMCcLcLGenParticlemUcOintgR_Dictionary, isa_proxy, 4,
                  sizeof(map<HepMC::GenParticle*,int>) );
      instance.SetNew(&new_maplEHepMCcLcLGenParticlemUcOintgR);
      instance.SetNewArray(&newArray_maplEHepMCcLcLGenParticlemUcOintgR);
      instance.SetDelete(&delete_maplEHepMCcLcLGenParticlemUcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEHepMCcLcLGenParticlemUcOintgR);
      instance.SetDestructor(&destruct_maplEHepMCcLcLGenParticlemUcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<HepMC::GenParticle*,int> >()));

      ::ROOT::AddClassAlternate("map<HepMC::GenParticle*,int>","std::map<HepMC::GenParticle*, int, std::less<HepMC::GenParticle*>, std::allocator<std::pair<HepMC::GenParticle* const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<HepMC::GenParticle*,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEHepMCcLcLGenParticlemUcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<HepMC::GenParticle*,int>*)nullptr)->GetClass();
      maplEHepMCcLcLGenParticlemUcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEHepMCcLcLGenParticlemUcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEHepMCcLcLGenParticlemUcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<HepMC::GenParticle*,int> : new map<HepMC::GenParticle*,int>;
   }
   static void *newArray_maplEHepMCcLcLGenParticlemUcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<HepMC::GenParticle*,int>[nElements] : new map<HepMC::GenParticle*,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEHepMCcLcLGenParticlemUcOintgR(void *p) {
      delete ((map<HepMC::GenParticle*,int>*)p);
   }
   static void deleteArray_maplEHepMCcLcLGenParticlemUcOintgR(void *p) {
      delete [] ((map<HepMC::GenParticle*,int>*)p);
   }
   static void destruct_maplEHepMCcLcLGenParticlemUcOintgR(void *p) {
      typedef map<HepMC::GenParticle*,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<HepMC::GenParticle*,int>

namespace {
  void TriggerDictionaryInitialization_HepMCIncDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/HepMCDict/src/HepMCInc.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/HepMCDict",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/HepMCDict/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/Dict/HepMCDict",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/Dict/HepMCDict/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "HepMCIncDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenParticle.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  GenParticle;}
namespace std{template <typename _T1, typename _T2> struct __attribute__((annotate("$clingAutoload$bits/stl_iterator.h")))  __attribute__((annotate("$clingAutoload$string")))  pair;
}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenVertex.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  GenVertex;}
namespace std{template <class _CharT> struct __attribute__((annotate("$clingAutoload$bits/char_traits.h")))  __attribute__((annotate("$clingAutoload$string")))  char_traits;
}
namespace std{template <typename > class __attribute__((annotate("$clingAutoload$bits/memoryfwd.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace std{template <typename _Tp = void> struct __attribute__((annotate("$clingAutoload$bits/stl_function.h")))  __attribute__((annotate("$clingAutoload$string")))  greater;
}
namespace std{template <typename _Tp = void> struct __attribute__((annotate("$clingAutoload$bits/stl_function.h")))  __attribute__((annotate("$clingAutoload$string")))  less;
}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/Flow.h")))  Flow;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenCrossSection.h")))  GenCrossSection;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/WeightContainer.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  WeightContainer;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/SimpleVector.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  FourVector;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/SimpleVector.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  ThreeVector;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/Polarization.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  Polarization;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/HeavyIon.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  HeavyIon;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/PdfInfo.h")))  __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  PdfInfo;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenEvent.h")))  GenEvent;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  GenEventVertexRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  ConstGenEventVertexRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  GenEventParticleRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  ConstGenEventParticleRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  GenVertexParticleRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  GenParticleProductionRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  ConstGenParticleProductionRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  GenParticleEndRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/GenRanges.h")))  ConstGenParticleEndRange;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/IO_BaseClass.h")))  __attribute__((annotate("$clingAutoload$HepMC/IO_AsciiParticles.h")))  IO_BaseClass;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/IO_AsciiParticles.h")))  IO_AsciiParticles;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/IO_Exception.h")))  IO_Exception;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/IO_GenEvent.h")))  IO_GenEvent;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/StreamInfo.h")))  StreamInfo;}
namespace HepMC{class __attribute__((annotate("$clingAutoload$HepMC/TempParticleMap.h")))  TempParticleMap;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "HepMCIncDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/HepMCDict/src/HepMCInc.h"
#include <map>
#include <string>
#include <utility>
#include <functional>
#include "HepMC/Flow.h"
#include "HepMC/GenCrossSection.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenRanges.h"
#include "HepMC/GenVertex.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/IO_AsciiParticles.h"
#include "HepMC/IO_BaseClass.h"
#include "HepMC/IO_Exception.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/Polarization.h"
#include "HepMC/SimpleVector.h"
#include "HepMC/StreamInfo.h"
#include "HepMC/TempParticleMap.h"
#include "HepMC/WeightContainer.h"

using namespace HepMC;
using namespace std;
#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
typedef long size_type; // make sure size_type is defined
#pragma link C++ class std::map<string,size_type>+;
//#pragma link C++ class std::runtime_error+;
#pragma link C++ class HepMC::ConstGenEventParticleRange+;
#pragma link C++ class HepMC::ConstGenEventVertexRange+;
#pragma link C++ class HepMC::ConstGenParticleEndRange+;
#pragma link C++ class HepMC::ConstGenParticleProductionRange+;
#pragma link C++ class HepMC::Flow+;
#pragma link C++ class HepMC::FourVector+;
#pragma link C++ class HepMC::GenCrossSection+;
#pragma link C++ class HepMC::GenEvent+;
#pragma link C++ class HepMC::GenEventParticleRange+;
#pragma link C++ class HepMC::GenEventVertexRange+;
#pragma link C++ class HepMC::GenParticle+;
#pragma link C++ class HepMC::GenParticleEndRange+;
#pragma link C++ class HepMC::GenParticleProductionRange+;
#pragma link C++ class HepMC::GenVertex+;
#pragma link C++ class HepMC::GenVertexParticleRange+;
#pragma link C++ class HepMC::HeavyIon+;
//#pragma link C++ class HepMC::HEPEVT_Wrapper+;
#pragma link C++ class HepMC::IO_AsciiParticles+;
#pragma link C++ class HepMC::IO_BaseClass+;
#pragma link C++ class HepMC::IO_Exception+;
#pragma link C++ class HepMC::IO_GenEvent+;
//#pragma link C++ class HepMC::IO_HEPEVT+;
//#pragma link C++ class HepMC::IO_HERWIG+;
#pragma link C++ class HepMC::PdfInfo+;
#pragma link C++ class HepMC::Polarization+;
#pragma link C++ class HepMC::StreamInfo+;
#pragma link C++ class HepMC::TempParticleMap+;
#pragma link C++ class HepMC::ThreeVector+;
#pragma link C++ class HepMC::WeightContainer+;
#pragma link C++ class std::map<HepMC::GenParticle*,int>+;
#pragma link C++ class std::map<int,HepMC::GenParticle*>+;
#pragma link C++ class std::map<int,int>+;
#pragma link C++ class std::map<std::string,size_type>+;
#pragma link C++ class std::map< int,HepMC::GenVertex*,std::greater<int> >+;
#pragma link C++ class std::map< int,HepMC::GenParticle*,std::less<int> >+;
#pragma link C++ class std::map<const GenVertex*,GenVertex*>+;
#pragma link C++ class std::pair<string,unsigned long>+;
#pragma link C++ class std::pair<int,HepMC::GenVertex*>+;
#pragma link C++ class std::pair<int,HepMC::GenParticle*>+;
#pragma link C++ class std::greater<int>+;
#pragma link C++ class std::less<int>+;
#endif


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"HepMC::ConstGenEventParticleRange", payloadCode, "@",
"HepMC::ConstGenEventVertexRange", payloadCode, "@",
"HepMC::ConstGenParticleEndRange", payloadCode, "@",
"HepMC::ConstGenParticleProductionRange", payloadCode, "@",
"HepMC::Flow", payloadCode, "@",
"HepMC::FourVector", payloadCode, "@",
"HepMC::GenCrossSection", payloadCode, "@",
"HepMC::GenEvent", payloadCode, "@",
"HepMC::GenEventParticleRange", payloadCode, "@",
"HepMC::GenEventVertexRange", payloadCode, "@",
"HepMC::GenParticle", payloadCode, "@",
"HepMC::GenParticleEndRange", payloadCode, "@",
"HepMC::GenParticleProductionRange", payloadCode, "@",
"HepMC::GenVertex", payloadCode, "@",
"HepMC::GenVertexParticleRange", payloadCode, "@",
"HepMC::HeavyIon", payloadCode, "@",
"HepMC::IO_AsciiParticles", payloadCode, "@",
"HepMC::IO_BaseClass", payloadCode, "@",
"HepMC::IO_Exception", payloadCode, "@",
"HepMC::IO_GenEvent", payloadCode, "@",
"HepMC::PdfInfo", payloadCode, "@",
"HepMC::Polarization", payloadCode, "@",
"HepMC::StreamInfo", payloadCode, "@",
"HepMC::TempParticleMap", payloadCode, "@",
"HepMC::ThreeVector", payloadCode, "@",
"HepMC::WeightContainer", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("HepMCIncDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_HepMCIncDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_HepMCIncDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_HepMCIncDict() {
  TriggerDictionaryInitialization_HepMCIncDict_Impl();
}

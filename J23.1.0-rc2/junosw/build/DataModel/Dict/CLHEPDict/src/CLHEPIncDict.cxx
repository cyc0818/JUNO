// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIjunofsdIusersdIyuchinchengdIJ23dO1dO0mIrc2dIjunoswdIbuilddIDataModeldIDictdICLHEPDictdIsrcdICLHEPIncDict
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
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *CLHEPcLcLHep3Vector_Dictionary();
   static void CLHEPcLcLHep3Vector_TClassManip(TClass*);
   static void *new_CLHEPcLcLHep3Vector(void *p = nullptr);
   static void *newArray_CLHEPcLcLHep3Vector(Long_t size, void *p);
   static void delete_CLHEPcLcLHep3Vector(void *p);
   static void deleteArray_CLHEPcLcLHep3Vector(void *p);
   static void destruct_CLHEPcLcLHep3Vector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::Hep3Vector*)
   {
      ::CLHEP::Hep3Vector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::Hep3Vector));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::Hep3Vector", "CLHEP/Vector/ThreeVector.h", 41,
                  typeid(::CLHEP::Hep3Vector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHep3Vector_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::Hep3Vector) );
      instance.SetNew(&new_CLHEPcLcLHep3Vector);
      instance.SetNewArray(&newArray_CLHEPcLcLHep3Vector);
      instance.SetDelete(&delete_CLHEPcLcLHep3Vector);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHep3Vector);
      instance.SetDestructor(&destruct_CLHEPcLcLHep3Vector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::Hep3Vector*)
   {
      return GenerateInitInstanceLocal((::CLHEP::Hep3Vector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::Hep3Vector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHep3Vector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::Hep3Vector*)nullptr)->GetClass();
      CLHEPcLcLHep3Vector_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHep3Vector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepAxisAngle_Dictionary();
   static void CLHEPcLcLHepAxisAngle_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepAxisAngle(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepAxisAngle(Long_t size, void *p);
   static void delete_CLHEPcLcLHepAxisAngle(void *p);
   static void deleteArray_CLHEPcLcLHepAxisAngle(void *p);
   static void destruct_CLHEPcLcLHepAxisAngle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepAxisAngle*)
   {
      ::CLHEP::HepAxisAngle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepAxisAngle));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepAxisAngle", "CLHEP/Vector/AxisAngle.h", 37,
                  typeid(::CLHEP::HepAxisAngle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepAxisAngle_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepAxisAngle) );
      instance.SetNew(&new_CLHEPcLcLHepAxisAngle);
      instance.SetNewArray(&newArray_CLHEPcLcLHepAxisAngle);
      instance.SetDelete(&delete_CLHEPcLcLHepAxisAngle);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepAxisAngle);
      instance.SetDestructor(&destruct_CLHEPcLcLHepAxisAngle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepAxisAngle*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepAxisAngle*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepAxisAngle*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepAxisAngle_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepAxisAngle*)nullptr)->GetClass();
      CLHEPcLcLHepAxisAngle_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepAxisAngle_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepLorentzVector_Dictionary();
   static void CLHEPcLcLHepLorentzVector_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepLorentzVector(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepLorentzVector(Long_t size, void *p);
   static void delete_CLHEPcLcLHepLorentzVector(void *p);
   static void deleteArray_CLHEPcLcLHepLorentzVector(void *p);
   static void destruct_CLHEPcLcLHepLorentzVector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepLorentzVector*)
   {
      ::CLHEP::HepLorentzVector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepLorentzVector));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepLorentzVector", "CLHEP/Vector/LorentzVector.h", 72,
                  typeid(::CLHEP::HepLorentzVector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepLorentzVector_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepLorentzVector) );
      instance.SetNew(&new_CLHEPcLcLHepLorentzVector);
      instance.SetNewArray(&newArray_CLHEPcLcLHepLorentzVector);
      instance.SetDelete(&delete_CLHEPcLcLHepLorentzVector);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepLorentzVector);
      instance.SetDestructor(&destruct_CLHEPcLcLHepLorentzVector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepLorentzVector*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepLorentzVector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepLorentzVector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepLorentzVector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepLorentzVector*)nullptr)->GetClass();
      CLHEPcLcLHepLorentzVector_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepLorentzVector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHep4RotationInterface_Dictionary();
   static void CLHEPcLcLHep4RotationInterface_TClassManip(TClass*);
   static void *new_CLHEPcLcLHep4RotationInterface(void *p = nullptr);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::Hep4RotationInterface*)
   {
      ::CLHEP::Hep4RotationInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::Hep4RotationInterface));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::Hep4RotationInterface", "CLHEP/Vector/RotationInterfaces.h", 100,
                  typeid(::CLHEP::Hep4RotationInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHep4RotationInterface_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::Hep4RotationInterface) );
      instance.SetNew(&new_CLHEPcLcLHep4RotationInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::Hep4RotationInterface*)
   {
      return GenerateInitInstanceLocal((::CLHEP::Hep4RotationInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::Hep4RotationInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHep4RotationInterface_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::Hep4RotationInterface*)nullptr)->GetClass();
      CLHEPcLcLHep4RotationInterface_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHep4RotationInterface_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHep3RotationInterface_Dictionary();
   static void CLHEPcLcLHep3RotationInterface_TClassManip(TClass*);
   static void *new_CLHEPcLcLHep3RotationInterface(void *p = nullptr);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::Hep3RotationInterface*)
   {
      ::CLHEP::Hep3RotationInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::Hep3RotationInterface));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::Hep3RotationInterface", "CLHEP/Vector/RotationInterfaces.h", 217,
                  typeid(::CLHEP::Hep3RotationInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHep3RotationInterface_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::Hep3RotationInterface) );
      instance.SetNew(&new_CLHEPcLcLHep3RotationInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::Hep3RotationInterface*)
   {
      return GenerateInitInstanceLocal((::CLHEP::Hep3RotationInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::Hep3RotationInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHep3RotationInterface_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::Hep3RotationInterface*)nullptr)->GetClass();
      CLHEPcLcLHep3RotationInterface_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHep3RotationInterface_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepRep3x3_Dictionary();
   static void CLHEPcLcLHepRep3x3_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepRep3x3(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepRep3x3(Long_t size, void *p);
   static void delete_CLHEPcLcLHepRep3x3(void *p);
   static void deleteArray_CLHEPcLcLHepRep3x3(void *p);
   static void destruct_CLHEPcLcLHepRep3x3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepRep3x3*)
   {
      ::CLHEP::HepRep3x3 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepRep3x3));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepRep3x3", "CLHEP/Vector/RotationInterfaces.h", 309,
                  typeid(::CLHEP::HepRep3x3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepRep3x3_Dictionary, isa_proxy, 0,
                  sizeof(::CLHEP::HepRep3x3) );
      instance.SetNew(&new_CLHEPcLcLHepRep3x3);
      instance.SetNewArray(&newArray_CLHEPcLcLHepRep3x3);
      instance.SetDelete(&delete_CLHEPcLcLHepRep3x3);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepRep3x3);
      instance.SetDestructor(&destruct_CLHEPcLcLHepRep3x3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepRep3x3*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepRep3x3*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepRep3x3*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepRep3x3_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepRep3x3*)nullptr)->GetClass();
      CLHEPcLcLHepRep3x3_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepRep3x3_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepRep4x4_Dictionary();
   static void CLHEPcLcLHepRep4x4_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepRep4x4(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepRep4x4(Long_t size, void *p);
   static void delete_CLHEPcLcLHepRep4x4(void *p);
   static void deleteArray_CLHEPcLcLHepRep4x4(void *p);
   static void destruct_CLHEPcLcLHepRep4x4(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepRep4x4*)
   {
      ::CLHEP::HepRep4x4 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepRep4x4));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepRep4x4", "CLHEP/Vector/RotationInterfaces.h", 336,
                  typeid(::CLHEP::HepRep4x4), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepRep4x4_Dictionary, isa_proxy, 0,
                  sizeof(::CLHEP::HepRep4x4) );
      instance.SetNew(&new_CLHEPcLcLHepRep4x4);
      instance.SetNewArray(&newArray_CLHEPcLcLHepRep4x4);
      instance.SetDelete(&delete_CLHEPcLcLHepRep4x4);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepRep4x4);
      instance.SetDestructor(&destruct_CLHEPcLcLHepRep4x4);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepRep4x4*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepRep4x4*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepRep4x4*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepRep4x4_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepRep4x4*)nullptr)->GetClass();
      CLHEPcLcLHepRep4x4_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepRep4x4_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepRep4x4Symmetric_Dictionary();
   static void CLHEPcLcLHepRep4x4Symmetric_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepRep4x4Symmetric(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepRep4x4Symmetric(Long_t size, void *p);
   static void delete_CLHEPcLcLHepRep4x4Symmetric(void *p);
   static void deleteArray_CLHEPcLcLHepRep4x4Symmetric(void *p);
   static void destruct_CLHEPcLcLHepRep4x4Symmetric(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepRep4x4Symmetric*)
   {
      ::CLHEP::HepRep4x4Symmetric *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepRep4x4Symmetric));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepRep4x4Symmetric", "CLHEP/Vector/RotationInterfaces.h", 370,
                  typeid(::CLHEP::HepRep4x4Symmetric), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepRep4x4Symmetric_Dictionary, isa_proxy, 0,
                  sizeof(::CLHEP::HepRep4x4Symmetric) );
      instance.SetNew(&new_CLHEPcLcLHepRep4x4Symmetric);
      instance.SetNewArray(&newArray_CLHEPcLcLHepRep4x4Symmetric);
      instance.SetDelete(&delete_CLHEPcLcLHepRep4x4Symmetric);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepRep4x4Symmetric);
      instance.SetDestructor(&destruct_CLHEPcLcLHepRep4x4Symmetric);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepRep4x4Symmetric*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepRep4x4Symmetric*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepRep4x4Symmetric*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepRep4x4Symmetric_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepRep4x4Symmetric*)nullptr)->GetClass();
      CLHEPcLcLHepRep4x4Symmetric_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepRep4x4Symmetric_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepBoostX_Dictionary();
   static void CLHEPcLcLHepBoostX_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepBoostX(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepBoostX(Long_t size, void *p);
   static void delete_CLHEPcLcLHepBoostX(void *p);
   static void deleteArray_CLHEPcLcLHepBoostX(void *p);
   static void destruct_CLHEPcLcLHepBoostX(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepBoostX*)
   {
      ::CLHEP::HepBoostX *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepBoostX));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepBoostX", "CLHEP/Vector/BoostX.h", 42,
                  typeid(::CLHEP::HepBoostX), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepBoostX_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepBoostX) );
      instance.SetNew(&new_CLHEPcLcLHepBoostX);
      instance.SetNewArray(&newArray_CLHEPcLcLHepBoostX);
      instance.SetDelete(&delete_CLHEPcLcLHepBoostX);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepBoostX);
      instance.SetDestructor(&destruct_CLHEPcLcLHepBoostX);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepBoostX*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepBoostX*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepBoostX*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepBoostX_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepBoostX*)nullptr)->GetClass();
      CLHEPcLcLHepBoostX_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepBoostX_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepBoostY_Dictionary();
   static void CLHEPcLcLHepBoostY_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepBoostY(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepBoostY(Long_t size, void *p);
   static void delete_CLHEPcLcLHepBoostY(void *p);
   static void deleteArray_CLHEPcLcLHepBoostY(void *p);
   static void destruct_CLHEPcLcLHepBoostY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepBoostY*)
   {
      ::CLHEP::HepBoostY *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepBoostY));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepBoostY", "CLHEP/Vector/BoostY.h", 42,
                  typeid(::CLHEP::HepBoostY), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepBoostY_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepBoostY) );
      instance.SetNew(&new_CLHEPcLcLHepBoostY);
      instance.SetNewArray(&newArray_CLHEPcLcLHepBoostY);
      instance.SetDelete(&delete_CLHEPcLcLHepBoostY);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepBoostY);
      instance.SetDestructor(&destruct_CLHEPcLcLHepBoostY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepBoostY*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepBoostY*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepBoostY*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepBoostY_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepBoostY*)nullptr)->GetClass();
      CLHEPcLcLHepBoostY_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepBoostY_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepBoostZ_Dictionary();
   static void CLHEPcLcLHepBoostZ_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepBoostZ(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepBoostZ(Long_t size, void *p);
   static void delete_CLHEPcLcLHepBoostZ(void *p);
   static void deleteArray_CLHEPcLcLHepBoostZ(void *p);
   static void destruct_CLHEPcLcLHepBoostZ(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepBoostZ*)
   {
      ::CLHEP::HepBoostZ *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepBoostZ));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepBoostZ", "CLHEP/Vector/BoostZ.h", 42,
                  typeid(::CLHEP::HepBoostZ), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepBoostZ_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepBoostZ) );
      instance.SetNew(&new_CLHEPcLcLHepBoostZ);
      instance.SetNewArray(&newArray_CLHEPcLcLHepBoostZ);
      instance.SetDelete(&delete_CLHEPcLcLHepBoostZ);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepBoostZ);
      instance.SetDestructor(&destruct_CLHEPcLcLHepBoostZ);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepBoostZ*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepBoostZ*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepBoostZ*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepBoostZ_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepBoostZ*)nullptr)->GetClass();
      CLHEPcLcLHepBoostZ_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepBoostZ_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepBoost_Dictionary();
   static void CLHEPcLcLHepBoost_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepBoost(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepBoost(Long_t size, void *p);
   static void delete_CLHEPcLcLHepBoost(void *p);
   static void deleteArray_CLHEPcLcLHepBoost(void *p);
   static void destruct_CLHEPcLcLHepBoost(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepBoost*)
   {
      ::CLHEP::HepBoost *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepBoost));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepBoost", "CLHEP/Vector/Boost.h", 43,
                  typeid(::CLHEP::HepBoost), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepBoost_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepBoost) );
      instance.SetNew(&new_CLHEPcLcLHepBoost);
      instance.SetNewArray(&newArray_CLHEPcLcLHepBoost);
      instance.SetDelete(&delete_CLHEPcLcLHepBoost);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepBoost);
      instance.SetDestructor(&destruct_CLHEPcLcLHepBoost);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepBoost*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepBoost*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepBoost*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepBoost_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepBoost*)nullptr)->GetClass();
      CLHEPcLcLHepBoost_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepBoost_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepEulerAngles_Dictionary();
   static void CLHEPcLcLHepEulerAngles_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepEulerAngles(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepEulerAngles(Long_t size, void *p);
   static void delete_CLHEPcLcLHepEulerAngles(void *p);
   static void deleteArray_CLHEPcLcLHepEulerAngles(void *p);
   static void destruct_CLHEPcLcLHepEulerAngles(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepEulerAngles*)
   {
      ::CLHEP::HepEulerAngles *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepEulerAngles));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepEulerAngles", "CLHEP/Vector/EulerAngles.h", 32,
                  typeid(::CLHEP::HepEulerAngles), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepEulerAngles_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepEulerAngles) );
      instance.SetNew(&new_CLHEPcLcLHepEulerAngles);
      instance.SetNewArray(&newArray_CLHEPcLcLHepEulerAngles);
      instance.SetDelete(&delete_CLHEPcLcLHepEulerAngles);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepEulerAngles);
      instance.SetDestructor(&destruct_CLHEPcLcLHepEulerAngles);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepEulerAngles*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepEulerAngles*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepEulerAngles*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepEulerAngles_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepEulerAngles*)nullptr)->GetClass();
      CLHEPcLcLHepEulerAngles_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepEulerAngles_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepRotationX_Dictionary();
   static void CLHEPcLcLHepRotationX_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepRotationX(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepRotationX(Long_t size, void *p);
   static void delete_CLHEPcLcLHepRotationX(void *p);
   static void deleteArray_CLHEPcLcLHepRotationX(void *p);
   static void destruct_CLHEPcLcLHepRotationX(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepRotationX*)
   {
      ::CLHEP::HepRotationX *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepRotationX));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepRotationX", "CLHEP/Vector/RotationX.h", 44,
                  typeid(::CLHEP::HepRotationX), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepRotationX_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepRotationX) );
      instance.SetNew(&new_CLHEPcLcLHepRotationX);
      instance.SetNewArray(&newArray_CLHEPcLcLHepRotationX);
      instance.SetDelete(&delete_CLHEPcLcLHepRotationX);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepRotationX);
      instance.SetDestructor(&destruct_CLHEPcLcLHepRotationX);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepRotationX*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepRotationX*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepRotationX*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepRotationX_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepRotationX*)nullptr)->GetClass();
      CLHEPcLcLHepRotationX_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepRotationX_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepRotationY_Dictionary();
   static void CLHEPcLcLHepRotationY_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepRotationY(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepRotationY(Long_t size, void *p);
   static void delete_CLHEPcLcLHepRotationY(void *p);
   static void deleteArray_CLHEPcLcLHepRotationY(void *p);
   static void destruct_CLHEPcLcLHepRotationY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepRotationY*)
   {
      ::CLHEP::HepRotationY *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepRotationY));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepRotationY", "CLHEP/Vector/RotationY.h", 43,
                  typeid(::CLHEP::HepRotationY), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepRotationY_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepRotationY) );
      instance.SetNew(&new_CLHEPcLcLHepRotationY);
      instance.SetNewArray(&newArray_CLHEPcLcLHepRotationY);
      instance.SetDelete(&delete_CLHEPcLcLHepRotationY);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepRotationY);
      instance.SetDestructor(&destruct_CLHEPcLcLHepRotationY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepRotationY*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepRotationY*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepRotationY*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepRotationY_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepRotationY*)nullptr)->GetClass();
      CLHEPcLcLHepRotationY_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepRotationY_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepRotationZ_Dictionary();
   static void CLHEPcLcLHepRotationZ_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepRotationZ(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepRotationZ(Long_t size, void *p);
   static void delete_CLHEPcLcLHepRotationZ(void *p);
   static void deleteArray_CLHEPcLcLHepRotationZ(void *p);
   static void destruct_CLHEPcLcLHepRotationZ(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepRotationZ*)
   {
      ::CLHEP::HepRotationZ *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepRotationZ));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepRotationZ", "CLHEP/Vector/RotationZ.h", 43,
                  typeid(::CLHEP::HepRotationZ), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepRotationZ_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepRotationZ) );
      instance.SetNew(&new_CLHEPcLcLHepRotationZ);
      instance.SetNewArray(&newArray_CLHEPcLcLHepRotationZ);
      instance.SetDelete(&delete_CLHEPcLcLHepRotationZ);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepRotationZ);
      instance.SetDestructor(&destruct_CLHEPcLcLHepRotationZ);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepRotationZ*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepRotationZ*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepRotationZ*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepRotationZ_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepRotationZ*)nullptr)->GetClass();
      CLHEPcLcLHepRotationZ_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepRotationZ_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepRotation_Dictionary();
   static void CLHEPcLcLHepRotation_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepRotation(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepRotation(Long_t size, void *p);
   static void delete_CLHEPcLcLHepRotation(void *p);
   static void deleteArray_CLHEPcLcLHepRotation(void *p);
   static void destruct_CLHEPcLcLHepRotation(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepRotation*)
   {
      ::CLHEP::HepRotation *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepRotation));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepRotation", "CLHEP/Vector/Rotation.h", 48,
                  typeid(::CLHEP::HepRotation), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepRotation_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepRotation) );
      instance.SetNew(&new_CLHEPcLcLHepRotation);
      instance.SetNewArray(&newArray_CLHEPcLcLHepRotation);
      instance.SetDelete(&delete_CLHEPcLcLHepRotation);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepRotation);
      instance.SetDestructor(&destruct_CLHEPcLcLHepRotation);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepRotation*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepRotation*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepRotation*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepRotation_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepRotation*)nullptr)->GetClass();
      CLHEPcLcLHepRotation_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepRotation_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepLorentzRotation_Dictionary();
   static void CLHEPcLcLHepLorentzRotation_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepLorentzRotation(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepLorentzRotation(Long_t size, void *p);
   static void delete_CLHEPcLcLHepLorentzRotation(void *p);
   static void deleteArray_CLHEPcLcLHepLorentzRotation(void *p);
   static void destruct_CLHEPcLcLHepLorentzRotation(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepLorentzRotation*)
   {
      ::CLHEP::HepLorentzRotation *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepLorentzRotation));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepLorentzRotation", "CLHEP/Vector/LorentzRotation.h", 54,
                  typeid(::CLHEP::HepLorentzRotation), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepLorentzRotation_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepLorentzRotation) );
      instance.SetNew(&new_CLHEPcLcLHepLorentzRotation);
      instance.SetNewArray(&newArray_CLHEPcLcLHepLorentzRotation);
      instance.SetDelete(&delete_CLHEPcLcLHepLorentzRotation);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepLorentzRotation);
      instance.SetDestructor(&destruct_CLHEPcLcLHepLorentzRotation);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepLorentzRotation*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepLorentzRotation*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepLorentzRotation*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepLorentzRotation_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepLorentzRotation*)nullptr)->GetClass();
      CLHEPcLcLHepLorentzRotation_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepLorentzRotation_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHep2Vector_Dictionary();
   static void CLHEPcLcLHep2Vector_TClassManip(TClass*);
   static void *new_CLHEPcLcLHep2Vector(void *p = nullptr);
   static void *newArray_CLHEPcLcLHep2Vector(Long_t size, void *p);
   static void delete_CLHEPcLcLHep2Vector(void *p);
   static void deleteArray_CLHEPcLcLHep2Vector(void *p);
   static void destruct_CLHEPcLcLHep2Vector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::Hep2Vector*)
   {
      ::CLHEP::Hep2Vector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::Hep2Vector));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::Hep2Vector", "CLHEP/Vector/TwoVector.h", 49,
                  typeid(::CLHEP::Hep2Vector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHep2Vector_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::Hep2Vector) );
      instance.SetNew(&new_CLHEPcLcLHep2Vector);
      instance.SetNewArray(&newArray_CLHEPcLcLHep2Vector);
      instance.SetDelete(&delete_CLHEPcLcLHep2Vector);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHep2Vector);
      instance.SetDestructor(&destruct_CLHEPcLcLHep2Vector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::Hep2Vector*)
   {
      return GenerateInitInstanceLocal((::CLHEP::Hep2Vector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::Hep2Vector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHep2Vector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::Hep2Vector*)nullptr)->GetClass();
      CLHEPcLcLHep2Vector_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHep2Vector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepGenMatrix_Dictionary();
   static void CLHEPcLcLHepGenMatrix_TClassManip(TClass*);
   static void delete_CLHEPcLcLHepGenMatrix(void *p);
   static void deleteArray_CLHEPcLcLHepGenMatrix(void *p);
   static void destruct_CLHEPcLcLHepGenMatrix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepGenMatrix*)
   {
      ::CLHEP::HepGenMatrix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepGenMatrix));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepGenMatrix", "CLHEP/Matrix/GenMatrix.h", 36,
                  typeid(::CLHEP::HepGenMatrix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepGenMatrix_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepGenMatrix) );
      instance.SetDelete(&delete_CLHEPcLcLHepGenMatrix);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepGenMatrix);
      instance.SetDestructor(&destruct_CLHEPcLcLHepGenMatrix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepGenMatrix*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepGenMatrix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepGenMatrix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepGenMatrix_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepGenMatrix*)nullptr)->GetClass();
      CLHEPcLcLHepGenMatrix_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepGenMatrix_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepDiagMatrix_Dictionary();
   static void CLHEPcLcLHepDiagMatrix_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepDiagMatrix(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepDiagMatrix(Long_t size, void *p);
   static void delete_CLHEPcLcLHepDiagMatrix(void *p);
   static void deleteArray_CLHEPcLcLHepDiagMatrix(void *p);
   static void destruct_CLHEPcLcLHepDiagMatrix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepDiagMatrix*)
   {
      ::CLHEP::HepDiagMatrix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepDiagMatrix));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepDiagMatrix", "CLHEP/Matrix/DiagMatrix.h", 39,
                  typeid(::CLHEP::HepDiagMatrix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepDiagMatrix_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepDiagMatrix) );
      instance.SetNew(&new_CLHEPcLcLHepDiagMatrix);
      instance.SetNewArray(&newArray_CLHEPcLcLHepDiagMatrix);
      instance.SetDelete(&delete_CLHEPcLcLHepDiagMatrix);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepDiagMatrix);
      instance.SetDestructor(&destruct_CLHEPcLcLHepDiagMatrix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepDiagMatrix*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepDiagMatrix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepDiagMatrix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepDiagMatrix_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepDiagMatrix*)nullptr)->GetClass();
      CLHEPcLcLHepDiagMatrix_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepDiagMatrix_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepMatrix_Dictionary();
   static void CLHEPcLcLHepMatrix_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepMatrix(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepMatrix(Long_t size, void *p);
   static void delete_CLHEPcLcLHepMatrix(void *p);
   static void deleteArray_CLHEPcLcLHepMatrix(void *p);
   static void destruct_CLHEPcLcLHepMatrix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepMatrix*)
   {
      ::CLHEP::HepMatrix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepMatrix));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepMatrix", "CLHEP/Matrix/Matrix.h", 209,
                  typeid(::CLHEP::HepMatrix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepMatrix_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepMatrix) );
      instance.SetNew(&new_CLHEPcLcLHepMatrix);
      instance.SetNewArray(&newArray_CLHEPcLcLHepMatrix);
      instance.SetDelete(&delete_CLHEPcLcLHepMatrix);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepMatrix);
      instance.SetDestructor(&destruct_CLHEPcLcLHepMatrix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepMatrix*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepMatrix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepMatrix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepMatrix_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepMatrix*)nullptr)->GetClass();
      CLHEPcLcLHepMatrix_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepMatrix_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepSymMatrix_Dictionary();
   static void CLHEPcLcLHepSymMatrix_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepSymMatrix(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepSymMatrix(Long_t size, void *p);
   static void delete_CLHEPcLcLHepSymMatrix(void *p);
   static void deleteArray_CLHEPcLcLHepSymMatrix(void *p);
   static void destruct_CLHEPcLcLHepSymMatrix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepSymMatrix*)
   {
      ::CLHEP::HepSymMatrix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepSymMatrix));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepSymMatrix", "CLHEP/Matrix/SymMatrix.h", 90,
                  typeid(::CLHEP::HepSymMatrix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepSymMatrix_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepSymMatrix) );
      instance.SetNew(&new_CLHEPcLcLHepSymMatrix);
      instance.SetNewArray(&newArray_CLHEPcLcLHepSymMatrix);
      instance.SetDelete(&delete_CLHEPcLcLHepSymMatrix);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepSymMatrix);
      instance.SetDestructor(&destruct_CLHEPcLcLHepSymMatrix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepSymMatrix*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepSymMatrix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepSymMatrix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepSymMatrix_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepSymMatrix*)nullptr)->GetClass();
      CLHEPcLcLHepSymMatrix_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepSymMatrix_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CLHEPcLcLHepVector_Dictionary();
   static void CLHEPcLcLHepVector_TClassManip(TClass*);
   static void *new_CLHEPcLcLHepVector(void *p = nullptr);
   static void *newArray_CLHEPcLcLHepVector(Long_t size, void *p);
   static void delete_CLHEPcLcLHepVector(void *p);
   static void deleteArray_CLHEPcLcLHepVector(void *p);
   static void destruct_CLHEPcLcLHepVector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::HepVector*)
   {
      ::CLHEP::HepVector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::HepVector));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::HepVector", "CLHEP/Matrix/Vector.h", 39,
                  typeid(::CLHEP::HepVector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHepVector_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::HepVector) );
      instance.SetNew(&new_CLHEPcLcLHepVector);
      instance.SetNewArray(&newArray_CLHEPcLcLHepVector);
      instance.SetDelete(&delete_CLHEPcLcLHepVector);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHepVector);
      instance.SetDestructor(&destruct_CLHEPcLcLHepVector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::HepVector*)
   {
      return GenerateInitInstanceLocal((::CLHEP::HepVector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CLHEP::HepVector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHepVector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CLHEP::HepVector*)nullptr)->GetClass();
      CLHEPcLcLHepVector_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHepVector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHep3Vector(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::Hep3Vector : new ::CLHEP::Hep3Vector;
   }
   static void *newArray_CLHEPcLcLHep3Vector(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::Hep3Vector[nElements] : new ::CLHEP::Hep3Vector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHep3Vector(void *p) {
      delete ((::CLHEP::Hep3Vector*)p);
   }
   static void deleteArray_CLHEPcLcLHep3Vector(void *p) {
      delete [] ((::CLHEP::Hep3Vector*)p);
   }
   static void destruct_CLHEPcLcLHep3Vector(void *p) {
      typedef ::CLHEP::Hep3Vector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::Hep3Vector

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepAxisAngle(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepAxisAngle : new ::CLHEP::HepAxisAngle;
   }
   static void *newArray_CLHEPcLcLHepAxisAngle(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepAxisAngle[nElements] : new ::CLHEP::HepAxisAngle[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepAxisAngle(void *p) {
      delete ((::CLHEP::HepAxisAngle*)p);
   }
   static void deleteArray_CLHEPcLcLHepAxisAngle(void *p) {
      delete [] ((::CLHEP::HepAxisAngle*)p);
   }
   static void destruct_CLHEPcLcLHepAxisAngle(void *p) {
      typedef ::CLHEP::HepAxisAngle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepAxisAngle

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepLorentzVector(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepLorentzVector : new ::CLHEP::HepLorentzVector;
   }
   static void *newArray_CLHEPcLcLHepLorentzVector(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepLorentzVector[nElements] : new ::CLHEP::HepLorentzVector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepLorentzVector(void *p) {
      delete ((::CLHEP::HepLorentzVector*)p);
   }
   static void deleteArray_CLHEPcLcLHepLorentzVector(void *p) {
      delete [] ((::CLHEP::HepLorentzVector*)p);
   }
   static void destruct_CLHEPcLcLHepLorentzVector(void *p) {
      typedef ::CLHEP::HepLorentzVector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepLorentzVector

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHep4RotationInterface(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::Hep4RotationInterface : new ::CLHEP::Hep4RotationInterface;
   }
} // end of namespace ROOT for class ::CLHEP::Hep4RotationInterface

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHep3RotationInterface(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::Hep3RotationInterface : new ::CLHEP::Hep3RotationInterface;
   }
} // end of namespace ROOT for class ::CLHEP::Hep3RotationInterface

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepRep3x3(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRep3x3 : new ::CLHEP::HepRep3x3;
   }
   static void *newArray_CLHEPcLcLHepRep3x3(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRep3x3[nElements] : new ::CLHEP::HepRep3x3[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepRep3x3(void *p) {
      delete ((::CLHEP::HepRep3x3*)p);
   }
   static void deleteArray_CLHEPcLcLHepRep3x3(void *p) {
      delete [] ((::CLHEP::HepRep3x3*)p);
   }
   static void destruct_CLHEPcLcLHepRep3x3(void *p) {
      typedef ::CLHEP::HepRep3x3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepRep3x3

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepRep4x4(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRep4x4 : new ::CLHEP::HepRep4x4;
   }
   static void *newArray_CLHEPcLcLHepRep4x4(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRep4x4[nElements] : new ::CLHEP::HepRep4x4[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepRep4x4(void *p) {
      delete ((::CLHEP::HepRep4x4*)p);
   }
   static void deleteArray_CLHEPcLcLHepRep4x4(void *p) {
      delete [] ((::CLHEP::HepRep4x4*)p);
   }
   static void destruct_CLHEPcLcLHepRep4x4(void *p) {
      typedef ::CLHEP::HepRep4x4 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepRep4x4

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepRep4x4Symmetric(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRep4x4Symmetric : new ::CLHEP::HepRep4x4Symmetric;
   }
   static void *newArray_CLHEPcLcLHepRep4x4Symmetric(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRep4x4Symmetric[nElements] : new ::CLHEP::HepRep4x4Symmetric[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepRep4x4Symmetric(void *p) {
      delete ((::CLHEP::HepRep4x4Symmetric*)p);
   }
   static void deleteArray_CLHEPcLcLHepRep4x4Symmetric(void *p) {
      delete [] ((::CLHEP::HepRep4x4Symmetric*)p);
   }
   static void destruct_CLHEPcLcLHepRep4x4Symmetric(void *p) {
      typedef ::CLHEP::HepRep4x4Symmetric current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepRep4x4Symmetric

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepBoostX(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoostX : new ::CLHEP::HepBoostX;
   }
   static void *newArray_CLHEPcLcLHepBoostX(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoostX[nElements] : new ::CLHEP::HepBoostX[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepBoostX(void *p) {
      delete ((::CLHEP::HepBoostX*)p);
   }
   static void deleteArray_CLHEPcLcLHepBoostX(void *p) {
      delete [] ((::CLHEP::HepBoostX*)p);
   }
   static void destruct_CLHEPcLcLHepBoostX(void *p) {
      typedef ::CLHEP::HepBoostX current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepBoostX

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepBoostY(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoostY : new ::CLHEP::HepBoostY;
   }
   static void *newArray_CLHEPcLcLHepBoostY(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoostY[nElements] : new ::CLHEP::HepBoostY[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepBoostY(void *p) {
      delete ((::CLHEP::HepBoostY*)p);
   }
   static void deleteArray_CLHEPcLcLHepBoostY(void *p) {
      delete [] ((::CLHEP::HepBoostY*)p);
   }
   static void destruct_CLHEPcLcLHepBoostY(void *p) {
      typedef ::CLHEP::HepBoostY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepBoostY

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepBoostZ(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoostZ : new ::CLHEP::HepBoostZ;
   }
   static void *newArray_CLHEPcLcLHepBoostZ(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoostZ[nElements] : new ::CLHEP::HepBoostZ[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepBoostZ(void *p) {
      delete ((::CLHEP::HepBoostZ*)p);
   }
   static void deleteArray_CLHEPcLcLHepBoostZ(void *p) {
      delete [] ((::CLHEP::HepBoostZ*)p);
   }
   static void destruct_CLHEPcLcLHepBoostZ(void *p) {
      typedef ::CLHEP::HepBoostZ current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepBoostZ

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepBoost(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoost : new ::CLHEP::HepBoost;
   }
   static void *newArray_CLHEPcLcLHepBoost(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepBoost[nElements] : new ::CLHEP::HepBoost[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepBoost(void *p) {
      delete ((::CLHEP::HepBoost*)p);
   }
   static void deleteArray_CLHEPcLcLHepBoost(void *p) {
      delete [] ((::CLHEP::HepBoost*)p);
   }
   static void destruct_CLHEPcLcLHepBoost(void *p) {
      typedef ::CLHEP::HepBoost current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepBoost

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepEulerAngles(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepEulerAngles : new ::CLHEP::HepEulerAngles;
   }
   static void *newArray_CLHEPcLcLHepEulerAngles(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepEulerAngles[nElements] : new ::CLHEP::HepEulerAngles[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepEulerAngles(void *p) {
      delete ((::CLHEP::HepEulerAngles*)p);
   }
   static void deleteArray_CLHEPcLcLHepEulerAngles(void *p) {
      delete [] ((::CLHEP::HepEulerAngles*)p);
   }
   static void destruct_CLHEPcLcLHepEulerAngles(void *p) {
      typedef ::CLHEP::HepEulerAngles current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepEulerAngles

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepRotationX(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotationX : new ::CLHEP::HepRotationX;
   }
   static void *newArray_CLHEPcLcLHepRotationX(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotationX[nElements] : new ::CLHEP::HepRotationX[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepRotationX(void *p) {
      delete ((::CLHEP::HepRotationX*)p);
   }
   static void deleteArray_CLHEPcLcLHepRotationX(void *p) {
      delete [] ((::CLHEP::HepRotationX*)p);
   }
   static void destruct_CLHEPcLcLHepRotationX(void *p) {
      typedef ::CLHEP::HepRotationX current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepRotationX

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepRotationY(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotationY : new ::CLHEP::HepRotationY;
   }
   static void *newArray_CLHEPcLcLHepRotationY(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotationY[nElements] : new ::CLHEP::HepRotationY[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepRotationY(void *p) {
      delete ((::CLHEP::HepRotationY*)p);
   }
   static void deleteArray_CLHEPcLcLHepRotationY(void *p) {
      delete [] ((::CLHEP::HepRotationY*)p);
   }
   static void destruct_CLHEPcLcLHepRotationY(void *p) {
      typedef ::CLHEP::HepRotationY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepRotationY

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepRotationZ(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotationZ : new ::CLHEP::HepRotationZ;
   }
   static void *newArray_CLHEPcLcLHepRotationZ(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotationZ[nElements] : new ::CLHEP::HepRotationZ[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepRotationZ(void *p) {
      delete ((::CLHEP::HepRotationZ*)p);
   }
   static void deleteArray_CLHEPcLcLHepRotationZ(void *p) {
      delete [] ((::CLHEP::HepRotationZ*)p);
   }
   static void destruct_CLHEPcLcLHepRotationZ(void *p) {
      typedef ::CLHEP::HepRotationZ current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepRotationZ

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepRotation(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotation : new ::CLHEP::HepRotation;
   }
   static void *newArray_CLHEPcLcLHepRotation(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepRotation[nElements] : new ::CLHEP::HepRotation[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepRotation(void *p) {
      delete ((::CLHEP::HepRotation*)p);
   }
   static void deleteArray_CLHEPcLcLHepRotation(void *p) {
      delete [] ((::CLHEP::HepRotation*)p);
   }
   static void destruct_CLHEPcLcLHepRotation(void *p) {
      typedef ::CLHEP::HepRotation current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepRotation

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepLorentzRotation(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepLorentzRotation : new ::CLHEP::HepLorentzRotation;
   }
   static void *newArray_CLHEPcLcLHepLorentzRotation(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepLorentzRotation[nElements] : new ::CLHEP::HepLorentzRotation[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepLorentzRotation(void *p) {
      delete ((::CLHEP::HepLorentzRotation*)p);
   }
   static void deleteArray_CLHEPcLcLHepLorentzRotation(void *p) {
      delete [] ((::CLHEP::HepLorentzRotation*)p);
   }
   static void destruct_CLHEPcLcLHepLorentzRotation(void *p) {
      typedef ::CLHEP::HepLorentzRotation current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepLorentzRotation

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHep2Vector(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::Hep2Vector : new ::CLHEP::Hep2Vector;
   }
   static void *newArray_CLHEPcLcLHep2Vector(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::Hep2Vector[nElements] : new ::CLHEP::Hep2Vector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHep2Vector(void *p) {
      delete ((::CLHEP::Hep2Vector*)p);
   }
   static void deleteArray_CLHEPcLcLHep2Vector(void *p) {
      delete [] ((::CLHEP::Hep2Vector*)p);
   }
   static void destruct_CLHEPcLcLHep2Vector(void *p) {
      typedef ::CLHEP::Hep2Vector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::Hep2Vector

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepGenMatrix(void *p) {
      delete ((::CLHEP::HepGenMatrix*)p);
   }
   static void deleteArray_CLHEPcLcLHepGenMatrix(void *p) {
      delete [] ((::CLHEP::HepGenMatrix*)p);
   }
   static void destruct_CLHEPcLcLHepGenMatrix(void *p) {
      typedef ::CLHEP::HepGenMatrix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepGenMatrix

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepDiagMatrix(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepDiagMatrix : new ::CLHEP::HepDiagMatrix;
   }
   static void *newArray_CLHEPcLcLHepDiagMatrix(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepDiagMatrix[nElements] : new ::CLHEP::HepDiagMatrix[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepDiagMatrix(void *p) {
      delete ((::CLHEP::HepDiagMatrix*)p);
   }
   static void deleteArray_CLHEPcLcLHepDiagMatrix(void *p) {
      delete [] ((::CLHEP::HepDiagMatrix*)p);
   }
   static void destruct_CLHEPcLcLHepDiagMatrix(void *p) {
      typedef ::CLHEP::HepDiagMatrix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepDiagMatrix

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepMatrix(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepMatrix : new ::CLHEP::HepMatrix;
   }
   static void *newArray_CLHEPcLcLHepMatrix(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepMatrix[nElements] : new ::CLHEP::HepMatrix[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepMatrix(void *p) {
      delete ((::CLHEP::HepMatrix*)p);
   }
   static void deleteArray_CLHEPcLcLHepMatrix(void *p) {
      delete [] ((::CLHEP::HepMatrix*)p);
   }
   static void destruct_CLHEPcLcLHepMatrix(void *p) {
      typedef ::CLHEP::HepMatrix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepMatrix

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepSymMatrix(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepSymMatrix : new ::CLHEP::HepSymMatrix;
   }
   static void *newArray_CLHEPcLcLHepSymMatrix(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepSymMatrix[nElements] : new ::CLHEP::HepSymMatrix[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepSymMatrix(void *p) {
      delete ((::CLHEP::HepSymMatrix*)p);
   }
   static void deleteArray_CLHEPcLcLHepSymMatrix(void *p) {
      delete [] ((::CLHEP::HepSymMatrix*)p);
   }
   static void destruct_CLHEPcLcLHepSymMatrix(void *p) {
      typedef ::CLHEP::HepSymMatrix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepSymMatrix

namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHepVector(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepVector : new ::CLHEP::HepVector;
   }
   static void *newArray_CLHEPcLcLHepVector(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::CLHEP::HepVector[nElements] : new ::CLHEP::HepVector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHepVector(void *p) {
      delete ((::CLHEP::HepVector*)p);
   }
   static void deleteArray_CLHEPcLcLHepVector(void *p) {
      delete [] ((::CLHEP::HepVector*)p);
   }
   static void destruct_CLHEPcLcLHepVector(void *p) {
      typedef ::CLHEP::HepVector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::HepVector

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

namespace {
  void TriggerDictionaryInitialization_CLHEPIncDict_Impl() {
    static const char* headers[] = {
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h",
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
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/include",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/Dict/CLHEPDict",
"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/ROOT/6.26.06/include/",
"/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/Dict/CLHEPDict/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "CLHEPIncDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/ThreeVector.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  Hep3Vector;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/AxisAngle.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepAxisAngle;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/LorentzVector.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepLorentzVector;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationInterfaces.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  Hep4RotationInterface;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationInterfaces.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  Hep3RotationInterface;}
namespace CLHEP{struct __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationInterfaces.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepRep3x3;}
namespace CLHEP{struct __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationInterfaces.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepRep4x4;}
namespace CLHEP{struct __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationInterfaces.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepRep4x4Symmetric;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/BoostX.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepBoostX;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/BoostY.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepBoostY;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/BoostZ.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepBoostZ;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/Boost.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepBoost;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/EulerAngles.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepEulerAngles;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationX.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepRotationX;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationY.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepRotationY;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/RotationZ.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepRotationZ;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/Rotation.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepRotation;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/LorentzRotation.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepLorentzRotation;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/TwoVector.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  Hep2Vector;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Matrix/GenMatrix.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepGenMatrix;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Matrix/DiagMatrix.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepDiagMatrix;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Matrix/Matrix.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepMatrix;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Matrix/SymMatrix.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepSymMatrix;}
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Matrix/Vector.h")))  __attribute__((annotate("$clingAutoload$/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h")))  HepVector;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CLHEPIncDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/Dict/CLHEPDict/src/CLHEPInc.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CLHEP::Hep2Vector", payloadCode, "@",
"CLHEP::Hep3RotationInterface", payloadCode, "@",
"CLHEP::Hep3Vector", payloadCode, "@",
"CLHEP::Hep4RotationInterface", payloadCode, "@",
"CLHEP::HepAxisAngle", payloadCode, "@",
"CLHEP::HepBoost", payloadCode, "@",
"CLHEP::HepBoostX", payloadCode, "@",
"CLHEP::HepBoostY", payloadCode, "@",
"CLHEP::HepBoostZ", payloadCode, "@",
"CLHEP::HepDiagMatrix", payloadCode, "@",
"CLHEP::HepEulerAngles", payloadCode, "@",
"CLHEP::HepGenMatrix", payloadCode, "@",
"CLHEP::HepLorentzRotation", payloadCode, "@",
"CLHEP::HepLorentzVector", payloadCode, "@",
"CLHEP::HepMatrix", payloadCode, "@",
"CLHEP::HepRep3x3", payloadCode, "@",
"CLHEP::HepRep4x4", payloadCode, "@",
"CLHEP::HepRep4x4Symmetric", payloadCode, "@",
"CLHEP::HepRotation", payloadCode, "@",
"CLHEP::HepRotationX", payloadCode, "@",
"CLHEP::HepRotationY", payloadCode, "@",
"CLHEP::HepRotationZ", payloadCode, "@",
"CLHEP::HepSymMatrix", payloadCode, "@",
"CLHEP::HepVector", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CLHEPIncDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CLHEPIncDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CLHEPIncDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CLHEPIncDict() {
  TriggerDictionaryInitialization_CLHEPIncDict_Impl();
}

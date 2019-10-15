// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NeuronWeaponPlayground.h"

// Generated
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectRegistry.generated.h"

// Types
DECLARE_LOG_CATEGORY_EXTERN(LogNWPObjectRegistry, Log, All);

/**
 * Object that associates class names with object lists and manages them.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPObjectRegistry : public UObject
{
	GENERATED_BODY()

// Types
protected:

	class ObjectEntry
	{

		// Friend class
		friend class UNWPObjectRegistry;

		// Types
		protected:
			using BaseType = ObjectEntry;

		// Constructors
		public:

			ObjectEntry() {}

			virtual ~ObjectEntry() {}

		// Member functions
		protected:

			UClass* GetObjectClass() { return ObjectClass; }

			uint32 GetObjectCount() const { return ObjectList.Num(); }

			const TArray<UObject*>& GetObjectList() { return ObjectList; }

			virtual void AddObject(UClass* _ObjetClass, UObject* _ObjectToAdd)
			{
				if (!ObjectListNames.Contains(_ObjectToAdd->GetFName()))
				{
					ObjectClass = _ObjetClass;
					ObjectListNames.Add(_ObjectToAdd->GetFName());
					ObjectList.Add(_ObjectToAdd);
				}
			}

			virtual void RemoveObject(UObject* _ObjectToRemove)
			{
				ObjectClass = nullptr;
				ObjectListNames.Remove(_ObjectToRemove->GetFName());
				ObjectList.Remove(_ObjectToRemove);
			}

		// Member variables
		protected:

			UClass* ObjectClass;

			TSet<FName> ObjectListNames;

			TArray<UObject*> ObjectList;
	};

	template <typename T>
	class TemplateObjectEntry : public ObjectEntry
	{

		// Friend class
		friend class UNWPObjectRegistry;

		// Member functions
		protected:

			const TArray<T>& GetCastObjectList() { return CastObjectList; }

			/// ObjectEntry interface begin
			virtual void AddObject(UClass* _ObjetClass, UObject* _ObjectToAdd)
			{
				BaseType::AddObject(_ObjetClass, _ObjectToAdd);

				CastObjectList.Add(static_cast<T>(_ObjectToAdd));
			}

			virtual void RemoveObject(UObject* _ObjectToRemove) override
			{
				BaseType::RemoveObject(_ObjectToRemove);

				CastObjectList.Remove(static_cast<T>(_ObjectToRemove));
			}
			/// ObjectEntry interface end

		// Member variables
		protected:

			TArray<T> CastObjectList;
	};
	
// Member functions
public:

	///////////////////////////////////////////////////////////////////////////
	// Registry life cycle

	template <typename T>
	void RegisterObject(UObject* _ObjectToRegister)
	{
		UClass* ObjectClass = _ObjectToRegister->GetClass();
		check(ObjectClass);

		if (!ensure(ObjectClass == T::StaticClass()))
		{
			V_LOG(LogNWPObjectRegistry, Warning, TEXT("Inconsistency found! The class passed to the function as template must be the same as the "
				"one passed in _ObjectToRegister"));
			return;
		}

		if (!ClassNameObjectEntryMap.Contains(ObjectClass->GetFName()))
		{
			TemplateObjectEntry<T*>* CurrentObjectEntry = new TemplateObjectEntry<T*>();
			ClassNameObjectEntryMap.Emplace(ObjectClass->GetFName(), TUniquePtr<ObjectEntry>(CurrentObjectEntry));
		}

		ObjectEntry* CurrentObjectEntry = ClassNameObjectEntryMap[ObjectClass->GetFName()].Get();
		check(CurrentObjectEntry);

		CurrentObjectEntry->AddObject(ObjectClass, _ObjectToRegister);
	}

	void UnregisterObject(UObject* _ObjectToUnregister)
	{
		UClass* ObjectClass = _ObjectToUnregister->GetClass();
		check(ObjectClass);

		ObjectEntry* CurrentObjectEntry = nullptr;

		if (ClassNameObjectEntryMap.Contains(ObjectClass->GetFName()))
		{
			CurrentObjectEntry = ClassNameObjectEntryMap[ObjectClass->GetFName()].Get();
			CurrentObjectEntry->RemoveObject(_ObjectToUnregister);
		}

		if (CurrentObjectEntry && CurrentObjectEntry->GetObjectCount() == 0)
		{
			ClassNameObjectEntryMap.Remove(ObjectClass->GetFName());
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Get Objects - Casted

	template <class T>
	const TArray<T*>& GetRegisteredObjects() const
	{
		UClass* ObjectClass = T::StaticClass();
		check(ObjectClass);

		if (ClassNameObjectEntryMap.Contains(ObjectClass->GetFName()))
		{
			TemplateObjectEntry<T*>* ObjectEntry = static_cast<TemplateObjectEntry<T*>*>(ClassNameObjectEntryMap[ObjectClass->GetFName()].Get());
			check(ObjectEntry);
			return ObjectEntry->GetCastObjectList();
		}

		static const TArray<T*> DummyList;

		return DummyList;
	}

	template <class T>
	TArray<T*> GetRegisteredObjectsCopy() const
	{
		return GetRegisteredObjects<T>();
	}

	///////////////////////////////////////////////////////////////////////////
	// Get Objects - As Objects

	const TArray<UObject*>& GetRegisteredObjectsAsObjects(UClass* _RegisteredClass) const
	{
		if (ClassNameObjectEntryMap.Contains(_RegisteredClass->GetFName()))
		{
			return ClassNameObjectEntryMap[_RegisteredClass->GetFName()].Get()->GetObjectList();
		}

		static const TArray<UObject*> DummyList;

		return DummyList;
	}

	TArray<UObject*> GetRegisteredObjectsAsObjectsCopy(UClass* _RegisteredClass) const
	{
		return GetRegisteredObjectsAsObjects(_RegisteredClass);
	}

	template <class T>
	const TArray<UObject*>& GetRegisteredObjectsAsObjects() const
	{
		return GetRegisteredObjectsAsObjects(T::StaticClass());
	}

	template <class T>
	TArray<UObject*> GetRegisteredObjectsAsObjectsCopy() const
	{
		return GetRegisteredObjectsAsObjects<T>();
	}

// Member variables
protected:

	TMap<FName, TUniquePtr<ObjectEntry>> ClassNameObjectEntryMap;
};

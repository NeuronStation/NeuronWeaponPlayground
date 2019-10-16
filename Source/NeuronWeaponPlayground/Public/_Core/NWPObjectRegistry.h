// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NeuronWeaponPlayground.h"

// Generated
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NWPObjectRegistry.generated.h"

// Types
DECLARE_LOG_CATEGORY_EXTERN(LogNWPObjectRegistry, Log, All);

// Delegates
DECLARE_EVENT_TwoParams(UNWPObjectRegistry, FOnObjectRegisteredEventDelegate, UClass*, UObject*);
DECLARE_EVENT_TwoParams(UNWPObjectRegistry, FOnObjectUnregisteredEventDelegate, UClass*, UObject*);

/**
 * Class that associates class names with object lists and manages them. 
 *
 * It is useful to be able to get all the objects of a class and then for example loop them.
 * If you want to filter the object list, you must first request a copy of the object list and apply a predicate using the RemoveAll function as indicated here:
 * https://docs.unrealengine.com/en-US/Programming/UnrealArchitecture/TArrays/index.html
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPObjectRegistry : public UObject
{
	GENERATED_BODY()

// Types
protected:

	// Class containing information about an object list
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

			// Returns the class of the registered objects
			UClass* GetObjectClass() { return ObjectClass; }

			// Returns the number of the registered objects 
			uint32 GetObjectCount() const { return ObjectList.Num(); }

			// Returns the object list
			const TArray<UObject*>& GetObjectList() { return ObjectList; }

			// Adds an object to the object list
			virtual bool AddObject(UClass* _ObjetClass, UObject* _ObjectToAdd)
			{
				if (!ObjectListNames.Contains(_ObjectToAdd->GetFName()))
				{
					ObjectClass = _ObjetClass;
					ObjectListNames.Add(_ObjectToAdd->GetFName());
					ObjectList.Add(_ObjectToAdd);

					return true;
				}

				return false;
			}

			// Removes an object from the object list
			virtual void RemoveObject(UObject* _ObjectToRemove)
			{
				ObjectClass = nullptr;
				ObjectListNames.Remove(_ObjectToRemove->GetFName());
				ObjectList.Remove(_ObjectToRemove);
			}

		// Member variables
		protected:

			// Class of the registered objects
			UClass* ObjectClass;

			// Set of names of the registered objects. Used to prevent an object from being registered more than once
			TSet<FName> ObjectListNames;

			// List of objects currently registered
			TArray<UObject*> ObjectList;
	};

	// ObjectEntry templated version. Used to store a cast registered object list. Objects are cast to their most atomic type
	template <typename T>
	class TemplateObjectEntry : public ObjectEntry
	{

		// Friend class
		friend class UNWPObjectRegistry;

		// Member functions
		protected:

			// Returns the cast object list
			const TArray<T>& GetCastObjectList() { return CastObjectList; }

			/// ObjectEntry interface begin
			virtual bool AddObject(UClass* _ObjetClass, UObject* _ObjectToAdd)
			{
				bool bObjectSuccessfullyAdded = BaseType::AddObject(_ObjetClass, _ObjectToAdd);

				CastObjectList.Add(static_cast<T>(_ObjectToAdd));

				return bObjectSuccessfullyAdded;
			}

			virtual void RemoveObject(UObject* _ObjectToRemove) override
			{
				BaseType::RemoveObject(_ObjectToRemove);

				CastObjectList.Remove(static_cast<T>(_ObjectToRemove));
			}
			/// ObjectEntry interface end

		// Member variables
		protected:

			// List of objects currently registered. In this list the objects are stored cast to their most atomic type
			TArray<T> CastObjectList;
	};

// Events / Delegates
public:

	// Returns a reference to the object registered event delegate
	FOnObjectRegisteredEventDelegate& OnObjectRegistered() { return OnObjectRegisteredEventDelegate; }

	// Returns a reference to the object unregistered event delegate
	FOnObjectUnregisteredEventDelegate& OnObjectUnregistered() { return OnObjectUnregisteredEventDelegate; }

protected:

	// Event delegate called when an object is registered
	FOnObjectRegisteredEventDelegate OnObjectRegisteredEventDelegate;

	// Event delegate called when an object is unregistered
	FOnObjectUnregisteredEventDelegate OnObjectUnregisteredEventDelegate;
	
// Member functions
public:

	///////////////////////////////////////////////////////////////////////////
	// Registry life cycle

	// Registers an object to the object registry. Note: The class passed to the function as template must be the same or a child as the one passed in _ObjectToRegister
	template <typename T>
	void RegisterObject(UObject* _ObjectToRegister)
	{
		UClass* StaticObjectClass = T::StaticClass();
		check(StaticObjectClass);
		UClass* ObjectClass = _ObjectToRegister->GetClass();
		check(ObjectClass);

		// Check if the class passed to the function as template is the same or a child as the one passed in _ObjectToRegister (potential inconsistency)
		if (!ensure(ObjectClass == StaticObjectClass || ObjectClass->IsChildOf(StaticObjectClass)))
		{
			V_LOG(LogNWPObjectRegistry, Warning, TEXT("Inconsistency found! The class passed to the function as template must be the same or a child as the "
				"one passed in _ObjectToRegister"));
			return;
		}

		// Create the object entry if it does not exist
		if (!ClassNameObjectEntryMap.Contains(StaticObjectClass->GetFName()))
		{
			TemplateObjectEntry<T*>* CurrentObjectEntry = new TemplateObjectEntry<T*>();
			ClassNameObjectEntryMap.Emplace(StaticObjectClass->GetFName(), TUniquePtr<ObjectEntry>(CurrentObjectEntry));
		}

		// Add the object to the object entry
		ObjectEntry* CurrentObjectEntry = ClassNameObjectEntryMap[StaticObjectClass->GetFName()].Get();
		check(CurrentObjectEntry);
		bool bObjectSuccessfullyAdded = CurrentObjectEntry->AddObject(StaticObjectClass, _ObjectToRegister);

		// Broadcast the event delegate if the object was added successfully
		if (bObjectSuccessfullyAdded)
		{
			OnObjectRegisteredEventDelegate.Broadcast(StaticObjectClass, _ObjectToRegister);
		}
	}

	// Unregisters an object from the object registry
	template <typename T>
	void UnregisterObject(UObject* _ObjectToUnregister)
	{
		UClass* StaticObjectClass = T::StaticClass();
		check(StaticObjectClass);
		UClass* ObjectClass = _ObjectToUnregister->GetClass();
		check(ObjectClass);
		ObjectEntry* CurrentObjectEntry = nullptr;

		// Check if the class passed to the function as template is the same or a child as the one passed in _ObjectToUnregister (potential inconsistency)
		if (!ensure(ObjectClass == StaticObjectClass || ObjectClass->IsChildOf(StaticObjectClass)))
		{
			V_LOG(LogNWPObjectRegistry, Warning, TEXT("Inconsistency found! The class passed to the function as template must be the same or a child as the "
				"one passed in _ObjectToRegister"));
			return;
		}

		// Remove the object from the object entry if it exists
		if (ClassNameObjectEntryMap.Contains(StaticObjectClass->GetFName()))
		{
			CurrentObjectEntry = ClassNameObjectEntryMap[StaticObjectClass->GetFName()].Get();
			CurrentObjectEntry->RemoveObject(_ObjectToUnregister);
			OnObjectUnregisteredEventDelegate.Broadcast(StaticObjectClass, _ObjectToUnregister);
		}

		// Remove the object entry if it exists and if the object list is empty
		if (CurrentObjectEntry && CurrentObjectEntry->GetObjectCount() == 0)
		{
			ClassNameObjectEntryMap.Remove(StaticObjectClass->GetFName());
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Get Objects - Casted

	// Returns the registered objects by template as a cast object list. Returns the original list by constant reference
	template <typename T>
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

	// Returns the registered objects by template as a cast object list. Returns a copy of the original list
	template <typename T>
	TArray<T*> GetRegisteredObjectsCopy() const
	{
		return GetRegisteredObjects<T>();
	}

	///////////////////////////////////////////////////////////////////////////
	// Get Objects - As Objects

	// Returns the registered objects by UClass as an object list. Returns the original list by constant reference
	const TArray<UObject*>& GetRegisteredObjectsAsObjects(UClass* _RegisteredClass) const
	{
		if (ClassNameObjectEntryMap.Contains(_RegisteredClass->GetFName()))
		{
			return ClassNameObjectEntryMap[_RegisteredClass->GetFName()].Get()->GetObjectList();
		}

		static const TArray<UObject*> DummyList;

		return DummyList;
	}

	// Returns the registered objects by UClass as an object list. Returns a copy of the original list
	TArray<UObject*> GetRegisteredObjectsAsObjectsCopy(UClass* _RegisteredClass) const
	{
		return GetRegisteredObjectsAsObjects(_RegisteredClass);
	}

	// Returns the registered objects by template as an object list. Returns the original list by constant reference
	template <typename T>
	const TArray<UObject*>& GetRegisteredObjectsAsObjects() const
	{
		return GetRegisteredObjectsAsObjects(T::StaticClass());
	}

	// Returns the registered objects by template as an object list. Returns a copy of the original list
	template <typename T>
	TArray<UObject*> GetRegisteredObjectsAsObjectsCopy() const
	{
		return GetRegisteredObjectsAsObjects<T>();
	}

// Member variables
protected:

	// Map that associates class names with object entries (list of objects)
	TMap<FName, TUniquePtr<ObjectEntry>> ClassNameObjectEntryMap;
};

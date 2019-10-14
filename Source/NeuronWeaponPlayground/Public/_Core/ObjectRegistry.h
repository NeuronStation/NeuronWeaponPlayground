// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectRegistry.generated.h"

struct ObjectEntry
{
	UClass* ObjectClass;

	TArray<UObject*> ObjectList;
};

template <typename T>
struct TemplateObjectEntry : ObjectEntry
{
	TArray<T> CastObjectList;
};

/**
 * Object that associates class names with object lists and manages them.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPObjectRegistry : public UObject
{
	GENERATED_BODY()
	
// Member functions
public:

	///////////////////////////////////////////////////////////////////////////
	// Registry life cycle

	template <typename T>
	FORCEINLINE void RegisterObject(UObject* _ObjectToRegister)
	{
		UClass* ObjectClass = _ObjectToRegister->GetClass();
		check(ObjectClass);

		if (!ensure(ObjectClass == T::StaticClass()))
		{
			return;
		}

		if (!ClassNameObjectEntryMap.Contains(ObjectClass->GetFName()))
		{
			TemplateObjectEntry<T*>* ObjectEntry = new TemplateObjectEntry<T*>();
			ClassNameObjectEntryMap.Emplace(ObjectClass->GetFName(), ObjectEntry);
		}

		TemplateObjectEntry<T*>* ObjectEntry = static_cast<TemplateObjectEntry<T*>*>(ClassNameObjectEntryMap[ObjectClass->GetFName()]);
		ObjectEntry->ObjectClass = ObjectClass;
		ObjectEntry->CastObjectList.Add(static_cast<T*>(_ObjectToRegister));
		ObjectEntry->ObjectList.Add(_ObjectToRegister);
	}


	//void UnregisterObject(UObject* _ObjectToUnregister);

	///////////////////////////////////////////////////////////////////////////
	// Get Objects - Casted

	template <class T>
	const TArray<T*>& GetRegisteredObjects(UClass* _RegisteredClass) const
	{
		if (ClassNameObjectEntryMap.Contains(_RegisteredClass->GetFName()))
		{
			TemplateObjectEntry<T*>* ObjectEntry = static_cast<TemplateObjectEntry<T*>*>(ClassNameObjectEntryMap[_RegisteredClass->GetFName()]);
			return ObjectEntry->CastObjectList;
		}

		static const TArray<T*> DummyList;

		return DummyList;
	}

	template <class T>
	TArray<T*> GetRegisteredObjectsCopy(UClass* _RegisteredClass) const
	{
		return GetRegisteredObjects<T>(_RegisteredClass);
	}

	template <class T>
	const TArray<T*>& GetRegisteredObjects() const
	{
		return GetRegisteredObjects<T>(T::StaticClass());
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
			return ClassNameObjectEntryMap[_RegisteredClass->GetFName()]->ObjectList;
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
		GetRegisteredObjectsAsObjects(T::StaticClass());
	}

	template <class T>
	TArray<UObject*> GetRegisteredObjectsAsObjectsCopy() const
	{
		return GetRegisteredObjectsAsObjects<T>();
	}

// Member variables
protected:

	TMap<FName, ObjectEntry*> ClassNameObjectEntryMap;
};

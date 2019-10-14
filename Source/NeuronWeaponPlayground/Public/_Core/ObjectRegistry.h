// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectRegistry.generated.h"

struct ObjectEntry
{
	UClass* ObjectClass;
};

template <typename T>
struct TemplateObjectEntry : ObjectEntry
{
	TArray<T> CastObjectList;

	TArray<UObject*> ObjectList;
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

	template <class T>
	const TArray<T*>& GetRegisteredObjects(UClass* _RegisteredClass) const
	{
		UClass* ObjectClass = _RegisteredClass;

		if (ClassNameObjectEntryMap.Contains(ObjectClass->GetFName()))
		{
			TemplateObjectEntry<T*>* ObjectEntry = static_cast<TemplateObjectEntry<T*>*>(ClassNameObjectEntryMap[ObjectClass->GetFName()]);
			return ObjectEntry->CastObjectList;
		}

		static const TArray<T*> DummyList;

		return DummyList;
	}

	template <class T>
	const TArray<T*>& GetRegisteredObjects() const
	{
		return GetRegisteredObjects<T>(T::StaticClass());
	}

	//template <class T>
	//TArray<UObject*> GetRegisteredObjectList<T>() const;

	//TArray<UObject*> GetRegisteredObjectList(UClass* _RegisteredClass) const;

// Member variables
protected:

	TMap<FName, ObjectEntry*> ClassNameObjectEntryMap;
};

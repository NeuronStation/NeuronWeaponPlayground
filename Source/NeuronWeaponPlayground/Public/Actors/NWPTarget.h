// // Plugin that implements a simple and scalable weapon system developed by Neuron Station

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "NWPTarget.generated.h"

/**
 * Actor that represents the target that the smart weapon projectiles can follow
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API ANWPTarget : public AStaticMeshActor
{
	GENERATED_BODY()
};

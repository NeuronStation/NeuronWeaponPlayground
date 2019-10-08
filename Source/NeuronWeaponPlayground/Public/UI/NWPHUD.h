// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NWPHUD.generated.h"

/**
 * Player's HUD. Shows information about the current equipped weapon 
 *
 * TODO: [NWP-REVIEW] The target system is dependent from resolution. It should not be like that because players with lower resolutions will
 * have a bigger target area. That will be a disadvantage for players with higher resolutions. It has not been solved due to lack of time
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API ANWPHUD : public AHUD
{
	GENERATED_BODY()

// Constructors
public:

	ANWPHUD();

// Member functions
public:

	/// AHUD interface begin
	// Primary draw call for the HUD
	virtual void DrawHUD() override;
	/// AHUD interface end

	// Draws an empty rectangle on the HUD
	void DrawEmptyRect(FLinearColor RectColor, float ScreenX, float ScreenY, float ScreenW, float ScreenH, 
		float LineThickness = 0.0f, float OffsetX = 0.0f, float OffsetY = 0.0f);
};

// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPHUD.h"

// UE
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

// NWP
#include "NWPCharacter.h"
#include "NWPSmartWeapon.h"
#include "NWPSmartWeaponConfig.h"
#include "NWPUtils.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPHUD::DrawHUD()
{
	Super::DrawHUD();

	ANWPCharacter* OwningCharacter = Cast<ANWPCharacter>(GetOwningPawn());

	if (OwningCharacter)
	{
		ANWPWeapon* CurrentWeapon = OwningCharacter->GetWeapon();

		if (CurrentWeapon)
		{
			const UNWPWeaponConfig* CurrentWeaponConfig = CurrentWeapon->GetWeaponConfig();

			if (CurrentWeaponConfig)
			{
				// Draw weapon info
				FString WeaponName = CurrentWeapon->GetName();
				FString WeaponConfigName = CurrentWeapon->GetWeaponConfig()->GetName();
				FString WeaponCompatibilityCadenceType = UNWPUtils::GetEnumName(TEXT("ENWPWeaponCadenceConfig"), CurrentWeapon->GetWeaponConfig()->GetCadenceConfig());
				FString WeaponCurrentCadenceType = UNWPUtils::GetEnumName(TEXT("ENWPWeaponCadenceType"), CurrentWeapon->GetCurrentConfiguredCadenceType());
				FString WeaponState = UNWPUtils::GetEnumName(TEXT("ENWPWeaponState"), CurrentWeapon->GetWeaponState());
				bool bIsSmartWeapon = CurrentWeapon->IsA<ANWPSmartWeapon>();

				const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
				FString WeaponInfo = FString::Printf(TEXT("Weapon: %s\nWeapon Config: %s\nWeapon Cadence Config: %s\n"
					"Weapon Cadence Type: %s\nWeapon State: %s\nAmmo In Magazine: %d\nAmmo: %d\nAmmo Capacity: %d\nCooldown: %.2f\nIs Smart Weapon: %s"),
					*WeaponName, *WeaponConfigName, *WeaponCompatibilityCadenceType, *WeaponCurrentCadenceType, *WeaponState,
					CurrentWeapon->GetCurrentAmmoInMagazine(), CurrentWeapon->GetCurrentAmmo(), CurrentWeaponConfig->GetMaximumAmmo(), CurrentWeapon->GetCurrentCoolDown(),
					bIsSmartWeapon ? TEXT("Yes") : TEXT("No"));

				DrawRect(FLinearColor(0.3f, 0.3f, 0.3f, 0.5), 8.0f, 8.0f, 350.0f, 153.0f);
				DrawText(WeaponInfo, FLinearColor::White, 10.0f, 10.0f);

				// Check if the weapon is a smart weapon
				if (bIsSmartWeapon)
				{
					ANWPSmartWeapon* SmartWeapon = Cast<ANWPSmartWeapon>(CurrentWeapon);

					// Draw the target area
					DrawEmptyRect(FLinearColor::Red, SmartWeapon->GetTargetAreaBeginPosition().X, SmartWeapon->GetTargetAreaBeginPosition().Y,
						SmartWeapon->GetTargetArea().X, SmartWeapon->GetTargetArea().Y, 1.0f, 30.0f, 30.0f);

					// Draw the target locks
					APlayerController* OwnerPlayerController = Cast<APlayerController>(OwningCharacter->GetController());
					const TArray<AActor*> CurrentTargets = SmartWeapon->GetCurrentTargets();
					const UNWPSmartWeaponConfig* SmartWeaponConfig = SmartWeapon->GetSmartWeaponConfig();

					// Check if there is a player controller, if there is any target & if the weapon is configured
					if (OwnerPlayerController && CurrentTargets.Num() > 0 && SmartWeaponConfig)
					{
						for (int32 Index = 0; Index < CurrentTargets.Num(); ++Index)
						{
							// Transform world coordinates to screen coordinates
							FVector2D TargetScreenLocation;

							if (CurrentTargets[Index])
							{
								OwnerPlayerController->ProjectWorldLocationToScreen(CurrentTargets[Index]->GetActorLocation(), TargetScreenLocation);

								// Draw the target lock using the lock size
								DrawEmptyRect(FLinearColor::Red, TargetScreenLocation.X - SmartWeaponConfig->GetTargetLockSize() / 2,
									TargetScreenLocation.Y - SmartWeaponConfig->GetTargetLockSize() / 2, SmartWeaponConfig->GetTargetLockSize(), SmartWeaponConfig->GetTargetLockSize(), 1.0f);
							}		
						}
					}
				}
			}
		}
	}
	
	// Draw very simple crosshair
	DrawLine((Canvas->ClipX * 0.5f) - 20.0f, Canvas->ClipY * 0.5f, (Canvas->ClipX * 0.5f) + 20.0f, Canvas->ClipY * 0.5f, FLinearColor::Red);
	DrawLine(Canvas->ClipX * 0.5f, (Canvas->ClipY * 0.5f) - 20.0f, Canvas->ClipX * 0.5f, (Canvas->ClipY * 0.5f) + 20.0f, FLinearColor::Red);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPHUD::DrawEmptyRect(FLinearColor RectColor, float ScreenX, float ScreenY, float ScreenW, float ScreenH, float LineThickness, float OffsetX, float OffsetY)
{
	// Top line
	DrawLine(ScreenX + OffsetX, ScreenY, ScreenX + ScreenW - OffsetX, ScreenY, RectColor, LineThickness);

	// Right line
	DrawLine(ScreenX + ScreenW, ScreenY + OffsetY, ScreenX + ScreenW, ScreenY + ScreenH - OffsetY, RectColor, LineThickness);

	// Bottom line
	DrawLine(ScreenX + OffsetX, ScreenY + ScreenH, ScreenX + ScreenW - OffsetX, ScreenY + ScreenH, RectColor, LineThickness);

	// Left line
	DrawLine(ScreenX, ScreenY + OffsetY, ScreenX, ScreenY + ScreenH - OffsetY, RectColor, LineThickness);
}

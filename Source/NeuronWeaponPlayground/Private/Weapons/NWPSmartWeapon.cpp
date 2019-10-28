// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPSmartWeapon.h"

// UE
#include "GameFramework/PlayerController.h"
#include "Engine/UserInterfaceSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// NWP
#include "NWPGameplayStatics.h"
#include "NWPGameState.h"
#include "NWPObjectRegistryComponent.h"
#include "NWPWeaponTargetComponent.h"
#include "NWPObjectRegistry.h"
#include "NWPUtils.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPSmartWeapon::ANWPSmartWeapon(const class FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer),
	TargetAreaBeginPosition(FVector2D::ZeroVector),
	TargetArea(FVector2D::ZeroVector),
	CurrentUpdateProjectilesTime(0.0f)
{
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Calculate the viewport target positions
	CalculateViewportTargetPositions();

	// Execute the update methods 
	UpdateTargets();

	UpdateSmartProjectiles(DeltaSeconds);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::OnWeaponLoaded()
{
	Super::OnWeaponLoaded();

	// Calculate the viewport target positions
	CalculateViewportTargetPositions();
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

bool ANWPSmartWeapon::HasTargetToShoot()
{
	return CurrentTargets.Num() != 0;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

const AActor* ANWPSmartWeapon::GetTargetToShoot()
{
	// Check if there is any target
	if (CurrentTargets.Num() == 0)
	{
		return nullptr;
	}

	//// Simple heuristic, random between the current targets
	//return CurrentTargets[FMath::RandRange(0, CurrentTargets.Num() - 1)];

	// Better heuristic: Select the target which, from its position, has the smallest angle with respect to the weapon forward
	int32 SelectedTargetIndex = -1;
	float BestAngle = MAX_flt;

	for (int32 Index = 0; Index < CurrentTargets.Num(); ++Index)
	{
		FVector TargetToFromWeaponToTarget = (CurrentTargets[Index]->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		float AngleToProcess = FMath::RadiansToDegrees(FMath::Acos(GetActorForwardVector() | TargetToFromWeaponToTarget));

		if (AngleToProcess < BestAngle)
		{
			BestAngle = AngleToProcess;
			SelectedTargetIndex = Index;
		}
	}

	return CurrentTargets[SelectedTargetIndex];
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::CalculateViewportTargetPositions()
{
	const UNWPSmartWeaponConfig* SmartWeaponConfig = GetSmartWeaponConfig();

	// Early return if invalid weapon config
	if (!SmartWeaponConfig)
	{
		return;
	}

	// Evaluate if the potential targets are inside the target area
	if (OwnerCharacter)
	{
		APlayerController* OwnerPlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

		if (OwnerPlayerController)
		{
			int32 ViewportSizeX = -1.0f;
			int32 ViewportSizeY = -1.0f;

			OwnerPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

			// TODO: [NWP-REVIEW] Use values that are independent from resolution or develop some scale system

			// Check if valid viewport size
			if (ViewportSizeX > 0 && ViewportSizeY > 0)
			{
				// Calculate the target area positions
				TargetAreaBeginPosition.X = (ViewportSizeX - SmartWeaponConfig->GetHorizontalTargetArea()) / 2;
				TargetAreaBeginPosition.Y = (ViewportSizeY - SmartWeaponConfig->GetVerticalTargetArea()) / 2;
				TargetArea.X = SmartWeaponConfig->GetHorizontalTargetArea();
				TargetArea.Y = SmartWeaponConfig->GetVerticalTargetArea();
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::UpdateTargets()
{
	UWorld* World = GetWorld();

	// Query the weapon target components
	UNWPObjectRegistryComponent* ObjectRegistryComponent = UNWPGameplayStatics::GetNWPGameState(this)->GetObjectRegistryComponent();
	check(ObjectRegistryComponent && ObjectRegistryComponent->GetObjectRegistry());
	const TArray<UNWPWeaponTargetComponent*>&  WeaponTargetComponents = ObjectRegistryComponent->GetObjectRegistry()->GetRegisteredObjects<UNWPWeaponTargetComponent>();

	// Evaluate if the potential targets are inside the target area & the ones that left the target area
	TArray<AActor*> TargetsToRemoveFromCache;

	for (int32 Index = 0; Index < WeaponTargetComponents.Num(); ++Index)
	{
		AActor* WeaponTargetComponentOwner = WeaponTargetComponents[Index]->GetOwner();
		check(WeaponTargetComponentOwner);

		// TODO: [NWP-REVIEW] Consider checking bounding box projection onto the viewport
		if (IsActorInsideTargetArea(WeaponTargetComponentOwner))
		{
			CurrentTargets.AddUnique(WeaponTargetComponentOwner);
		}
		else if(CurrentTargets.Contains(WeaponTargetComponentOwner))
		{
			TargetsToRemoveFromCache.AddUnique(WeaponTargetComponentOwner);
		}
	}

	// Remove the targets that left the target area
	for (int32 Index = 0; Index < TargetsToRemoveFromCache.Num(); ++Index)
	{
		CurrentTargets.RemoveSingle(TargetsToRemoveFromCache[Index]);
	}
	TargetsToRemoveFromCache.Empty();
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

bool ANWPSmartWeapon::IsActorInsideTargetArea(class AActor* ActorToEvaluate)
{
	APlayerController* OwnerPlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	FVector2D TargetScreenLocation;

	// Transform world coordinates to screen coordinates
	OwnerPlayerController->ProjectWorldLocationToScreen(ActorToEvaluate->GetActorLocation(), TargetScreenLocation);

	return (TargetScreenLocation.X > TargetAreaBeginPosition.X  && TargetScreenLocation.X < TargetAreaBeginPosition.X + TargetArea.X) &&
		(TargetScreenLocation.Y > TargetAreaBeginPosition.Y && TargetScreenLocation.Y < TargetAreaBeginPosition.Y + TargetArea.Y);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::SpawProjectile()
{
	Super::SpawProjectile();

	const UNWPSmartWeaponConfig* SmartWeaponConfig = GetSmartWeaponConfig();

	// Early return if invalid weapon config or no projectile configured
	if (!SmartWeaponConfig || !SmartWeaponConfig->GetDefaultProjectileClass().Get())
	{
		return;
	}

	checkf(SmartWeaponConfig->ShouldUseProjectileAsAmmo(), TEXT("ANWPSmartWeapon::SpawProjectile: Smart Weapons should always use projectiles"));

	// Add the last spawned actor to the map if there is at least one target
	if (HasTargetToShoot())
	{
		SmartProjectiles.Add(CurrentSpawnedProjectiles[CurrentSpawnedProjectiles.Num() - 1], FNWPSmartProjectileData(GetTargetToShoot()));
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

FVector ANWPSmartWeapon::GetAvoidObstaclePoint(class ANWPProjectile* _ProjectileToProcess, class AActor* TargetObstacle)
{
	// TODO: [NWP-REVIEW] This heuristic is simple and work most of the times
	// In order to have a better heuristic, i will need more time and iterate more on the Gameplay mechanic
	// Consider to add some noise to the selected point

	const UNWPSmartWeaponConfig* SmartWeaponConfig = GetSmartWeaponConfig();

	// Early return if no projectile, smart weapon config or the projectile is not in the map
	if (!_ProjectileToProcess || !SmartWeaponConfig || !SmartProjectiles.Contains(_ProjectileToProcess))
	{
		return FVector::ZeroVector;
	}

	// Get the bounding box
	FBox HitActorBoundingBox = TargetObstacle->GetComponentsBoundingBox();
	TArray<FVector> BoundsPoints;

	// Calculate the diagonal of the bounding box in XY
	FVector BoundingBoxSize = HitActorBoundingBox.GetSize();
	float HalfBoundingBoxXYDiagonal = FMath::Sqrt(BoundingBoxSize.X * BoundingBoxSize.X + BoundingBoxSize.Y * BoundingBoxSize.Y) / 2.0f;

	// Calculate relevant points
	FVector RightPoint = TargetObstacle->GetActorLocation() + (_ProjectileToProcess->GetActorRightVector() * (HalfBoundingBoxXYDiagonal + SmartWeaponConfig->GetAvoidObstacleHorizontalOffset()));
	FVector LeftPoint = TargetObstacle->GetActorLocation() - (_ProjectileToProcess->GetActorRightVector() * (HalfBoundingBoxXYDiagonal + SmartWeaponConfig->GetAvoidObstacleHorizontalOffset()));
	FVector UpPoint = TargetObstacle->GetActorLocation() + (TargetObstacle->GetActorUpVector() * (BoundingBoxSize.Z / 2.0f + SmartWeaponConfig->GetAvoidObstacleVerticalOffset()));

	// Get the closest relevant point to the projectile
	TArray<FVector> RelevantPoints;
	int32 SelectedPoint = -1.0f;
	float CurrentBestSquareDistance = -1.0f;

	// Create a list of relevant points
	RelevantPoints.Add(RightPoint);
	RelevantPoints.Add(LeftPoint);
	RelevantPoints.Add(UpPoint);

	// Loop the relevant points list and found the closest one
	for (int32 Index = 0; Index < RelevantPoints.Num(); ++Index)
	{
		float DistanceToCheck = (RelevantPoints[Index] - _ProjectileToProcess->GetActorLocation()).SizeSquared();

		if (SelectedPoint < 0)
		{
			SelectedPoint = Index;
			CurrentBestSquareDistance = DistanceToCheck;
		}
		else
		{
			// Evaluate if the point is closer than the current one
			if (DistanceToCheck < CurrentBestSquareDistance)
			{
				SelectedPoint = Index;
				CurrentBestSquareDistance = DistanceToCheck;
			}
		}
	}

	// Draw some debug data
#if !UE_BUILD_SHIPPING 
	if (CVarbDebugWeapon.GetValueOnGameThread())
	{
		// Obstacle bounding box
		DrawDebugBox(GetWorld(), HitActorBoundingBox.GetCenter(), HitActorBoundingBox.GetExtent(), FColor::Red, true, 1.0f);

		// Relevant points
		DrawDebugSphere(GetWorld(), RightPoint, 20.0f, 10, FColor::Blue, false, 1.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), LeftPoint, 20.0f, 10, FColor::Blue, false, 1.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), UpPoint, 20.0f, 10, FColor::Blue, false, 1.0f, 0, 3.0f);

		// Selected point
		DrawDebugSphere(GetWorld(), RelevantPoints[SelectedPoint], 20.0f, 10, FColor::Green, false, 1.0f, 0, 3.0f);
	}
#endif

	// Return the selected point
	return RelevantPoints[SelectedPoint];
}

void ANWPSmartWeapon::UpdateSmartProjectiles(float DeltaTime)
{
	// Check if we can update the projectiles
	if (CurrentUpdateProjectilesTime != 0.0f)
	{
		return;
	}

	// Update each projectile
	for (auto It = SmartProjectiles.CreateConstIterator(); It; ++It)
	{
		UpdateSmartProjectile(It.Key(), DeltaTime);
	}

	// Recalculate the projectile update time
	CurrentUpdateProjectilesTime = FMath::Max(0.0f, --CurrentUpdateProjectilesTime);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::UpdateSmartProjectile(ANWPProjectile* _ProjectileToProcess, float DeltaTime)
{
	const UNWPSmartWeaponConfig* SmartWeaponConfig = GetSmartWeaponConfig();

	// Early return if no projectile, smart weapon config or the projectile is not in the map
	if (!_ProjectileToProcess || !SmartWeaponConfig || !SmartProjectiles.Contains(_ProjectileToProcess))
	{
		return;
	}

	UWorld* World = GetWorld();
	FNWPSmartProjectileData SmartProjectileData = SmartProjectiles[_ProjectileToProcess];

	// Return if the projectile has hit with something
	if (SmartProjectileData.HasHitWithSomthing())
	{
		return;
	}

	// Evaluate if there is an obstacle in front of the projectile
	FCollisionQueryParams QueryParams;

	// Ignore the projectile, character & weapon
	QueryParams.AddIgnoredActor(_ProjectileToProcess);
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.AddIgnoredActor(OwnerCharacter->GetWeapon());

	// Ignore all the spawned projectiles
	for (int32 Index = 0; Index < CurrentSpawnedProjectiles.Num(); ++Index)
	{
		QueryParams.AddIgnoredActor(CurrentSpawnedProjectiles[Index]);
	}
	QueryParams.bTraceComplex = true;

	FHitResult Hit;

	FVector TargetToFromProjectileToTargetActor = (SmartProjectileData.GetTargetActor()->GetActorLocation() - 
		_ProjectileToProcess->GetActorLocation()).GetSafeNormal();
	FVector ProjectilePosition = _ProjectileToProcess->GetActorLocation();
	FVector EndPosition = _ProjectileToProcess->GetActorLocation() + TargetToFromProjectileToTargetActor * SmartWeaponConfig->GetAvoidObstacleProjectileDistance();

	// Shoot a ray from the projectile to the target
	if (World->LineTraceSingleByChannel(Hit, ProjectilePosition, EndPosition, COLLISION_WEAPON, QueryParams))
	{
		AActor* HitActor = Hit.GetActor();

		// Evaluate the hit actor
		if (HitActor == SmartProjectileData.GetTargetActor())
		{
			// Orientate the projectile to the target
			SmartProjectileData.SetCurrentState(ENWPSmartProjectileState::OrientatingToTarget);
		}
		// Recalculate the avoid point if the obstacle changed
		else if (HitActor != SmartProjectileData.GetTargetObstacle())
		{
			// Set the target obstacle
			SmartProjectileData.SetTargetObstacle(Hit.GetActor());

			// Select the avoid obstacle point
			FVector AvoidObstaclePoint = GetAvoidObstaclePoint(_ProjectileToProcess, HitActor);

			// Set the avoid obstacle point
			SmartProjectileData.SetAvoidObstaclePoint(AvoidObstaclePoint);

			// Orientate the projectile to the avoid obstacle point
			SmartProjectileData.SetCurrentState(ENWPSmartProjectileState::OrientatingToAvoidObstacle);	
		}
	}

	// Apply the smart projectile data to the map
	SmartProjectiles[_ProjectileToProcess] = SmartProjectileData;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::OnProjectileVelocityComputed(ANWPProjectile* _ProjectileToProcess, FVector& _ComputedVelocity, float DeltaTime)
{
	const UNWPSmartWeaponConfig* SmartWeaponConfig = GetSmartWeaponConfig();

	// Early return if invalid weapon config or not smart projectile data
	if (!SmartWeaponConfig || !SmartProjectiles.Contains(_ProjectileToProcess))
	{
		return;
	}

	// Get the projectile velocity & speed
	FVector ProjectileVelocity = _ComputedVelocity;
	float ProjectileSpeed = ProjectileVelocity.Size();

	// Normalize velocity
	ProjectileVelocity = ProjectileVelocity.GetSafeNormal();

	// Get the smart projectile data from the map
	FNWPSmartProjectileData SmartProjectileData = SmartProjectiles[_ProjectileToProcess];

	// Calculate target point & orientation velocity
	FVector TargetPoint = FVector::ZeroVector;
	float OrientationVelocity = -1.0f;

	if (SmartProjectileData.IsOrientatingToAvoidObstacles())
	{
		TargetPoint = SmartProjectileData.GetAvoidObstaclePoint();
		OrientationVelocity = SmartWeaponConfig->GetOrientProjectileToAvoidObstacleVelocity();
	}
	else if (SmartProjectileData.IsOrientatingToTarget())
	{
		TargetPoint = SmartProjectileData.GetTargetActor()->GetActorLocation();
		OrientationVelocity = SmartWeaponConfig->GetOrientProjectileToTargetVelocity();
	}
	else
	{
		return;
	}

	// Recalculate the velocity direction using the target
	FVector TargetToFromProjectileToTargetPoint = (TargetPoint - _ProjectileToProcess->GetActorLocation()).GetSafeNormal();

	// Perform the calculation only if the projectile moves towards the target
	if ((ProjectileVelocity | TargetToFromProjectileToTargetPoint) >= 0.0f)
	{
		_ComputedVelocity = FMath::VInterpConstantTo(ProjectileVelocity, TargetToFromProjectileToTargetPoint,
			DeltaTime, OrientationVelocity) * ProjectileSpeed;
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::OnProjectileHit(class ANWPProjectile* _ProjectileToProcess, class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Early return if no smart projectile data
	if (!SmartProjectiles.Contains(_ProjectileToProcess))
	{
		return;
	}

	// Get the smart projectile data from the map
	FNWPSmartProjectileData SmartProjectileData = SmartProjectiles[_ProjectileToProcess];

	// Change the state to hit with something
	SmartProjectileData.SetCurrentState(ENWPSmartProjectileState::HitWithSomething);

	// Apply the smart projectile data to the map
	SmartProjectiles[_ProjectileToProcess] = SmartProjectileData;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPSmartWeapon::OnProjectileIsGoingToBeDestroyed(ANWPProjectile* _ProjectileToProcess)
{
	// Remove the projectile from the smart projectiles map
	SmartProjectiles.Remove(_ProjectileToProcess);
}

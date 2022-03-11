// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "VoxelWorld.h"
#include <Voxel/Public/VoxelTools/VoxelBlueprintLibrary.h>
#include <Voxel/Public/VoxelData/VoxelData.h>
#include <Voxel/Public/VoxelValue.h>
//#include <Voxel/Public/VoxelTools/VoxelSurfaceTools.h>
//#include <Voxel/Private/VoxelTools/VoxelDataTools.cpp>
#include <Voxel/Public/VoxelTools/VoxelDataTools.h>
//C:\Program Files(x86)\Epic Games\UE_4.27\Engine\Plugins\Marketplace\VoxelFree\Source\Voxel\Public\VoxelTools\VoxelDataTools.h

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) return;

	InputComponent->BindAction("ResetWorld", IE_Pressed, this, &AMyPlayerController::ResetWorld);
	InputComponent->BindAction("GetInfo", IE_Pressed, this, &AMyPlayerController::PrintInfo);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> Results;

	UGameplayStatics::GetAllActorsOfClass(this, AVoxelWorld::StaticClass(), Results);
	if (Results.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VoxelWorld not found"));
		return;
	}
	if (Results.Num() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("More than one VoxelWorld found"));
		return;
	}
	auto FoundVoxelWorld = Cast< AVoxelWorld>(Results[0]);

	VoxelWorld = FoundVoxelWorld;
	UE_LOG(LogTemp, Display, TEXT("VoxelWorld is set %s with Generator:%s"),
		*VoxelWorld->GetName(),
		*VoxelWorld->Generator.GetGenerator()->GetName());
	//IVoxelPool Pool = VoxelWorld->GetPool();
	//UVoxelBlueprintLibrary::CreateWorldVoxelThreadPool(GetWorld(),);
}

void AMyPlayerController::ResetWorld()
{
	UE_LOG(LogTemp, Display, TEXT("VoxelWorld ResetWorld"));
	if (VoxelWorld)
	{
		//VoxelWorld->Destroy();
		//UE_LOG(LogTemp, Display, TEXT("VoxelWorld Destroyed"));

		FVoxelWorldCreateInfo Info;
		VoxelWorld->RecreateAll(Info);// CreateWorld();
		UE_LOG(LogTemp, Display, TEXT("VoxelWorld World ReCreated"));
	}
}

void AMyPlayerController::PrintInfo()
{
	UE_LOG(LogTemp, Display, TEXT("PrintInfo"));
	if (VoxelWorld)
	{
		auto CurrentPawn = GetPawn();
		if (!CurrentPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::PrintInfo() Pawn not exist"));
			return;
		}
		auto PawnLocation = CurrentPawn->GetActorLocation();
		auto VoxelPosition = VoxelWorld->GlobalToLocal(PawnLocation);
		auto VoxelBelowPosition = VoxelWorld->GlobalToLocal(FVector(PawnLocation.X, PawnLocation.Y, PawnLocation.Z - VoxelWorld->VoxelSize));

		float PawnPositionVoxelValue;
		UVoxelDataTools::GetValue(PawnPositionVoxelValue, VoxelWorld, VoxelPosition);

		float BelowPawnPositionVoxelValue;
		UVoxelDataTools::GetValue(BelowPawnPositionVoxelValue, VoxelWorld, VoxelBelowPosition);

		UE_LOG(LogTemp, Warning, TEXT("WorldPosition:%s VoxelPosition:%s Value:%f Below[%s]:%f"),
			*CurrentPawn->GetActorLocation().ToString(),
			*VoxelPosition.ToString(),
			PawnPositionVoxelValue,
			*VoxelBelowPosition.ToString(),
			BelowPawnPositionVoxelValue
		);
	}
}

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
#include <Voxel/Public/VoxelTools/Gen/VoxelSphereTools.h>
#include <Voxel/Public/VoxelTools/VoxelSurfaceEdits.h>
#include <Voxel/Public/VoxelTools/VoxelSurfaceTools.h>
#include <Voxel/Public/VoxelTools/Gen/VoxelSurfaceEditTools.h>
//C:\Program Files(x86)\Epic Games\UE_4.27\Engine\Plugins\Marketplace\VoxelFree\Source\Voxel\Public\VoxelTools\Gen\VoxelSphereTools.h

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) return;

	InputComponent->BindAction("ResetWorld", IE_Pressed, this, &AMyPlayerController::ResetWorld);
	InputComponent->BindAction("GetInfo", IE_Pressed, this, &AMyPlayerController::PrintInfo);
	InputComponent->BindAction("Dig", IE_Pressed, this, &AMyPlayerController::Dig);
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


		/*hein0r — 01 / 13 / 2021
		is there an equivalent to UVoxelDataTools::GetVoxelsValueAndMaterial(TArray<FVoxelValueMaterial>&Voxels, AVoxelWorld * World, const TArray<FIntVector>&Positions) which i can use to SET valuesand materials at once ? so far i set the value and the material seperately for every position which is probably not the fastest it could be...
		Phyronnaz — 01 / 13 / 2021
		Use the data setter directly
		Make sure you lock the data first
		Data.Set<FVoxelValue, FVoxelMaterial>*/
	}
}

void AMyPlayerController::Dig()
{
	UE_LOG(LogTemp, Display, TEXT("Dig"));
	if (VoxelWorld)
	{
		auto CurrentPawn = GetPawn();
		if (!CurrentPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::PrintInfo() Pawn not exist"));
			return;
		}
		auto PawnLocation = CurrentPawn->GetActorLocation();
		auto Location = PawnLocation + CurrentPawn->GetActorForwardVector() * VoxelWorld->VoxelSize;

		float DigRadius = 50;
		UE_LOG(LogTemp, Display, TEXT("AMyPlayerController::Dig() Loc:%s"), *Location.ToString());
		UVoxelSphereTools::RemoveSphere(VoxelWorld, Location, DigRadius);

		FVoxelIntBox BoundsToDoEditsIn = UVoxelBlueprintLibrary::MakeIntBoxFromGlobalPositionAndRadius(VoxelWorld, Location, DigRadius * 1.6f);

		FVoxelSurfaceEditsVoxels Voxels;
		UVoxelSurfaceTools::FindSurfaceVoxelsFromDistanceField(Voxels, VoxelWorld, BoundsToDoEditsIn);

		FVoxelSurfaceEditsStack Stack;
		Stack.Add(UVoxelSurfaceTools::ApplyFalloff(VoxelWorld, EVoxelFalloff::Linear, Location, DigRadius * 1.6f, .5f));
		Stack.Add(UVoxelSurfaceTools::ApplyConstantStrength(-.8f));

		FVoxelSurfaceEditsProcessedVoxels ProcessedVoxels = Stack.Execute(Voxels);
		UVoxelSurfaceEditTools::EditVoxelValues(VoxelWorld, ProcessedVoxels);

		/*float DigSmoothIterations = 4;

		UVoxelSphereTools::SmoothSphere(VoxelWorld, Location, DigRadius * 2, 2000, DigSmoothIterations);*/
	}
}
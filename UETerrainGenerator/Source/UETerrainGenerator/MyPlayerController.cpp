// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "VoxelWorld.h"
#include <Voxel/Public/VoxelTools/VoxelBlueprintLibrary.h>
//#include <Voxel/Public/IVoxelPool.h>
//#include "IVoxelPool.h"

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) return;

	InputComponent->BindAction("ResetWorld", IE_Pressed, this, &AMyPlayerController::ResetWorld);
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
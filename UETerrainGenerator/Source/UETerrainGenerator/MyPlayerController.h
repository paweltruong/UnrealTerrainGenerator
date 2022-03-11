// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AVoxelWorld;
class FVoxelData;
/**
 * 
 */
UCLASS()
class UETERRAINGENERATOR_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	void ResetWorld();

protected:
	virtual void BeginPlay() override;
private:
	AVoxelWorld* VoxelWorld;
	void PrintInfo();
	void Dig();
};

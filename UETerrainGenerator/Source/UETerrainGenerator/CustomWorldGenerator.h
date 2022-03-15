// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "FastNoise/VoxelFastNoise.h"
#include "VoxelGenerators/VoxelGeneratorHelpers.h"
#include "CustomWorldGenerator.generated.h"


UCLASS(Blueprintable)
class UETERRAINGENERATOR_API UCustomWorldGenerator : public UVoxelGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		float NoiseHeight = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		int32 Seed = 1337;

	//~ Begin UVoxelGenerator Interface
	virtual TVoxelSharedRef<FVoxelGeneratorInstance> GetInstance() override;
	//~ End UVoxelGenerator Interface


	UFUNCTION()
	void OnGenerateWorldOccured();
};

class FCustomWorldGeneratorInstance : public TVoxelGeneratorInstanceHelper<FCustomWorldGeneratorInstance, UCustomWorldGenerator>
{
public:
	using Super = TVoxelGeneratorInstanceHelper<FCustomWorldGeneratorInstance, UCustomWorldGenerator>;

	explicit FCustomWorldGeneratorInstance(const UCustomWorldGenerator& MyGenerator);

	//~ Begin FVoxelGeneratorInstance Interface
	virtual void Init(const FVoxelGeneratorInit& InitStruct) override;

	v_flt GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;
	FVoxelMaterial GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;

	TVoxelRange<v_flt> GetValueRangeImpl(const FVoxelIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const;

	virtual FVector GetUpVector(v_flt X, v_flt Y, v_flt Z) const override final;
	//~ End FVoxelGeneratorInstance Interface

private:
	const float NoiseHeight;
	int32 Seed;
	FVoxelFastNoise Noise;


	UFUNCTION()
	void OnGenerateWorldOccured();

};
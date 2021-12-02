// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundsGameMode.h"
#include "V2InfiniteTerrainGameMode.generated.h"

class UActorPool;
/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API AV2InfiniteTerrainGameMode : public ATestingGroundsGameMode
{
	GENERATED_BODY()


public:

	AV2InfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "BoundsPool")
	void PopulateBoundsVolumePool();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavMeshBoundsVolPool")
	UActorPool* NavMeshBoundsVolumePool;

private:
	void AddToPool(class ANavMeshBoundsVolume *VolumeToAdd);
};

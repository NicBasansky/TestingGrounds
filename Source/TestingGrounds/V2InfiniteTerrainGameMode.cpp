// Fill out your copyright notice in the Description page of Project Settings.


#include "V2InfiniteTerrainGameMode.h"
#include "EngineUtils.h"
#include "Components/ActorComponent.h"
#include "ActorPool.h"
#include "NavMesh/NavMeshBoundsVolume.h"

AV2InfiniteTerrainGameMode::AV2InfiniteTerrainGameMode()
{
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(TEXT("Nav Mesh Volume Pool"));
	
}

void AV2InfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (VolumeIterator)
	{
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
}

void AV2InfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume *VolumeToAdd)
{
		NavMeshBoundsVolumePool->AddActorToPool(VolumeToAdd);
}

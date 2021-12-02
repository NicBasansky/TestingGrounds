// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

USTRUCT(BlueprintType)
struct FRandomSpawningVars
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int MinSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int MaxSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float Radius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MinScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MaxScale = 1;
};

class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	FVector NavMeshOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "SpawnActors")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, FRandomSpawningVars RandomSpawningVars);

	UFUNCTION(BlueprintCallable, Category = "SpawnAI")
	void PlaceAIPawns(TSubclassOf<APawn> AISpawn, FRandomSpawningVars RandomSpawningVars);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);

	

private:
			
	void PositionNavMeshBoundsVolume();
	
	bool CanSpawnAtLocation(FVector Location, float Radius);

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, const FRandomSpawningVars RandomSpawningVars);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition);

	void PlaceActor(TSubclassOf<APawn> PawnToSpawn, const FSpawnPosition& SpawnPosition);

	bool FindEmptyLocation(FVector& OUTLocation, float Radius);
	
	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

};


// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Math/Box.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "NavMesh/NavMeshBoundsVolume.h"
//#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "AI/NavigationSystemBase.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	NavMeshOffset = FVector(2000, 0, 0);
	
}

void ATile::SetPool(UActorPool* InPool)	
{	
	Pool = InPool;	
	
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->CheckoutActorFromPool();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Not enough actors in Pool"), *GetName());
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavMeshOffset);
	FNavigationSystem::Build(*GetWorld());
	}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, const FRandomSpawningVars RandomSpawningVars)
{
	//Generate a random number of actors to spawn in a tile
	//The arguments are given in the blueprint when it is called in the event graph
	int NumberToSpawn = FMath::RandRange(RandomSpawningVars.MinSpawn, RandomSpawningVars.MaxSpawn);

	//Spawn the actors
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(RandomSpawningVars.MinScale, RandomSpawningVars.MaxScale);
		bool bFound = FindEmptyLocation(SpawnPosition.Location, RandomSpawningVars.Radius * SpawnPosition.Scale);
		if (bFound)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, FRandomSpawningVars RandomSpawningVars)
{
	RandomlyPlaceActors(ToSpawn, RandomSpawningVars); //called in BP
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> AISpawn, FRandomSpawningVars RandomSpawningVars)
{
	RandomlyPlaceActors(AISpawn, RandomSpawningVars); // called in BP
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0.0f, SpawnPosition.Rotation, 0.0f));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}


void ATile::PlaceActor(TSubclassOf<APawn> PawnToSpawn, const FSpawnPosition& SpawnPosition)
{
	APawn* Pawn = GetWorld()->SpawnActor<APawn>(PawnToSpawn);
	if (Pawn)
	{
		Pawn->SpawnDefaultController();
		Pawn->Tags.Add(FName("Enemy"));
		Pawn->SetActorRelativeLocation(SpawnPosition.Location);
		Pawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Pawn->SetActorRotation(FRotator(0.0f, SpawnPosition.Rotation, 0.0f));
	}
	
}

bool ATile::FindEmptyLocation(FVector& OUTLocation, float Radius)
{
	//Defining the FBox
	FBox Bounds(MinExtent, MaxExtent);
	
	//Find a candidate location for spawning actors
	const int MAX_ATTEMPTS = 5; //WAS PREVIOUSLY 100, AS PER THE TUTORIAL
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OUTLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	
	}
	
void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (Pool != nullptr && NavMeshBoundsVolume != nullptr)
	{
		Pool->ReturnActorToPool(NavMeshBoundsVolume);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Returning: %s"), *NavMeshBoundsVolume->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Number of Actors in Pool: %f"), *Pool->);

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation+FVector(0.1f, 0.f, 0.f), //.1f so the line trace actually sweeps
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;  //Ternary Operator... If true, then select what
																//is before the colon. False, use what is after the colon
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);

	return !HasHit; //the ! is to invert the HasHit, so if there is NO hit, then return true.
					//This is in line with the function title of CanSpawn...
}


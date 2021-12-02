// Fill out your copyright notice in the Description page of Project Settings.


#include "GrassComponent.h"

// Sets default values for this component's properties
UGrassComponent::UGrassComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UGrassComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnFoiliage();
}


// Called every frame
void UGrassComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrassComponent::SpawnFoiliage()
{

	//Generate a random location in the FBox
	//Add an instance of the foiliage and loop it as many times needed as per the SpawnCount
	for (size_t i = 0; i < SpawnCount; i++)
	{
		FVector Location = FMath::RandPointInBox(GrassBoundaries);
		AddInstance(FTransform(Location));
	}

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor* UActorPool::CheckoutActorFromPool()
{
	if (Pool.Num() == 0) //If there are 0 actors in the array
	{
		return nullptr;
	}
	return Pool.Pop(); //return the TArray and there is a pop function--press F12 for info--that 
					//is useful for our purposes. Takes an element out of the array and 
					//returns it.
}

void UActorPool::ReturnActorToPool(AActor* ActorToReturn)
{
	AddActorToPool(ActorToReturn);
}

void UActorPool::AddActorToPool(AActor* ActorToAdd)
{
	if (ActorToAdd == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Added null actor"), *GetName());
		return;
	}
	Pool.Push(ActorToAdd);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelDetailsActor.h"

#include "TokenActorBase.h"
#include "Kismet/GameplayStatics.h"

void ALevelDetailsActor::BeginPlay()
{
	// Get all actors of type token actor

	// Array is a local sink cause dont care about keeping the references
	TArray<AActor*> CoinSink;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATokenActorBase::StaticClass(), CoinSink);
	TotalTokenCount = CoinSink.Num();	
}

// Sets default values
ALevelDetailsActor::ALevelDetailsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
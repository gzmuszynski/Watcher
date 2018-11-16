// Fill out your copyright notice in the Description page of Project Settings.

#include "WatcherComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/PointLightComponent.h"
#include "../Public/WantedComponent.h"
#include "../Public/WatcherComponent.h"


UWatcherComponent::UWatcherComponent() 
{
	SightSense = NewObject<UAISenseConfig_Sight>();

	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = true;
	SightSense->DetectionByAffiliation.bDetectNeutrals = true;

	SightSense->SetMaxAge(0.5f);
	SightSense->PeripheralVisionAngleDegrees = 45.f;

	SensesConfig.Add(SightSense);

	DominantSense = SightSense->GetSenseImplementation();

	UE_LOG(WatcherLog, Log, (TEXT("WatcherComponent set up.")));
}

void UWatcherComponent::Process(UPARAM(ref)AActor* Source, UPARAM(ref) FAIStimulus& Stimulus)
{
	UWantedComponent* Wanted = (UWantedComponent*)Source->GetComponentByClass(UWantedComponent::StaticClass());

	if (Wanted != nullptr)
	{
		bool WasSpotted = Stimulus.WasSuccessfullySensed();

		Wanted->SetSpotted(WasSpotted);
		
		if (WasSpotted == true)
		{
			SpottedActors.Add(Source);
			UE_LOG(WatcherLog, Log, (TEXT("WatcherComponent: Actor Sighted")));
		}
		else if(WasSpotted == false)
		{
			SpottedActors.Remove(Source);
			UE_LOG(WatcherLog, Log, (TEXT("WatcherComponent: Actor Lost")));
		}
		else
		{
			UE_LOG(WatcherLog, Error, TEXT("ERROR: WatcherComponent: Actor in Shroedinger state!"));
		}

	}
	else
	{
		UE_LOG(WatcherLog, Warning, TEXT("Warning: WatcherComponent: Actor does not possess WantedComponent but was still perceived!"));
	}
}

void UWatcherComponent::BeginPlay()
{
	SightSense = NewObject<UAISenseConfig_Sight>();

	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = true;
	SightSense->DetectionByAffiliation.bDetectNeutrals = true;

	SightSense->SetMaxAge(0.5f);
	SightSense->PeripheralVisionAngleDegrees = 45.f;

	SensesConfig.Add(SightSense);

	DominantSense = SightSense->GetSenseImplementation();
	
	UE_LOG(WatcherLog, Log, (TEXT("WatcherComponent set up on beginplay.")));
}

void UWatcherComponent::CheckDistances()
{

	if (SpottedActors.Num() > 0)
	{
		TMap<AActor*, float> ActorMap;
		TArray<float> Distances;


		for (AActor* Actor : SpottedActors)
		{
			float Distance = FVector::Dist(Actor->GetActorLocation(), GetOwner()->GetActorLocation());
			ActorMap.Add(Actor, Distance);

			//Reset All Color
			UWantedComponent* Wanted = (UWantedComponent*)Actor->GetComponentByClass(UWantedComponent::StaticClass());
			Wanted->PointLight->SetLightColor(FLinearColor(1.f, 1.f, 1.f));
		}

		ActorMap.GenerateValueArray(Distances);
		Distances.Sort();

		//Set nearest color
		AActor* NearestActor = *ActorMap.FindKey(Distances.Last());
		UWantedComponent* NearestWanted = (UWantedComponent*)NearestActor->GetComponentByClass(UWantedComponent::StaticClass());
		NearestWanted->PointLight->SetLightColor(FLinearColor(1.f, 0.f, 0.f));

		//Set furthest color
		AActor* FurthestActor = *ActorMap.FindKey(Distances[0]);
		UWantedComponent* FurthestWanted = (UWantedComponent*)FurthestActor->GetComponentByClass(UWantedComponent::StaticClass());
		FurthestWanted->PointLight->SetLightColor(FLinearColor(0.f, 1.f, 0.f));
		UE_LOG(WatcherLog, Log, TEXT("WatcherComponent: Calculating distances"));
	}
	else
	{
		UE_LOG(WatcherLog, Log, TEXT("WatcherComponent: Actor List empty. Aborting distance calculation."));
	}

	GetWorld()->GetTimerManager().SetTimer(Cycle, this, &UWatcherComponent::CheckDistances, SightSense->GetMaxAge(), false);
}

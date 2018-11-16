// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "WatcherComponent.generated.h"

/**
 * 
 */



UCLASS(ClassGroup = (Common), meta = (BlueprintSpawnableComponent))
class WATCHER_API UWatcherComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:

	UWatcherComponent();

	DECLARE_EVENT_OneParam(OnPerceptionUpdated, MakeLight, TArray<AActor*> &)

	UFUNCTION(BlueprintCallable)
	void CheckDistances();

	UFUNCTION(BlueprintCallable)
		void Process(UPARAM(ref) AActor* Source, UPARAM(ref) FAIStimulus& Stimulus);

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UAISenseConfig_Sight* SightSense;
private:

	UPROPERTY()
		TArray<AActor*> SpottedActors;

	FTimerHandle Cycle;

	inline static bool ConstPredicate(const FAIStimulus& ip1, const FAIStimulus& ip2)
	{
		return (ip1.ReceiverLocation-ip1.StimulusLocation).Dist > (ip2.ReceiverLocation - ip2.StimulusLocation).Dist;
	}
	
};

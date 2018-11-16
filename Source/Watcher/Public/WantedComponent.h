// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PointLightComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "WantedComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(WatcherLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WATCHER_API UWantedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWantedComponent();

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UAIPerceptionStimuliSourceComponent* Stimuli;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	UFUNCTION()
		virtual void SetSpotted(bool &IsSpotted);


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};

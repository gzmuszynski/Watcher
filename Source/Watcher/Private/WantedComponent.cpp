// Fill out your copyright notice in the Description page of Project Settings.

#include "WantedComponent.h"
#include "Perception/AISense_Sight.h"
#include "../Public/WantedComponent.h"

DEFINE_LOG_CATEGORY(WatcherLog)
// Sets default values for this component's properties
UWantedComponent::UWantedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("SpottedLightIndicator"));


	Stimuli = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	Stimuli->RegisterForSense(UAISense_Sight::StaticClass());
	Stimuli->bAutoRegister = true;

	UE_LOG(WatcherLog, Log, (TEXT("WantedComponent set up.")));
	// ...
}


// Called when the game starts
void UWantedComponent::BeginPlay()
{
	Super::BeginPlay();
	PointLight->AddRelativeLocation(GetOwner()->GetActorLocation());
	PointLight->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	PointLight->AddRelativeLocation(FVector(0.f, 0.f, 0.f));
	PointLight->SetCastShadows(false);
	PointLight->SetIntensity(1500.f);
	PointLight->SetLightFalloffExponent(500.f);
	PointLight->bUseInverseSquaredFalloff = false;
	PointLight->SetVisibility(false);

	Stimuli->RegisterForSense(UAISense_Sight::StaticClass());
	Stimuli->Activate(true);

	UE_LOG(WatcherLog, Log, (TEXT("WantedComponent set up lights and registered stimuli.")));
	RegisterComponent();
	// ...
	
}


void UWantedComponent::SetSpotted(bool & IsSpotted)
{
	PointLight->SetVisibility(IsSpotted);
}

// Called every frame
void UWantedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


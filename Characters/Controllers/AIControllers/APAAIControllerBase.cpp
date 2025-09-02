#include "APAAIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ProjectPA/Characters/PlayerCharacter.h"
#include "ProjectPA/Characters/NPCs/NPCBase.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"

//~Begin Helper Function
template <EAISenseType SenseType>
FAIStimulus CanSense(AActor* Actor, UAIPerceptionComponent* PerceptionComponent)
{
	check(Actor && PerceptionComponent)

	TSubclassOf<UAISense> SenseClassToCompare;
	
	if constexpr (SenseType == EAISenseType::Hearing)
	{
		SenseClassToCompare = UAISense_Hearing::StaticClass();
	}
	else if constexpr (SenseType == EAISenseType::Sight)
	{
		SenseClassToCompare = UAISense_Sight::StaticClass();
	}
	else if constexpr (SenseType == EAISenseType::Damage)
	{
		SenseClassToCompare = UAISense_Damage::StaticClass();
	}
	
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	PerceptionComponent->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		TSubclassOf<UAISense> LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(Actor, AIStimulus);
		if (SenseClassToCompare == LastSensedStimulusClass)
		{
			return AIStimulus;
		}
	}

	return FAIStimulus();
}
//~End Helper Function

APAAIControllerBase::APAAIControllerBase()
	: SenseSoundActiveTime(5.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	TargetActorBlackboardKey = TEXT("TargetActor");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 3000.f;
		SightConfig->LoseSightRadius = 3500.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(0.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;
	
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		PerceptionComponent->ConfigureSense(*SightConfig);
	}
	
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 1000.0f;
		HearingConfig->SetMaxAge(5.0f);
        
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

		PerceptionComponent->ConfigureSense(*HearingConfig); 
	}
}

void APAAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (PerceptionComponent)
	{
		PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &APAAIControllerBase::AIPerceptionUpdated);
		PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &APAAIControllerBase::OnPerceptionInfoUpdated);
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APAAIControllerBase::OnTargetPerceptionUpdated);
	}

	if (GetBrainComponent())
	{
		GetBrainComponent()->SetComponentTickEnabled(true);
	}

	RunBehaviorTree(BehaviorTree);
	if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BTComp->StartTree(*BehaviorTree);
	}
}

void APAAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UAIPerceptionComponent* Perception = GetPerceptionComponent();
	if (IsValid(Perception))
	{
		Perception->RequestStimuliListenerUpdate();
	}

	if (BrainComponent)
	{
		BrainComponent->StartLogic();
	}
}

void APAAIControllerBase::AIPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	bool bSetTargetActor = false;
	
	for (AActor* Actor : UpdatedActors)
	{
		FAIStimulus AIStimulus;
		AIStimulus = CanSense<EAISenseType::Hearing>(Actor, GetPerceptionComponent());
		if (AIStimulus.IsValid())
		{
			HandleHearingStimulus(Actor, AIStimulus);
		}
		AIStimulus = CanSense<EAISenseType::Sight>(Actor, GetPerceptionComponent());
		if (AIStimulus.IsValid())
		{
			if (HandleSightStimulus(Actor, AIStimulus))
			{
				bSetTargetActor = true;
			}
		}
		AIStimulus = CanSense<EAISenseType::Damage>(Actor, GetPerceptionComponent());
		if (AIStimulus.IsValid())
		{
			HandleDamageStimulus(Actor, AIStimulus);
		}
	}

	if (bSetTargetActor == false && GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetActorBlackboardKey, nullptr);
	}
}

void APAAIControllerBase::OnPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{

}

void APAAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}

bool APAAIControllerBase::HandleSightStimulus(AActor* SensedActor, const FAIStimulus& Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() == false)
	{
		return false;
	}
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(SensedActor);
	if (PlayerCharacter && GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetActorBlackboardKey, PlayerCharacter);
		return true;
	}
	return false;
}

bool APAAIControllerBase::HandleHearingStimulus(AActor* SensedActor, const FAIStimulus& Stimulus)
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return false;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	
	if (GetBlackboardComponent())
	{
		if (SenseSoundBlackboardKey.IsValid())
		{
			if (TimerManager.IsTimerActive(SenseSoundTimer))
			{
				TimerManager.ClearTimer(SenseSoundTimer);
			}
				
			GetBlackboardComponent()->SetValueAsBool(SenseSoundBlackboardKey, true);
			TimerManager.SetTimer(SenseSoundTimer, this, &APAAIControllerBase::ClearSenseSound, SenseSoundActiveTime, false);
		}
	}
	return true;
}

bool APAAIControllerBase::HandleDamageStimulus(AActor* SensedActor, const FAIStimulus& Stimulus)
{
	return true;
}

void APAAIControllerBase::ClearSenseSound()
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		if (SenseSoundBlackboardKey.IsValid())
		{
			BlackboardComp->SetValueAsBool(SenseSoundBlackboardKey, false);
		}
	}
}

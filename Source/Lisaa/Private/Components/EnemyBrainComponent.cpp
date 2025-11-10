#include "Components/EnemyBrainComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UEnemyBrainComponent::UEnemyBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyBrainComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerChar = Cast<ACharacter>(GetOwner());
}

void UEnemyBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwnerChar.IsValid())
		return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
		return;

	UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement();
	if (!MoveComp)
		return;

	const FVector ToPlayer = PlayerPawn->GetActorLocation() - OwnerChar->GetActorLocation();
	const float Dist2D = FVector::Dist2D(PlayerPawn->GetActorLocation(), OwnerChar->GetActorLocation());

	float InRange = StopDistance;

	if (Dist2D <= InRange)
	{
		OwnerChar->ConsumeMovementInputVector();
		MoveComp->StopMovementImmediately();
		// lancer un attaque
		return;
	}

	const FVector Dir = ToPlayer.GetSafeNormal2D();
	OwnerChar->AddMovementInput(Dir, 1.f);

	const FRotator Target = Dir.Rotation();
	const FRotator NewRot = FMath::RInterpTo(OwnerChar->GetActorRotation(), Target, DeltaTime, 10.f);
	OwnerChar->SetActorRotation(NewRot);
	
}


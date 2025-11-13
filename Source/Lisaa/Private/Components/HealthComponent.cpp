#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void UHealthComponent::ApplyDamageAt(float Amount, AActor* ActorInstigator)
{
	if (Amount <= 0.f || IsDead())
		return;

	const float OldHealth = Health;
	Health = FMath::Clamp(Health - Amount, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(Health, Health - OldHealth);

	if (Health <= 0.f)
		OnDeath.Broadcast(GetOwner());
}


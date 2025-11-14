#include "Components/EnemyAttackComponent.h"

#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"

UEnemyAttackComponent::UEnemyAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyAttackComponent::TryAttack(AActor* Target)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Yo "));
	if (!Target) return;
	if (!OwnerChar.IsValid())
		OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar.IsValid())
		return;

	UWorld* MyWorld = GetWorld();
	if (!MyWorld)
		return;
	const float Time = MyWorld->GetTimeSeconds();

	if (bAttacking || (Time - LastAttackTime) < Cooldown)
		return;

	const float Dist2D = FVector::Dist2D(Target->GetActorLocation(), OwnerChar->GetActorLocation());
	if (Dist2D > AttackRange + HitRadius)
		return;

	// Orienter l'ia vers la target (le player)
	const FVector Dir = (Target->GetActorLocation() - OwnerChar->GetActorLocation());
	const FRotator TargetYaw = Dir.Rotation();
	OwnerChar->SetActorRotation(FMath::RInterpTo(OwnerChar->GetActorRotation(),
		TargetYaw, MyWorld->GetDeltaSeconds(), YawTurnRate));

	// check tolerance angle
	if (FacingToleranceDeg > 0.f && !IsFacingTarget(Target))
		return;

	// TODO Play anim attack

	bAttacking = true;
	CachedTarget = Target;

	MyWorld->GetTimerManager().SetTimer(HitHandle, this, &UEnemyAttackComponent::DoHit,
		FMath::Max(0.f, WindupTime), false);
	MyWorld->GetTimerManager().SetTimer(EndHandle, this, &UEnemyAttackComponent::EndAttack,
		FMath::Max(Cooldown, WindupTime), false);
}

void UEnemyAttackComponent::DoHit()
{
	if (!OwnerChar.IsValid())
		return;
	AActor* Target = CachedTarget.Get();
	if (!Target)
		return;
	const float Dist2D = FVector::Dist2D(Target->GetActorLocation(), OwnerChar->GetActorLocation());
	if (Dist2D > AttackRange + HitRadius)
		return;
	if (FacingToleranceDeg > 0.f && !IsFacingTarget(Target))
		return;

	if (UHealthComponent* HC = Target->FindComponentByClass<UHealthComponent>())
	{
		HC->ApplyDamageAt(Damage, OwnerChar.Get());
	}
}

void UEnemyAttackComponent::EndAttack()
{
	if (UWorld* MyWorld = GetWorld())
		LastAttackTime = MyWorld->GetTimeSeconds();

	bAttacking = false;
	CachedTarget = nullptr;
}

bool UEnemyAttackComponent::IsFacingTarget(AActor* Target) const
{
	if (!OwnerChar.IsValid() || !Target)
		return false;
	const FVector Forward = OwnerChar->GetActorForwardVector().GetSafeNormal2D();
	const FVector Dir = (Target->GetActorLocation() - OwnerChar->GetActorLocation());
	const float CosAngle = FVector::DotProduct(Forward, Dir);
	const float CosThresh = FMath::Cos(FMath::DegreesToRadians(FacingToleranceDeg));
	return CosAngle >= CosThresh;
}


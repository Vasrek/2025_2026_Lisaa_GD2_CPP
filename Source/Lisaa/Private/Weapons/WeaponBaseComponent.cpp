#include "Weapons/WeaponBaseComponent.h"
#include "Characters/EnemyCharacter.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

UWeaponBaseComponent::UWeaponBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponBaseComponent::TickFire(float Now)
{
	FRotator Rot;
	const FVector OriginMuzzle = GetMuzzleLocation(Rot);

	AActor* Target = nullptr;
	if (NeedsTarget())
	{
		Target = AcquireNearestTarget(OriginMuzzle);
		if (!Target)
			return;
	}

	DoFire(Target);

	NextFireTime = Now + BaseInterval;
}

void UWeaponBaseComponent::ResetCooldown()
{
	if (UWorld* World = GetWorld())
		NextFireTime = World->GetTimeSeconds();
}

void UWeaponBaseComponent::DoFire(AActor* Target)
{
	
}

AActor* UWeaponBaseComponent::AcquireNearestTarget(const FVector& Origin) const
{
	UWorld* World = GetWorld();
	if (!World)
		return nullptr;

	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllEnemies);

	AActor* NearestTarget = nullptr;
	float BestDistance = Range * Range;

	FCollisionQueryParams Params (SCENE_QUERY_STAT(WeaponLOS), false, GetOwner());

	for (AActor* BestTarget : AllEnemies)
	{
		if (!BestTarget || BestTarget == GetOwner()) continue;

		if (UHealthComponent* HealthComp = BestTarget->GetComponentByClass<UHealthComponent>())
		{
			if (HealthComp->IsDead()) continue;

			const float Dist = FVector::DistSquared2D(BestTarget->GetActorLocation(), Origin);
			if (Dist > BestDistance) continue;

			if (bRequireLineOfSight)
			{
				FHitResult LineTrace;
				const bool HitBlock = World->LineTraceSingleByChannel(LineTrace, Origin,
					BestTarget->GetActorLocation(), ECC_Visibility, Params);
				if (HitBlock && LineTrace.GetActor() != BestTarget) continue;
			}
			NearestTarget = BestTarget;
			BestDistance = Dist;
 		}
	}
	return NearestTarget;
}

FVector UWeaponBaseComponent::GetMuzzleLocation(FRotator& OutRot) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		OutRot = FRotator::ZeroRotator;
		return FVector::ZeroVector;
	}

	// Un socket existe sur le skelette du joueur
	if (const USkeletalMeshComponent* MuzzleSkeletal = Owner->FindComponentByClass<USkeletalMeshComponent>())
	{
		if (MuzzleSkeletal->DoesSocketExist(MuzzleSocketName))
		{
			OutRot = MuzzleSkeletal->GetSocketRotation(MuzzleSocketName);
			return MuzzleSkeletal->GetSocketLocation(MuzzleSocketName);
		}
	}

	OutRot = Owner->GetActorRotation();
	return Owner->GetActorLocation() + Owner->GetActorForwardVector() * 50.f;
}


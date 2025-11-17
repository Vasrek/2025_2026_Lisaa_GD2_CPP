#include "Weapons/Weapon_Projectile.h"
#include "Weapons/Projectile/Projectile.h"

void UWeapon_Projectile::DoFire(AActor* Target)
{
	if (!ProjectileClass || !Target)
		return;

	FRotator MuzzleRotation;
	const FVector MuzzleLocation = GetMuzzleLocation(MuzzleRotation);

	FVector ToTarget = Target->GetActorLocation() - MuzzleLocation;
	if (!ToTarget.Normalize())
		return;

	const FRotator BaseRot = ToTarget.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();

	UWorld* World = GetWorld();
	if (!World)
		return;

	for (int32 IndexProj = 0; IndexProj < ProjectilesPerShot; ++IndexProj)
	{
		FRotator ShotRotation = BaseRot;

		if (SpreadDeg > 0.f)
		{
			ShotRotation.Yaw += FMath::FRandRange(-SpreadDeg * 0.5f, SpreadDeg * 0.5f); 
		}

		if (AProjectile* SpawnProj = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, ShotRotation, SpawnParams))
		{
			SpawnProj->SetDamage(GetDamage());
			SpawnProj->InitShot(GetOwner(), ProjectileSpeed);
		}
	}
}

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBaseComponent.h"
#include "Weapon_Projectile.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS()
class LISAA_API UWeapon_Projectile : public UWeaponBaseComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float DamageMult = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 ProjectilesPerShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float SpreadDeg = 0.f;

	float GetDamage() const { return BaseDamage * DamageMult; }

protected:
	virtual bool NeedsTarget() const override { return true; }

	virtual void DoFire(AActor* Target) override;
};

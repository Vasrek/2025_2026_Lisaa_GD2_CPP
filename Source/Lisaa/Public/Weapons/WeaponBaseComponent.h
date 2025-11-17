#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISAA_API UWeaponBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponBaseComponent();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName WeaponId = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Weapon|FireRate")
	float BaseInterval = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Weapon|FireRate")
	float FireRateMult = 1.f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Range")
	float Range = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Range")
	bool bRequireLineOfSight = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	void TickFire(float Now);

	void ResetCooldown();

protected:

	virtual bool NeedsTarget() const { return true;}

	virtual void DoFire(AActor* Target);

	AActor* AcquireNearestTarget(const FVector& Origin) const;

	FVector GetMuzzleLocation(FRotator& OutRot) const;

private:
	float NextFireTime = 0.f;
};

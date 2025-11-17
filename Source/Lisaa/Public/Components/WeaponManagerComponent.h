#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class UWeaponBaseComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISAA_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManagerComponent();

	UPROPERTY(EditAnywhere, Category = "WeaponManager")
	TArray<TSubclassOf<UWeaponBaseComponent>> StartingWeapons;

	UPROPERTY(EditAnywhere, Category = "WeaponManager")
	float SchedulerTick = 0.05f;

	UFUNCTION(BlueprintCallable, Category = "WeaponManager")
	UWeaponBaseComponent* GetWeaponById(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "WeaponManager")
	bool HasWeaponByClass(TSubclassOf<UWeaponBaseComponent> WeaponClass) const;

	UFUNCTION(BlueprintCallable, Category = "WeaponManager")
	bool HasWeaponId(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "WeaponManager")
	UWeaponBaseComponent* AddWeaponByClass(TSubclassOf<UWeaponBaseComponent> WeaponToAdd);
	
protected:
	virtual void BeginPlay() override;

private:
		FTimerHandle SchedulerHandle;

	UPROPERTY()
	TArray<UWeaponBaseComponent*> AllPlayersWeapons;

	void SchedulerTickFunc();
};



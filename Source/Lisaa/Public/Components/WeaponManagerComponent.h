#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISAA_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManagerComponent();

	// UPROPERTY(EditAnywhere, Category = "WeaponManager")
	// TArray<TSubclassOf<>>
	
protected:
	virtual void BeginPlay() override;

private:
		
};

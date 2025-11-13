#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// OnDeath Called when we die
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

// OnHealthChanged called when we lose health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISAA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	void ApplyDamageAt(float Amount, AActor* ActorInstigator = nullptr);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return Health <= 0; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 50.f;

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	virtual void BeginPlay() override;

private:
	float Health = 0.f;
	
};

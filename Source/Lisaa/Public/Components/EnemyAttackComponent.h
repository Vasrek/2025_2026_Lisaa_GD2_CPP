#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISAA_API UEnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyAttackComponent();

	// === ATTACK PARAMS ===
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 150.f;;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Cooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float WindupTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float HitRadius = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float YawTurnRate = 720.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float FacingToleranceDeg = 75.f;

	UFUNCTION(BlueprintCallable)
	void TryAttack(AActor* Target);

private:
	TWeakObjectPtr<class ACharacter> OwnerChar;
	bool bAttacking = false;
	float LastAttackTime = -1000.f;
	TWeakObjectPtr<AActor> CachedTarget;

	FTimerHandle HitHandle;
	FTimerHandle EndHandle;

	void DoHit();
	void EndAttack();
	bool IsFacingTarget(AActor* Target) const;
};

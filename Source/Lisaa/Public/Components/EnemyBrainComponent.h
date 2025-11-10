#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBrainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISAA_API UEnemyBrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyBrainComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, Category = "AI")
	float StopDistance = 75.f;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<class ACharacter> OwnerChar;

		
};

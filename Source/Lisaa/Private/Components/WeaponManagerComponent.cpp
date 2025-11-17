#include "Components/WeaponManagerComponent.h"
#include "Weapons/WeaponBaseComponent.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UWeaponBaseComponent> BaseWeapon : StartingWeapons)
	{
		AddWeaponByClass(BaseWeapon);
	}

	if (SchedulerTick > 0.f && AllPlayersWeapons.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SchedulerHandle, this, &UWeaponManagerComponent::SchedulerTickFunc, SchedulerTick, true);
	}
}



UWeaponBaseComponent* UWeaponManagerComponent::GetWeaponById(FName Id) const
{
	for (UWeaponBaseComponent* WeaponToCheck : AllPlayersWeapons)
	{
		if (WeaponToCheck && WeaponToCheck->WeaponId == Id)
			return WeaponToCheck;
	}
	return nullptr;
}

UWeaponBaseComponent* UWeaponManagerComponent::AddWeaponByClass(TSubclassOf<UWeaponBaseComponent> WeaponToAdd) 
{
	if (!WeaponToAdd)
		return nullptr;

	AActor* Owner = GetOwner();
	if (!Owner)
		return nullptr;
	UWeaponBaseComponent* NewWeapon = NewObject<UWeaponBaseComponent>(Owner, WeaponToAdd);
	if (!NewWeapon)
		return nullptr;

	NewWeapon->RegisterComponent();
	AllPlayersWeapons.Add(NewWeapon);
	return NewWeapon;
}

bool UWeaponManagerComponent::HasWeaponByClass(TSubclassOf<UWeaponBaseComponent> WeaponClass) const
{
	if (!*WeaponClass)
		return false;
	for (const UWeaponBaseComponent* CurrentWeaponClass : AllPlayersWeapons)
		if (CurrentWeaponClass && CurrentWeaponClass->IsA(WeaponClass))
			return true;
	return false;
}

bool UWeaponManagerComponent::HasWeaponId(FName Id) const
{
	return GetWeaponById(Id) != nullptr;
}

void UWeaponManagerComponent::SchedulerTickFunc()
{
	const float Now = GetWorld()->GetTimeSeconds();
	for (UWeaponBaseComponent* CurrentWeapon : AllPlayersWeapons)
	{
		if (CurrentWeapon)
			CurrentWeapon->TickFire(Now);
	}
}


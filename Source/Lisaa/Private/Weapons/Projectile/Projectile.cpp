#include "Weapons/Projectile/Projectile.h"

#include "Components/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereCol->InitSphereRadius(8.0f);
	SphereCol->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCol->SetCollisionObjectType(ECC_WorldDynamic);
	SphereCol->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCol->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = SphereCol;
	SphereCol->SetSimulatePhysics(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->UpdatedComponent = SphereCol;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bForceSubStepping = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSeconds);

	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);

	if (AActor* MyOwner = GetOwner())
		SphereCol->IgnoreActorWhenMoving(MyOwner, true);
	
}

void AProjectile::InitShot(AActor* InOwner, float Speed)
{
	SetOwner(InOwner);
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
}

void AProjectile::OnOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!Other || Other == GetOwner())
		return;
	if (UHealthComponent* HealthComp = Other->FindComponentByClass<UHealthComponent>())
	{
		HealthComp->ApplyDamageAt(Damage, GetOwner());
	}
	Destroy();
}



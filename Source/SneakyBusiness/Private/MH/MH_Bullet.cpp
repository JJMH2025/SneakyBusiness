// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Bullet.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LHM/Enemy.h"
#include "MH/Player_Nick.h"

// Sets default values
AMH_Bullet::AMH_Bullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//콜리전
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionComp->SetSphereRadius(13);
	RootComponent = CollisionComp;

	//메쉬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetRelativeScale3D(FVector(0.5f));

	//이동
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	MovementComp->SetUpdatedComponent(RootComponent);
	//초기속도
	MovementComp->InitialSpeed = 1800;
	MovementComp->MaxSpeed = 1800;
	MovementComp->bShouldBounce = true;
	MovementComp->Bounciness = 0.5;
	MovementComp->ProjectileGravityScale = 0.2f;
	MovementComp->Friction = 0.2f;
}

// Called when the game starts or when spawned
void AMH_Bullet::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMH_Bullet::OnBulletBeginOverlap);
}

// Called every frame
void AMH_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMH_Bullet::OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	//OwnerActor에 플레이어가 있다면 
	if (OwnerActor != nullptr && OwnerActor->IsA(APlayer_Nick::StaticClass()))
	{
		//플레이어 액터 캐스트
		Player_Nick = Cast<APlayer_Nick>(OwnerActor);
	}

	if (!OtherActor || OtherActor == this || OtherActor == OwnerActor) return; // 자기 자신이나 발사자를 제외

	if (OtherActor != nullptr)
	{
		// 플레이어가 쏜 총알 → 에너미만 피격 가능
		if (OwnerActor != nullptr && OwnerActor->IsA(APlayer_Nick::StaticClass()))
		{
			AEnemy* Hitenemy = Cast<AEnemy>(OtherActor);
			//총알을 맞은게 에너미면
			if (OtherActor == Hitenemy)
			{
				GEngine->AddOnScreenDebugMessage(-4, 5.f, FColor::Green,TEXT("Enemy Overlap!!"));
				//에너미 기절
				Hitenemy->ReceiveDamage();
			}
			//플레이어 총알 재장전 ++
			if (Player_Nick)
			{
				Player_Nick->ShootComp->ReloadAmmo();
			}
		}

		//에너미가 쏜 총알 -> 플레이어 피격
		if (OwnerActor != nullptr && OwnerActor->IsA(AEnemy::StaticClass()))
		{
			APlayer_Nick* Hitplayer = Cast<APlayer_Nick>(OwnerActor);
			if (OtherActor == Hitplayer)
			{
				GEngine->AddOnScreenDebugMessage(-4, 5.f, FColor::Green,TEXT("Player Overlap!!"));
				//HP-1
				Hitplayer->PlayerTakeDamage();
			}
		}

		//어딘가에 닿은 뒤 0.5초후 사라지기
		this->Destroy();
	}
}

void AMH_Bullet::SetOwnerActor(AActor* NewOwner)
{
	OwnerActor = NewOwner;
}

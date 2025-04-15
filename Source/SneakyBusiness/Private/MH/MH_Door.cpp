// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Door.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Components/BoxComponent.h"
#include "LHM/Enemy.h"
#include "MH/Player_Nick.h"

// Sets default values
AMH_Door::AMH_Door()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComp
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	//기절할 때 체크할 콜리전
	BoxComponentL = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentL"));
	BoxComponentL->SetupAttachment(RootComponent);

	BoxComponentR = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentR"));
	BoxComponentR->SetupAttachment(RootComponent);

	//회전 애니메이션
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootScene->SetupAttachment(RootComponent);

	// Door Mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootScene);
}

// Called when the game starts or when spawned
void AMH_Door::BeginPlay()
{
	Super::BeginPlay();

	BoxComponentR->OnComponentBeginOverlap.AddDynamic(this, &AMH_Door::OnDoorBeginOverlapDoor);
	BoxComponentL->OnComponentBeginOverlap.AddDynamic(this, &AMH_Door::OnDoorBeginOverlapDoor);

	BoxComponentR->OnComponentEndOverlap.AddDynamic(this, &AMH_Door::OnDoorEndOverlapDoor);
	BoxComponentL->OnComponentEndOverlap.AddDynamic(this, &AMH_Door::OnDoorEndOverlapDoor);
}

// Called every frame
void AMH_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingDoor)
	{
		//현재 회전값
		float CurrentYaw = RootScene->GetComponentRotation().Yaw;
		// 최단 거리 회전각 계산
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
		float NewYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + DeltaYaw, GetWorld()->GetDeltaSeconds(), 15.0f);

		RootScene->SetRelativeRotation(FRotator(0.f, NewYaw, 0.f));

		if (FMath::Abs(NewYaw - TargetYaw) < 1.0f)
		{
			NewYaw = TargetYaw;
			bIsRotatingDoor = false;
			//지나갔는지 확인하고 닫히기
		}
	}
}

void AMH_Door::DoorOpen(const FVector& InstigatorForward, AActor* InteractingActor)
{
	LastActorOpened = InteractingActor;

	//문열리기
	bIsRotatingDoor = true;
	float Dot = FVector::DotProduct(InstigatorForward, GetActorForwardVector());
	//오른쪽
	if (Dot > 0.5f)
	{
		TargetYaw = -90.f;
		bDoorOpenedR = true;

		GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("RStun"));
		//R콜리전에 닿아있는 애들 기절
		for (AActor* OtherActor : OverlappingActorsR)
		{
			//문연 애가 OtherActor가 아니라면
			if (IsValid(OtherActor) && OtherActor != LastActorOpened)
			{
				//플레이어
				APlayer_Nick* PlayerNick = Cast<APlayer_Nick>(OtherActor);
				if (PlayerNick)
				{
					//기절함수 호출
					PlayerNick->Frozen();
				}
				//애너미
				AEnemy* Enemy = Cast<AEnemy>(OtherActor);
				if (Enemy)
				{
					Enemy->SetEnemyAIState(EEnemyAIState::HitByDoor);;
				}
				//초기화
				LastActorOpened = nullptr;
			}
		}
	}
	//왼쪽
	else if (Dot < -0.5f)
	{
		TargetYaw = 90.f;
		bDoorOpenedR = false;

		GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("LStun"));
		//L콜리전에 닿아있는 애들 기절
		for (AActor* OtherActor : OverlappingActorsL)
		{
			//문연 애가 OtherActor가 아니라면
			if (IsValid(OtherActor) && OtherActor != LastActorOpened)
			{
				//플레이어
				APlayer_Nick* PlayerNick = Cast<APlayer_Nick>(OtherActor);
				if (PlayerNick)
				{
					//기절함수 호출
					PlayerNick->PlayerTakeDamage();
				}
				//애너미
				AEnemy* Enemy = Cast<AEnemy>(OtherActor);
				if (Enemy)
				{
					Enemy->Stun();
				}
				//초기화
				LastActorOpened = nullptr;
			}
		}
	}
}

void AMH_Door::DoorClosed()
{
	bIsRotatingDoor = true;
	GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("DoorClosed"));
	//문닫히기
	//회전값 원래 위치로 돌아가기
	TargetYaw = 0.f;
}

void AMH_Door::OnDoorBeginOverlapDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	//오른쪽 
	if (OverlappedComponent == BoxComponentR)
	{GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("DoorR"));
		//오버랩된 액터가 문을 연 액터라면 무시
		if (OtherActor == LastActorOpened)
		{
			return;
		}
		if (OtherActor && OtherActor->IsA(AActor::StaticClass()))
		{
			//오버랩된 OtherActor 저장
			OverlappingActorsR.Add(OtherActor);
		}
	}
	//왼쪽
	if (OverlappedComponent == BoxComponentL)
	{GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("DoorL"));
		//오버랩된 액터가 문을 연 액터라면 무시
		if (OtherActor == LastActorOpened)
		{
			return;
		}
		if (OtherActor && OtherActor->IsA(AActor::StaticClass()))
		{
			//오버랩된 OtherActor 저장
			OverlappingActorsL.Add(OtherActor);
		}
	}
}

void AMH_Door::OnDoorEndOverlapDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//오른쪽 
	if (OverlappedComponent == BoxComponentR)
	{
		//오버랩된 액터가 문을 연 액터라면 무시
		if (OtherActor == LastActorOpened)
		{
			return;
		}
		//오버랩된 OtherActor 삭제
		OverlappingActorsR.Remove(OtherActor);
	}
	//왼쪽
	if (OverlappedComponent == BoxComponentL)
	{
		//오버랩된 액터가 문을 연 액터라면 무시
		if (OtherActor == LastActorOpened)
		{
			return;
		}
		//오버랩된 OtherActor 삭제
		OverlappingActorsL.Remove(OtherActor);
	}
}

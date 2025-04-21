// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Lasertrap.h"

#include "Components/BoxComponent.h"
#include "MH/MH_EnemyAlertComp.h"

// Sets default values
AMH_Lasertrap::AMH_Lasertrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyAlertComp = CreateDefaultSubobject<UMH_EnemyAlertComp>(TEXT("EnemyAlertComp"));
		
	LaserRoot  = CreateDefaultSubobject<USceneComponent>(TEXT("LaserRoot "));
	RootComponent = LaserRoot ;
	
	// 생성자에서 초기화
	LaserCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserCollisionBox"));
	LaserCollisionBox->SetupAttachment(LaserRoot );
	LaserCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 기본 OFF
	LaserCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	LaserCollisionBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Overlap);
	LaserCollisionBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMH_Lasertrap::BeginPlay()
{
	Super::BeginPlay();

	Directions = {
		FVector(0, 0, -1),   // v 아래
		FVector(1, 0, -1).GetSafeNormal(),	// 오른 대각선
		FVector(0, 0, -1),   // v 아래
		FVector(-1, 0, -1).GetSafeNormal(),  // 왼 대각선
		};
	LaserCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMH_Lasertrap::OnLaserOverlap);
	FireLaser();
	
}

// Called every frame
void AMH_Lasertrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMH_Lasertrap::FireLaser()
{
	//레이저 시작 위치
	FVector LaserStart = GetActorLocation();
	//현재 방향
	FVector Direction = Directions[CurrentDirIdx];
	//레이저 끝나는 위치 = 시작점 + 방향 * 길이
	FVector LaserEnd = LaserStart + Direction*LaserLength;

	// 콜리전 박스는 X축 기준으로 길어지게 설계한다고 가정
	FVector BoxExtent = FVector(LaserLength * 0.5f, 5.0f, 5.0f);
	FVector BoxLocalPosition = FVector(LaserLength * 0.5f, 0.0f, 0.0f); // Local 기준: Forward(X) 방향으로 반 길이만큼 이동
	
	// LaserRoot의 로테이션을 방향 벡터로 설정
	FRotator DesiredRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	LaserRoot->SetWorldRotation(DesiredRotation);

	// 콜리전 박스 위치 및 사이즈 설정
	LaserCollisionBox->SetRelativeLocation(BoxLocalPosition); // Root 기준으로 앞쪽에 위치
	LaserCollisionBox->SetBoxExtent(BoxExtent);
	LaserCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 디버그
	DrawDebugLine(GetWorld(), LaserStart, LaserEnd, FColor::Red, false, 1.0f, 0, 3.0f);

	// 순환 방향 변경
	CurrentDirIdx = (CurrentDirIdx + 1) % Directions.Num();
	GetWorldTimerManager().SetTimer(LaserTimerHandle, this, &AMH_Lasertrap::FireLaser, LaserInterval);
}

void AMH_Lasertrap::OnLaserOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-4, 3.f, FColor::Red, TEXT("Player TRIPPED the Laser!"));
		EnemyAlertComp->AlertNearbyEnemies(OtherActor->GetActorLocation(), AlertRadius);
	}
}

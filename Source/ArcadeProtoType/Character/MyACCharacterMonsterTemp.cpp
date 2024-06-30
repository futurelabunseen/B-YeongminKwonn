#include "Character/MyACCharacterMonsterTemp.h"

AMyACCharacterMonsterTemp::AMyACCharacterMonsterTemp() 
{
	GetMesh()->SetVisibility(false);

	// Static Mesh
	// MonsterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	// if (SphereMeshAsset.Object)
	// {
	// 	MonsterMesh->SetStaticMesh(SphereMeshAsset.Object);
	// }
	// MonsterMesh->SetupAttachment(RootComponent);
}

void AMyACCharacterMonsterTemp::Dead()
{
	Destroy();
}

float AMyACCharacterMonsterTemp::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}
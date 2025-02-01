
#include "CameroidHitComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include <HiddenStaticMesh.h>

UCameroidHitComponent::UCameroidHitComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCameroidHitComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCameroidHitComponent::PerformRaycast(FVector ForwardVector) const
{
    UE_LOG(LogTemp, Log, TEXT("PERFORM RAYCAST"));
    FHitResult HitResult;

    FVector Start = GetComponentLocation() + (GetForwardVector()*50);
    FVector End = Start + ForwardVector;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        CollisionParams
    );

    DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 1, 0, 1);

    if (bHit && HitResult.GetComponent())
    {
        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10, FColor::Red, true, 1);

        if (HitResult.GetComponent()->ComponentTags.Num() > 0)
        {
            if (HitResult.GetComponent()->ComponentTags.Contains(FName("Revealable")))
            {
                AHiddenStaticMesh* HiddenMeshActor = Cast<AHiddenStaticMesh>(HitResult.GetActor());
                if (HiddenMeshActor)
                {
                    HiddenMeshActor->SetIsCaptured(true);
                    UMaterialInstanceDynamic* DynamicMaterial = HitResult.GetComponent()->CreateAndSetMaterialInstanceDynamic(0);
                    if (HitResult.GetComponent()->ComponentTags.Contains(FName("0To1")))
                    {
                        DynamicMaterial->SetScalarParameterValue("Opacity", 0);
                        HiddenMeshActor->SetMeshVisible();
                    }
                    else if (HitResult.GetComponent()->ComponentTags.Contains(FName("1To0")))
                    {
                        DynamicMaterial->SetScalarParameterValue("Opacity", 1);
						HiddenMeshActor->SetMeshInvisible();
                    }
					//log le changment de visibilité
					UE_LOG(LogTemp, Log, TEXT("Mesh visibility changed"));
                }
			}else
            {
                // Imprimer tous les tags
                for (const FName& Tag : HitResult.GetComponent()->ComponentTags)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
                }
                // Imprimer le nom de l'acteur touché
                if (HitResult.GetActor())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Hit Actor is null"));
                }
                UE_LOG(LogTemp, Warning, TEXT("Cameroid Hit Actor not revealable."));
            }
		}
        else {
            UE_LOG(LogTemp, Warning, TEXT("Cameroid Hit Actor has no tags."));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("No hit detected"));
    }
}

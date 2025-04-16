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

    FVector Start = GetComponentLocation() + (GetForwardVector() * 50);
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

        UPrimitiveComponent* HitComponent = HitResult.GetComponent();
        AHiddenStaticMesh* HiddenMeshActor = Cast<AHiddenStaticMesh>(HitResult.GetActor());

        if (HitComponent->ComponentTags.Contains(FName("Revealable")) && HiddenMeshActor)
        {
            int32 MaterialCount = HitComponent->GetNumMaterials();

            if (HiddenMeshActor->TranslucentMats.Num() == MaterialCount &&
                HiddenMeshActor->NormalMats.Num() == MaterialCount)
            {
                // Appliquer les matériaux translucides d'abord
                for (int32 i = 0; i < MaterialCount; ++i)
                {
                    HitComponent->SetMaterial(i, HiddenMeshActor->TranslucentMats[i]);
                }

                // Créer les matériaux dynamiques après l'application
                TArray<UMaterialInstanceDynamic*> DynamicMaterials;
                for (int32 i = 0; i < MaterialCount; ++i)
                {
                    UMaterialInstanceDynamic* DynamicMaterial = HitComponent->CreateAndSetMaterialInstanceDynamic(i);
                    if (DynamicMaterial)
                    {
                        DynamicMaterials.Add(DynamicMaterial);
                    }
                }

                // Appliquer l'opacité selon le tag
                if (HitComponent->ComponentTags.Contains(FName("0To1")))
                {
                    for (UMaterialInstanceDynamic* DynamicMaterial : DynamicMaterials)
                    {
                        DynamicMaterial->SetScalarParameterValue("Opacity", 0);
                    }
                    HiddenMeshActor->SetMeshVisible();
                    
                }
                else if (HitComponent->ComponentTags.Contains(FName("1To0")))
                {
                    for (UMaterialInstanceDynamic* DynamicMaterial : DynamicMaterials)
                    {
                        DynamicMaterial->SetScalarParameterValue("Opacity", 1);
                    }
                    HiddenMeshActor->SetMeshInvisible();
                    
                }
                HiddenMeshActor->SetIsCaptured(true);
                UE_LOG(LogTemp, Log, TEXT("Mesh visibility changed"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Material arrays size mismatch for actor: %s"), *HiddenMeshActor->GetName());
            }
        }
        else
        {
            // Affichage debug des tags et du nom de l'acteur non revealable
            for (const FName& Tag : HitComponent->ComponentTags)
            {
                UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
            }

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
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No hit detected"));
    }
}


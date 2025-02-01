


#include "HiddenStaticMesh.h"

void AHiddenStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	mesh = GetStaticMeshComponent();
    bIsCaptured = false; // Initialiser le booléen
}

void AHiddenStaticMesh::SetIsCaptured(bool bCaptured)
{
    bIsCaptured = bCaptured;
}

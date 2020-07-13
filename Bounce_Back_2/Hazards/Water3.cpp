// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Water3.h"

AWater3::AWater3 ()
{
    // set decal
    static ConstructorHelpers::FObjectFinder <UMaterialInstance> material (TEXT ("/Game/Assets/Materials/PuddleInstances/WaterSpill3_D_Inst.WaterSpill3_D_Inst")); check (material.Succeeded ());
    decal->SetDecalMaterial (material.Object);

    // set collision box positions & size
    CollisionBox1->SetRelativeLocationAndRotation (FVector (5.0f, -60.0f, 20.0f), FRotator (0.0f, 0.0f, 0.0f));
    CollisionBox1->SetBoxExtent (FVector (10.0f, 85.0f, 210.0f), true);

    CollisionBox2->SetRelativeLocationAndRotation (FVector (5.0f, 80.0f, -90.0f), FRotator (0.0f, 0.0f, 0.0f));
    CollisionBox2->SetBoxExtent (FVector (10.0f, 50.0f, 105.0f), true);

    CollisionBox3->SetRelativeLocationAndRotation (FVector (5.0f, 40.0f, 60.0f), FRotator (0.0f, 0.0f, 0.0f));
    CollisionBox3->SetBoxExtent (FVector (10.0f, 20.0f, 30.0f), true);
}

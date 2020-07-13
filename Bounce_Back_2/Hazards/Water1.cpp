// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Water1.h"

AWater1::AWater1 ()
{
    // set decal
    static ConstructorHelpers::FObjectFinder <UMaterialInstance> material (TEXT ("/Game/Assets/Materials/PuddleInstances/WaterSpill_D_Inst.WaterSpill_D_Inst")); check (material.Succeeded ());
    decal->SetDecalMaterial (material.Object);

    // set collision box positions & size
    CollisionBox1->SetRelativeLocationAndRotation (FVector (5.0f, 190.0f, 0.0f), FRotator (0.0f, 0.0f, 20.0f));
    CollisionBox1->SetBoxExtent (FVector (10.0f, 40.0f, 110.0f), true);

    CollisionBox2->SetRelativeLocationAndRotation (FVector (5.0f, -10.0f, -130.0f), FRotator (0.0f, 0.0f, 0.0f));
    CollisionBox2->SetBoxExtent (FVector (10.0f, 250.0f, 80.0f), true);
    
    CollisionBox3->SetRelativeLocationAndRotation (FVector (5.0f, 160.0f, -190.0f), FRotator (0.0f, 0.0f, 0.0f));
    CollisionBox3->SetBoxExtent (FVector (10.0f, 60.0f, 65.0f), true);
}

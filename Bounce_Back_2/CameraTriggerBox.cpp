#include "Bounce_Back_2.h"
#include "CameraTriggerBox.h"
#include "Bounce_Back_2Ball.h"

ACameraTriggerBox::ACameraTriggerBox()
{
	bOrthoStart = true;
}

void ACameraTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	bActive = false;

	OnActorBeginOverlap.AddDynamic(this, &ACameraTriggerBox::OnOverlapBegin);
}

void ACameraTriggerBox::OnOverlapBegin(class AActor* actor, class AActor* other)
{
	if (other && (other != this) && other == SpecificActor)
	{
		//printf("%s Entered Box", *other->GetName());
		{
			ABounce_Back_2Ball* ball = Cast<ABounce_Back_2Ball>(other);
			if (ball && bActive == false)
			{
				if (bOrthoStart)
				{
					if (!ball->GetIs2D())
					{
						bActive = true;
						ball->SwitchCameraTo2D(Camera);
					}
				}
				else
				{
					if (ball->GetIs2D())
					{
						bActive = false;
						ball->SwitchCameraTo3D();
					}
				}
			}
		}
	}
}
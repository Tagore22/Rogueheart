#include "Character/Enemy/MeleeEnemy.h"

AMeleeEnemy::AMeleeEnemy()
{
    HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    HPBarWidget->SetVisibility(false);
    HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 45.f)); // 후에 에디터에서 수정 이후 확정지을 것.
}
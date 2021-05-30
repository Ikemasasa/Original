#include "MenuStatus.h"

#include "PlayerManager.h"

MenuBase::Select MenuStatus::Update(PlayerManager* plm)
{
    return MENU_SELECT;
}

void MenuStatus::Render()
{
}

#pragma once
#include "MenuBase.h"

class MenuStatus : public MenuBase
{
public:
	MenuStatus() = default;
	MenuStatus(const MenuStatus& obj) = default;
	~MenuStatus() = default;

	Select Update(PlayerManager* plm) override;
	void Render() override;
};
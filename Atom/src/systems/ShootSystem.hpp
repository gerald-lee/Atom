/*
* Copyright (C) 2021 DigiPen Institute of Technology.
* Reproduction or disclosure of this file or its contents without the
* prior written consent of DigiPen Institute of Technology is prohibited.
* 
* @file		ShootSystem.hpp
* @author	Gerald Lee
* @brief	Contains logic for the shoot action
* @date		2021-03-06
*/

#pragma once

#include "Pch.hpp"
#include "core/System.hpp"
#include "core/Event.hpp"
#include "core/Types.hpp"

//shoot; involve instantiation of new game object

class ShootSystem : public System {
public:
	void init() override;
	void update() override;
	void onEvent(Event& e) override;
};

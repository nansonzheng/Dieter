//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2020  Daniel Allendorf, Ryan Payton, Nanson Zheng		//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../UIDragElement.h"

#include "../Components/Slider.h"
#include "../Components/Textfield.h"

#include "../../Gameplay/Stage.h"


namespace ms
{
	class UIPartyInvite : public UIDragElement<PosPARTYINVITE>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::PARTYINVITE;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = true;

		UIPartyInvite();

		void draw(float alpha) const override;
		void update() override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;
		Cursor::State send_cursor(bool pressed, Point<int16_t> cursorpos) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:

		void invite(std::string name);

		enum Buttons
		{
			BT_CLOSE,
			BT_INVITE_NAME,
			BT_SORT_NAME,
			BT_SORT_JOB,
			BT_SORT_LEVEL,
			BT_INVITE_0,
			BT_INVITE_1,
			BT_INVITE_2,
			BT_INVITE_3,
			BT_INVITE_4,
			BT_INVITE_5,
			BT_INVITE_6,
			BT_INVITE_7,
			BT_INVITE_8,
			BT_INVITE_9,
			BT_INVITE_10,
		};

		// TODO: list of invitable chars (how to know if a char is in party?)
		// Use mapchars ref?

		Slider slider;

		Textfield namefield;
	};
}
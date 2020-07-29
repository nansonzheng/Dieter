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
#include "UIPartyInvite.h"

#include "../../IO/Components/MapleButton.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIPartyInvite::UIPartyInvite() : UIDragElement<PosPARTYINVITE>(Point<int16_t>(264, 20))
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose3"];
		nl::node main = nl::nx::ui["UIWindow2.img"]["UserList"]["PopupInvite"];

		sprites.emplace_back(main["backgrnd"]);

		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(244, 7));

		sprites.emplace_back(main["backgrnd2"]);
		sprites.emplace_back(main["title"]);
		sprites.emplace_back(main["backgrnd3"]);

		namefield = Textfield(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::BLACK, Rectangle<int16_t>(0, 176, 0, 14), 12);

		buttons[Buttons::BT_SORT_NAME] = std::make_unique<MapleButton>(main["BtName"]);
		buttons[Buttons::BT_SORT_JOB] = std::make_unique<MapleButton>(main["BtJob"]);
		buttons[Buttons::BT_SORT_LEVEL] = std::make_unique<MapleButton>(main["BtLevel"]);

		sprites.emplace_back(main["invite"]);
		sprites.emplace_back(main["table"]);

		buttons[Buttons::BT_INVITE_NAME] = std::make_unique<MapleButton>(main["BtInviteName"]);
		buttons[Buttons::BT_INVITE_NAME]->set_state(Button::State::DISABLED);

		for (int i = 0; i < 11; i++)
		{
			buttons[BT_INVITE_0 + i] = std::make_unique<MapleButton>(main["BtInvite"], Point<int16_t>(203, 153 + i * 20));
			buttons[BT_INVITE_0 + i]->set_state(Button::State::DISABLED);
		}

		slider = Slider(Slider::Type::DEFAULT_SILVER, Range<int16_t>(132, 359), 243, 11, 11, [](bool) {});

		dimension = Point<int16_t>(264, 382);

		
	}

	void UIPartyInvite::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);
		UIElement::draw_buttons(alpha);
		slider.draw(position);
		namefield.draw(position + Point<int16_t>(18, 86));
	}

	void UIPartyInvite::update()
	{
		// TODO: update list of available players
		UIElement::update();
		if (namefield.empty() != (buttons[Buttons::BT_INVITE_NAME]->get_state() == Button::State::DISABLED))
		{
			buttons[Buttons::BT_INVITE_NAME]->set_state(namefield.empty() ? Button::State::DISABLED : Button::State::NORMAL);
		}
	}

	void UIPartyInvite::send_key(int32_t keycode, bool pressed, bool escape)
	{
		// TODO: enter on textfield = send invite, esc = close?
		if (pressed)
		{
			if (escape)
				deactivate();
		}
	}

	Cursor::State UIPartyInvite::send_cursor(bool pressed, Point<int16_t> cursorpos)
	{
		if (namefield.get_state() == Textfield::State::NORMAL)
		{
			Cursor::State nstate = namefield.send_cursor(cursorpos, pressed);

			if (nstate != Cursor::State::IDLE)
				return nstate;
		}

		return UIElement::send_cursor(pressed, cursorpos);
	}

	UIElement::Type UIPartyInvite::get_type() const
	{
		return TYPE;
	}

	Button::State UIPartyInvite::button_pressed(uint16_t buttonid)
	{
		std::cout << "UIPartyInvite: " << buttonid << " pressed" << std::endl;
		switch (buttonid)
		{
		case Buttons::BT_CLOSE:
			deactivate();
			break;
		case Buttons::BT_SORT_NAME:
		case Buttons::BT_SORT_JOB:
		case Buttons::BT_SORT_LEVEL:
		case Buttons::BT_INVITE_NAME:
		case Buttons::BT_INVITE_0:
		case Buttons::BT_INVITE_1:
		case Buttons::BT_INVITE_2:
		case Buttons::BT_INVITE_3:
		case Buttons::BT_INVITE_4:
		case Buttons::BT_INVITE_5:
		case Buttons::BT_INVITE_6:
		case Buttons::BT_INVITE_7:
		case Buttons::BT_INVITE_8:
		case Buttons::BT_INVITE_9:
		case Buttons::BT_INVITE_10:
		default:
			break;
		}
		return Button::State::NORMAL;
	}

	void UIPartyInvite::invite(std::string name)
	{
		std::cout << "UIPartyInvite: Inviting " << name << std::endl;
	}


}
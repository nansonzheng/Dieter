//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
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
#include "UIUserList.h"

#include "UINotice.h"

#include "UIPartyInvite.h"

#include "../UI.h"

#include "../../Character/Player.h"
#include "../../Character/Job.h"

#include "../../IO/Components/MapleButton.h"

#include "../../Gameplay/Stage.h"

#include "../../Net/Packets/PartyPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

#define USE_NOTICE_INVITE

namespace ms
{
	UIUserList::UIUserList(uint16_t t) : UIDragElement<PosUSERLIST>(Point<int16_t>(260, 20)), tab(t)
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose3"];
		UserList = nl::nx::ui["UIWindow2.img"]["UserList"];
		nl::node Main = UserList["Main"];

		sprites.emplace_back(Main["backgrnd"]);

		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(244, 7));

		nl::node taben = Main["Tab"]["enabled"];
		nl::node tabdis = Main["Tab"]["disabled"];

		buttons[Buttons::BT_TAB_FRIEND] = std::make_unique<TwoSpriteButton>(tabdis["0"], taben["0"]);
		buttons[Buttons::BT_TAB_PARTY] = std::make_unique<TwoSpriteButton>(tabdis["1"], taben["1"]);
		buttons[Buttons::BT_TAB_BOSS] = std::make_unique<TwoSpriteButton>(tabdis["2"], taben["2"]);
		buttons[Buttons::BT_TAB_BLACKLIST] = std::make_unique<TwoSpriteButton>(tabdis["3"], taben["3"]);

		// Party Tab
		nl::node party_node = Main["Party"];
		nl::node PartySearch = party_node["PartySearch"];

		party_tab = Tab::PARTY_MINE;
		party_title = party_node["title"];

		for (size_t i = 0; i <= 4; i++)
			party_mine_grid[i] = UserList["Sheet2"][i];

		party_mine_name = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::BLACK, "none", 0);

		nl::node party_taben = party_node["Tab"]["enabled"];
		nl::node party_tabdis = party_node["Tab"]["disabled"];

		buttons[Buttons::BT_PARTY_CREATE] = std::make_unique<MapleButton>(party_node["BtPartyMake"]);
		buttons[Buttons::BT_PARTY_INVITE] = std::make_unique<MapleButton>(party_node["BtPartyInvite"]);
		buttons[Buttons::BT_PARTY_LEAVE] = std::make_unique<MapleButton>(party_node["BtPartyOut"]);
		buttons[Buttons::BT_PARTY_SETTINGS] = std::make_unique<MapleButton>(party_node["BtPartySettings"]);
		buttons[Buttons::BT_PARTY_CREATE]->set_active(false);
		buttons[Buttons::BT_PARTY_INVITE]->set_active(false);
		buttons[Buttons::BT_PARTY_LEAVE]->set_active(false);
		buttons[Buttons::BT_PARTY_SETTINGS]->set_active(false);

		buttons[Buttons::BT_TAB_PARTY_MINE] = std::make_unique<TwoSpriteButton>(party_tabdis["0"], party_taben["0"]);
		buttons[Buttons::BT_TAB_PARTY_SEARCH] = std::make_unique<TwoSpriteButton>(party_tabdis["1"], party_taben["1"]);
		buttons[Buttons::BT_TAB_PARTY_MINE]->set_active(false);
		buttons[Buttons::BT_TAB_PARTY_SEARCH]->set_active(false);

		party_search_grid[0] = PartySearch["partyName"];
		party_search_grid[1] = PartySearch["request"];
		party_search_grid[2] = PartySearch["table"];

		buttons[Buttons::BT_PARTY_SEARCH_LEVEL] = std::make_unique<MapleButton>(PartySearch["BtPartyLevel"]);
		buttons[Buttons::BT_PARTY_SEARCH_LEVEL]->set_active(false);

		int16_t party_x = 243;
		int16_t party_y = 114;
		int16_t party_height = party_y + 168;
		int16_t party_unitrows = 6;
		int16_t party_rowmax = 6;
		party_slider = Slider(Slider::Type::DEFAULT_SILVER, Range<int16_t>(party_y, party_height), party_x, party_unitrows, party_rowmax, [](bool) {});

		party = Optional<Party>();
		int32_t pid = Stage::get().get_player().get_party_id();
		if (pid != 0)
		{
			party = Stage::get().get_parties()[pid];
			update_party(party);
		}

		// Buddy Tab
		nl::node Friend = Main["Friend"];

		friend_tab = Tab::FRIEND_ALL;
		friend_sprites.emplace_back(Friend["title"]);
		friend_sprites.emplace_back(Friend["CbCondition"]["text"]);
		friend_sprites.emplace_back(UserList["line"], DrawArgument(Point<int16_t>(132, 115), Point<int16_t>(230, 0)));

		buttons[Buttons::BT_FRIEND_GROUP_0] = std::make_unique<MapleButton>(UserList["BtSheetIClose"], Point<int16_t>(13, 118));
		buttons[Buttons::BT_FRIEND_GROUP_0]->set_active(false);

		for (size_t i = 0; i <= 3; i++)
			friend_grid[i] = UserList["Sheet1"][i];

		std::string text = "(" + std::to_string(friend_count) + std::string("/") + std::to_string(friend_total) + std::string(")");
		friends_online_text = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::WHITE, text, 0);

		friends_cur_location = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::LIGHTGREY, "My Location - " + get_cur_location(), 0);
		friends_name = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::BLACK, "none", 0);
		friends_group_name = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::WHITE, "Default Group (0/0)", 0);

		buttons[Buttons::BT_FRIEND_ADD] = std::make_unique<MapleButton>(Friend["BtAddFriend"]);
		buttons[Buttons::BT_FRIEND_ADD_GROUP] = std::make_unique<MapleButton>(Friend["BtAddGroup"]);
		buttons[Buttons::BT_FRIEND_EXPAND] = std::make_unique<MapleButton>(Friend["BtPlusFriend"]);
		buttons[Buttons::BT_FRIEND_ADD]->set_active(false);
		buttons[Buttons::BT_FRIEND_ADD_GROUP]->set_active(false);
		buttons[Buttons::BT_FRIEND_EXPAND]->set_active(false);

		buttons[Buttons::BT_TAB_FRIEND_ALL] = std::make_unique<MapleButton>(Friend["TapShowAll"]);
		buttons[Buttons::BT_TAB_FRIEND_ONLINE] = std::make_unique<MapleButton>(Friend["TapShowOnline"]);
		buttons[Buttons::BT_TAB_FRIEND_ALL]->set_active(false);
		buttons[Buttons::BT_TAB_FRIEND_ONLINE]->set_active(false);

		int16_t friends_x = 243;
		int16_t friends_y = 115;
		int16_t friends_height = friends_y + 148;
		int16_t friends_unitrows = 6;
		int16_t friends_rowmax = 6;
		friends_slider = Slider(Slider::Type::DEFAULT_SILVER, Range<int16_t>(friends_y, friends_height), friends_x, friends_unitrows, friends_rowmax, [](bool) {});

		// Boss tab
		nl::node Boss = Main["Boss"];

		boss_sprites.emplace_back(Boss["base"]);
		boss_sprites.emplace_back(Boss["base3"]);
		boss_sprites.emplace_back(Boss["base2"]);

		buttons[Buttons::BT_BOSS_0] = std::make_unique<TwoSpriteButton>(Boss["BossList"]["0"]["icon"]["disabled"]["0"], Boss["BossList"]["0"]["icon"]["normal"]["0"]);
		buttons[Buttons::BT_BOSS_1] = std::make_unique<TwoSpriteButton>(Boss["BossList"]["1"]["icon"]["disabled"]["0"], Boss["BossList"]["1"]["icon"]["normal"]["0"]);
		buttons[Buttons::BT_BOSS_2] = std::make_unique<TwoSpriteButton>(Boss["BossList"]["2"]["icon"]["disabled"]["0"], Boss["BossList"]["2"]["icon"]["normal"]["0"]);
		buttons[Buttons::BT_BOSS_3] = std::make_unique<TwoSpriteButton>(Boss["BossList"]["3"]["icon"]["disabled"]["0"], Boss["BossList"]["3"]["icon"]["normal"]["0"]);
		buttons[Buttons::BT_BOSS_4] = std::make_unique<TwoSpriteButton>(Boss["BossList"]["4"]["icon"]["disabled"]["0"], Boss["BossList"]["4"]["icon"]["normal"]["0"]);
		buttons[Buttons::BT_BOSS_L] = std::make_unique<MapleButton>(Boss["BtArrow"]["Left"]);
		buttons[Buttons::BT_BOSS_R] = std::make_unique<MapleButton>(Boss["BtArrow"]["Right"]);
		buttons[Buttons::BT_BOSS_DIFF_L] = std::make_unique<MapleButton>(Boss["BtArrow2"]["Left"]);
		buttons[Buttons::BT_BOSS_DIFF_R] = std::make_unique<MapleButton>(Boss["BtArrow2"]["Right"]);
		buttons[Buttons::BT_BOSS_GO] = std::make_unique<MapleButton>(Boss["BtEntry"]);
		buttons[Buttons::BT_BOSS_0]->set_active(false);
		buttons[Buttons::BT_BOSS_1]->set_active(false);
		buttons[Buttons::BT_BOSS_2]->set_active(false);
		buttons[Buttons::BT_BOSS_3]->set_active(false);
		buttons[Buttons::BT_BOSS_4]->set_active(false);
		buttons[Buttons::BT_BOSS_L]->set_active(false);
		buttons[Buttons::BT_BOSS_R]->set_active(false);
		buttons[Buttons::BT_BOSS_DIFF_L]->set_active(false);
		buttons[Buttons::BT_BOSS_DIFF_R]->set_active(false);
		buttons[Buttons::BT_BOSS_GO]->set_active(false);

		// Blacklist tab
		nl::node BlackList = Main["BlackList"];

		blacklist_title = BlackList["base"];

		for (size_t i = 0; i < 3; i++)
			blacklist_grid[i] = UserList["Sheet6"][i];

		blacklist_name = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::BLACK, "none", 0);

		nl::node blacklist_taben = BlackList["Tab"]["enabled"];
		nl::node blacklist_tabdis = BlackList["Tab"]["disabled"];

		buttons[Buttons::BT_BLACKLIST_ADD] = std::make_unique<MapleButton>(BlackList["BtAdd"]);
		buttons[Buttons::BT_BLACKLIST_DELETE] = std::make_unique<MapleButton>(BlackList["BtDelete"]);
		buttons[Buttons::BT_TAB_BLACKLIST_INDIVIDUAL] = std::make_unique<MapleButton>(BlackList["TapShowIndividual"]);
		buttons[Buttons::BT_TAB_BLACKLIST_GUILD] = std::make_unique<MapleButton>(BlackList["TapShowGuild"]);
		buttons[Buttons::BT_BLACKLIST_ADD]->set_active(false);
		buttons[Buttons::BT_BLACKLIST_DELETE]->set_active(false);
		buttons[Buttons::BT_TAB_BLACKLIST_INDIVIDUAL]->set_active(false);
		buttons[Buttons::BT_TAB_BLACKLIST_GUILD]->set_active(false);

		change_tab(tab);

		dimension = Point<int16_t>(276, 390);
	}

	void UIUserList::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		background.draw(position);

		if (tab == Buttons::BT_TAB_PARTY)
		{
			party_title.draw(position);

			if (party_tab == Buttons::BT_TAB_PARTY_MINE)
			{
				party_mine_grid[0].draw(position + Point<int16_t>(10, 115));
				Point<int16_t> frame_offset = position + Point<int16_t>(10, 133);
				Point<int16_t> entry_offset = position + Point<int16_t>(31, 130);
				if (party)
				{
					for (int i = 0; i < party_member_info.size(); i++)
					{
						if (party_member_info.size() - i == 1)
							party_mine_grid[2].draw(frame_offset);
						else
							party_mine_grid[1].draw(frame_offset);
						party_member_info[i][0].draw(entry_offset);
						entry_offset.shift_x(100);
						party_member_info[i][1].draw(entry_offset);
						entry_offset.shift_x(110);
						party_member_info[i][2].draw(entry_offset);
						entry_offset += Point<int16_t>(-210, 20);
						frame_offset.shift_y(20);
					}
				}
				else
				{
					party_mine_grid[2].draw(frame_offset);
					party_mine_name.draw(entry_offset);
				}
			}
			else if (party_tab == Buttons::BT_TAB_PARTY_SEARCH)
			{
				party_search_grid[0].draw(position);
				party_search_grid[1].draw(position);
				party_search_grid[2].draw(position);
				party_slider.draw(position);
			}
		}
		else if (tab == Buttons::BT_TAB_FRIEND)
		{
			for each (auto sprite in friend_sprites)
				sprite.draw(position, alpha);

			friends_online_text.draw(position + Point<int16_t>(211, 62));
			friends_cur_location.draw(position + Point<int16_t>(9, 279));
			friend_grid[0].draw(position + Point<int16_t>(10, 116));
			friend_grid[2].draw(position + Point<int16_t>(10, 135));
			friends_name.draw(position + Point<int16_t>(24, 134));
			friends_group_name.draw(position + Point<int16_t>(29, 114));
			friends_slider.draw(position);
		}
		else if (tab == Buttons::BT_TAB_BOSS)
		{
			for each (auto sprite in boss_sprites)
				sprite.draw(position, alpha);
		}
		else if (tab == Buttons::BT_TAB_BLACKLIST)
		{
			blacklist_title.draw(position + Point<int16_t>(24, 104));
			blacklist_grid[0].draw(position + Point<int16_t>(24, 134));
			blacklist_name.draw(position + Point<int16_t>(24, 134));
		}

		UIElement::draw_buttons(alpha);
	}

	void UIUserList::update()
	{
		UIElement::update();

		if (tab == Buttons::BT_TAB_FRIEND)
			for each (auto sprite in friend_sprites)
				sprite.update();

		if (tab == Buttons::BT_TAB_BOSS)
			for each (auto sprite in boss_sprites)
				sprite.update();
	}

	void UIUserList::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				deactivate();
			}
			else if (keycode == KeyAction::Id::TAB)
			{
				uint16_t new_tab = tab;

				if (new_tab < Buttons::BT_TAB_BLACKLIST)
					new_tab++;
				else
					new_tab = Buttons::BT_TAB_FRIEND;

				change_tab(new_tab);
			}
		}
	}

	UIElement::Type UIUserList::get_type() const
	{
		return TYPE;
	}

	Button::State UIUserList::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::BT_CLOSE:
				deactivate();
				break;
			case Buttons::BT_TAB_FRIEND:
			case Buttons::BT_TAB_PARTY:
			case Buttons::BT_TAB_BOSS:
			case Buttons::BT_TAB_BLACKLIST:
				change_tab(buttonid);
				return Button::State::PRESSED;
			case Buttons::BT_TAB_PARTY_MINE:
			case Buttons::BT_TAB_PARTY_SEARCH:
				change_party_tab(buttonid);
				return Button::State::PRESSED;
			case Buttons::BT_PARTY_CREATE:
				create_party();
				break;
			case Buttons::BT_PARTY_LEAVE:
				leave_party();
				break;
			case Buttons::BT_PARTY_INVITE:
				invite_party();
				break;
			case Buttons::BT_TAB_FRIEND_ALL:
			case Buttons::BT_TAB_FRIEND_ONLINE:
				change_friend_tab(buttonid);
				return Button::State::PRESSED;
			case Buttons::BT_TAB_BLACKLIST_INDIVIDUAL:
			case Buttons::BT_TAB_BLACKLIST_GUILD:
				change_blacklist_tab(buttonid);
				return Button::State::PRESSED;
			default:
				break;
		}

		return Button::State::NORMAL;
	}

	void UIUserList::change_tab(uint8_t tabid)
	{
		uint8_t oldtab = tab;
		tab = tabid;

		background = tabid == Buttons::BT_TAB_BOSS ? UserList["Main"]["Boss"]["backgrnd3"] : UserList["Main"]["backgrnd2"];

		if (oldtab != tab)
			buttons[Buttons::BT_TAB_FRIEND + oldtab]->set_state(Button::State::NORMAL);

		buttons[Buttons::BT_TAB_FRIEND + tab]->set_state(Button::State::PRESSED);

		if (tab == Buttons::BT_TAB_PARTY)
		{
			buttons[Buttons::BT_PARTY_CREATE]->set_active(true);
			buttons[Buttons::BT_PARTY_INVITE]->set_active(true);
			buttons[Buttons::BT_TAB_PARTY_MINE]->set_active(true);
			buttons[Buttons::BT_TAB_PARTY_SEARCH]->set_active(true);

			change_party_tab(Tab::PARTY_MINE);
		}
		else
		{
			buttons[Buttons::BT_PARTY_CREATE]->set_active(false);
			buttons[Buttons::BT_PARTY_INVITE]->set_active(false);
			buttons[Buttons::BT_TAB_PARTY_MINE]->set_active(false);
			buttons[Buttons::BT_TAB_PARTY_SEARCH]->set_active(false);
			buttons[Buttons::BT_PARTY_SEARCH_LEVEL]->set_active(false);
		}

		if (tab == Buttons::BT_TAB_FRIEND)
		{
			buttons[Buttons::BT_FRIEND_ADD]->set_active(true);
			buttons[Buttons::BT_FRIEND_ADD_GROUP]->set_active(true);
			buttons[Buttons::BT_FRIEND_EXPAND]->set_active(true);
			buttons[Buttons::BT_TAB_FRIEND_ALL]->set_active(true);
			buttons[Buttons::BT_TAB_FRIEND_ONLINE]->set_active(true);
			buttons[Buttons::BT_FRIEND_GROUP_0]->set_active(true);

			change_friend_tab(Tab::FRIEND_ALL);
		}
		else
		{
			buttons[Buttons::BT_FRIEND_ADD]->set_active(false);
			buttons[Buttons::BT_FRIEND_ADD_GROUP]->set_active(false);
			buttons[Buttons::BT_FRIEND_EXPAND]->set_active(false);
			buttons[Buttons::BT_TAB_FRIEND_ALL]->set_active(false);
			buttons[Buttons::BT_TAB_FRIEND_ONLINE]->set_active(false);
			buttons[Buttons::BT_FRIEND_GROUP_0]->set_active(false);
		}

		if (tab == Buttons::BT_TAB_BOSS)
		{
			buttons[Buttons::BT_BOSS_0]->set_active(true);
			buttons[Buttons::BT_BOSS_1]->set_active(true);
			buttons[Buttons::BT_BOSS_2]->set_active(true);
			buttons[Buttons::BT_BOSS_3]->set_active(true);
			buttons[Buttons::BT_BOSS_4]->set_active(true);
			buttons[Buttons::BT_BOSS_L]->set_active(true);
			buttons[Buttons::BT_BOSS_R]->set_active(true);
			buttons[Buttons::BT_BOSS_DIFF_L]->set_active(true);
			buttons[Buttons::BT_BOSS_DIFF_R]->set_active(true);
			buttons[Buttons::BT_BOSS_GO]->set_active(true);
			buttons[Buttons::BT_BOSS_L]->set_state(Button::State::DISABLED);
			buttons[Buttons::BT_BOSS_R]->set_state(Button::State::DISABLED);
			buttons[Buttons::BT_BOSS_GO]->set_state(Button::State::DISABLED);
			buttons[Buttons::BT_BOSS_DIFF_L]->set_state(Button::State::DISABLED);
			buttons[Buttons::BT_BOSS_DIFF_R]->set_state(Button::State::DISABLED);
		}
		else
		{
			buttons[Buttons::BT_BOSS_0]->set_active(false);
			buttons[Buttons::BT_BOSS_1]->set_active(false);
			buttons[Buttons::BT_BOSS_2]->set_active(false);
			buttons[Buttons::BT_BOSS_3]->set_active(false);
			buttons[Buttons::BT_BOSS_4]->set_active(false);
			buttons[Buttons::BT_BOSS_L]->set_active(false);
			buttons[Buttons::BT_BOSS_R]->set_active(false);
			buttons[Buttons::BT_BOSS_DIFF_L]->set_active(false);
			buttons[Buttons::BT_BOSS_DIFF_R]->set_active(false);
			buttons[Buttons::BT_BOSS_GO]->set_active(false);
		}

		if (tab == Buttons::BT_TAB_BLACKLIST)
		{
			buttons[Buttons::BT_BLACKLIST_ADD]->set_active(true);
			buttons[Buttons::BT_BLACKLIST_DELETE]->set_active(true);
			buttons[Buttons::BT_TAB_BLACKLIST_INDIVIDUAL]->set_active(true);
			buttons[Buttons::BT_TAB_BLACKLIST_GUILD]->set_active(true);
		}
		else
		{
			buttons[Buttons::BT_BLACKLIST_ADD]->set_active(false);
			buttons[Buttons::BT_BLACKLIST_DELETE]->set_active(false);
			buttons[Buttons::BT_TAB_BLACKLIST_INDIVIDUAL]->set_active(false);
			buttons[Buttons::BT_TAB_BLACKLIST_GUILD]->set_active(false);
		}
	}

	uint16_t UIUserList::get_tab()
	{
		return tab;
	}

	void UIUserList::update_party(Optional<Party> updated_party)
	{
		party = updated_party;

		if (!updated_party)
		{
			buttons[Buttons::BT_PARTY_CREATE]->set_active(true);
			buttons[Buttons::BT_PARTY_LEAVE]->set_active(false);
			// party_member_info.clear();
		}
		else 
		{
			buttons[Buttons::BT_PARTY_CREATE]->set_active(false);
			buttons[Buttons::BT_PARTY_LEAVE]->set_active(true);
			// TODO update member info (can be round 2)
			PartyMember* members = updated_party->get_party_members();
			party_member_info.clear();
			for (int i = 0; i < Party::MAX_PARTY_SIZE; i++)
			{
				auto pm = members[i];
				if (pm.cid > 0)
				{
					std::vector<Text> mem_info = std::vector<Text>();
					mem_info.emplace_back(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::BLACK, pm.name);
					mem_info.emplace_back(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::BLACK, Job::get_name(pm.job));
					char level_buf[4];
					snprintf(level_buf, 3, "%d", members[i].level);
					mem_info.emplace_back(Text::Font::A12M, Text::Alignment::RIGHT, Color::Name::BLACK, level_buf, 24);
					party_member_info.emplace_back(mem_info);
				}
			}
		}
	}

	void UIUserList::change_party_tab(uint8_t tabid)
	{
		uint8_t oldtab = party_tab;
		party_tab = tabid;

		if (oldtab != party_tab)
			buttons[Buttons::BT_TAB_FRIEND + oldtab]->set_state(Button::State::NORMAL);

		buttons[Buttons::BT_TAB_FRIEND + party_tab]->set_state(Button::State::PRESSED);

		if (party_tab == Buttons::BT_TAB_PARTY_SEARCH)
			buttons[Buttons::BT_PARTY_SEARCH_LEVEL]->set_active(true);
		else
			buttons[Buttons::BT_PARTY_SEARCH_LEVEL]->set_active(false);
	}

	void UIUserList::change_friend_tab(uint8_t tabid)
	{
		uint8_t oldtab = friend_tab;
		friend_tab = tabid;

		if (oldtab != friend_tab)
			buttons[Buttons::BT_TAB_FRIEND + oldtab]->set_state(Button::State::NORMAL);

		buttons[Buttons::BT_TAB_FRIEND + friend_tab]->set_state(Button::State::PRESSED);
	}

	void UIUserList::change_blacklist_tab(uint8_t tabid)
	{
		uint8_t oldtab = blacklist_tab;
		blacklist_tab = tabid;

		if (oldtab != blacklist_tab)
			buttons[Buttons::BT_TAB_FRIEND + oldtab]->set_state(Button::State::NORMAL);

		buttons[Buttons::BT_TAB_FRIEND + blacklist_tab]->set_state(Button::State::PRESSED);
	}

	std::string UIUserList::get_cur_location()
	{
		return "Henesys Market";
	}

	void UIUserList::create_party()
	{
		PartyCreatePacket().dispatch();
		return;
	}

	void UIUserList::leave_party()
	{
		PartyLeavePacket().dispatch();
		return;
	}

	void UIUserList::invite_party()
	{
		// TODO pop window out
		// Actual functions will be done once input has been received
		std::cout << "Invite party button pressed" << std::endl;
#ifdef USE_NOTICE_INVITE
		auto handle_name = [&](std::string name)
		{
			std::cout << "Inviting " << name << " to party" << std::endl;
			PartyInvitePacket(name).dispatch();
		};

		UI::get().emplace<UIEnterString>("Enter the name of the character you wish to invite.", handle_name, 12);
#else		

		UI::get().emplace<UIPartyInvite>();
#endif
	}

}
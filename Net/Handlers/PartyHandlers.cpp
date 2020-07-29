//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton, Nanson Zheng		//
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

#include "PartyHandlers.h"

#include "../../Character/Player.h"

#include "../../Gameplay/Stage.h"

#include "../../IO/UI.h"

#include "../../IO/UITypes/UIUserList.h"

namespace ms
{
	void PartyOperationHandler::handle(InPacket& recv) const
	{
		uint8_t operation = recv.read_byte();
		switch (operation)
		{
		case 4:		// 0x04: Party invite (received), Party invite from party search
			std::cout << "Party invite received to #" << recv.read_int();
			std::cout << " by " << recv.read_string() << std::endl;
			recv.read_byte();	// 0x00
			break;
		case 7:		// 0x07: (Silent) Update, member channel/location change, logged on/off
			std::cout << "Silent update" << std::endl;
			update_party(recv);
			break;
		case 8:		// 0x08: Party Created
			std::cout << "Party created" << std::endl;
			create_party(recv);
			break;
		case 10:	// message code?
			std::cout << "Can't create party when below level 10" << std::endl;
			break;
		case 12:	// 0x0C: Disband, Expelled, Left (Anyone in the party)
			std::cout << "Disband/Expel/Leave" << std::endl;
			leave_party(recv);
			break;
		case 13:	// error message code?
			std::cout << "You have yet to join a party" << std::endl;
			break;
		case 15:	// 0x0F: Join a party
			std::cout << "Join" << std::endl;
			join_party(recv);
			break;
		case 16:	// Already joined a party
			std::cout << "You're already in a party" << std::endl;
			break;
		case 17:	// Party to join is full
			std::cout << "Party is full" << std::endl;
			break;
		case 19:	// Can't find character requested in channel
			std::cout << "Requested character not in channel" << std::endl;
			break;
		case 21:	// Party invite blocked
			std::cout << "Requested character has blocked invites" << std::endl;
			break;
		case 22:	// Player is busy with another invite
			std::cout << "That player is currently handling another invitation" << std::endl;
			break;
		case 23:	// Invite rejected (which side?)
			std::cout << "Invite was rejected" << std::endl;
			break;
		case 25:	// Can't kick in this map
			std::cout << "Party Op Error message code " << operation << std::endl;
			break;
		case 27:	// 0x1B: Leader change
			std::cout << "Change leader" << std::endl;
			break;
		case 28:
		case 29:
			std::cout << "Leadership can only be passed to member within vicinity" << std::endl;
			break;
		case 35:	//0x23: Mystic door
			std::cout << "Mystic door" << std::endl;
			break;
		default:
			std::cout << "Unimplemented party operation handler code " << operation << std::endl;
			break;
		}
	}

	void PartyMemberHPHandler::handle(InPacket& recv) const
	{
		int32_t charId = recv.read_int();
		int32_t currentHp = recv.read_int();
		int32_t maxHp = recv.read_int();
		std::cout << charId << ": " << currentHp << "/" << maxHp << " HP" << std::endl;
	}

	void PartyOperationHandler::create_party(InPacket& recv) const
	{
		int32_t pid = recv.read_int();
		if (Stage::get().get_parties().count(pid) == 0)
		{
			std::cout << "Failed to create and join party #" << pid << "!" << std::endl;
			return;
		}
		Player& player = Stage::get().get_player();
		player.set_party_id(pid);
		recv.read_int();	// Mystic Door destination
		recv.read_int();	// Mystic Door origin
		recv.read_int();	// Mystic Door position x
		recv.read_int();	// Mystic Door position y
		update_ui(Stage::get().get_parties()[pid]);
		std::cout << "Party created" << std::endl;
	}

	void PartyOperationHandler::leave_party(InPacket& recv) const
	{
		int32_t pid = recv.read_int();
		std::map<int32_t, Party> &parties = Stage::get().get_parties();
		if (parties.count(pid) == 0)
		{
			std::cout << "Attempting to leave non-existent party #" << pid << "!" << std::endl;
			return;
		}
		int32_t target = recv.read_int();
		int8_t operation = recv.read_byte();
		Party& party = Stage::get().get_parties()[pid];
		if (operation == 0)	// disband
		{
			party.disband();
			std::cout << "Party #" << pid << " has been disbanded" << std::endl;
			recv.read_int();	// Party ID repeat
			parties.erase(pid);
		}
		else if (operation == 1)
		{
			int8_t op2 = recv.read_byte();
			std::string victim_name = recv.read_string();
			party.remove_from_party(target);
			if (op2 == 0) // leave
				std::cout << victim_name << " left the party" << std::endl;
			else if (op2 == 1) // expel
				std::cout << victim_name << " was expelled from the party" << std::endl;

			edit_party(party, recv);
		}
		update_ui(nullptr);
	}

	void PartyOperationHandler::join_party(InPacket& recv) const
	{
		int32_t pid = recv.read_int();
		std::string joining = recv.read_string();
		std::cout << joining << " is joining party #" << pid << std::endl;
		edit_party(Stage::get().get_parties()[pid], recv);
	}

	void PartyOperationHandler::update_party(InPacket& recv) const
	{
		std::map<int32_t, Party> &parties = Stage::get().get_parties();
		int32_t pid = recv.read_int();
		if (parties.count(pid) == 0)
		{
			parties[pid] = Party(pid);
		}

		Player& player = Stage::get().get_player();
		if (player.get_party_id() == 0)
		{
			std::cout << "Assuming player is in party #" << pid << ", review members" << std::endl;
			Stage::get().get_player().set_party_id(pid);
		}
		else if (player.get_party_id() != pid)
		{
			std::cout << "Non-relevant party #" << pid << " info sent?" << std::endl;
		}

		// TODO cleanup invalid entries (probably not here)
		edit_party(parties[pid], recv);
	}

	void PartyOperationHandler::edit_party(Party& party, InPacket& recv) const
	{
		std::cout << "editing party #" << party.get_id() << std::endl;
		PartyMember *members = party.get_party_members();
		uint8_t size = 0;

		// Get party member IDs and count party size
		for (int i = 0; i < Party::MAX_PARTY_SIZE; i++)
		{
			int32_t cid = recv.read_int();
			members[i].cid = cid;
			if (cid != 0)
			{
				size++;
			}
		}

		// Get party member names
		for (int i = 0; i < size; i++)
		{
			members[i].name = recv.read_padded_string(13);
		}
		recv.skip((Party::MAX_PARTY_SIZE - size) * 13);

		// Get party member jobs
		for (int i = 0; i < size; i++)
		{
			members[i].job = (uint16_t)recv.read_int();
		}
		recv.skip((Party::MAX_PARTY_SIZE - size) * 4);

		// Get party member levels
		for (int i = 0; i < size; i++)
		{
			members[i].level = (uint16_t)recv.read_int();
		}
		recv.skip((Party::MAX_PARTY_SIZE - size) * 4);

		// Get party member channels (0-based, -2 if offline)
		for (int i = 0; i < size; i++)
		{
			members[i].channel = recv.read_int();
		}
		recv.skip((Party::MAX_PARTY_SIZE - size) * 4);

		// Get party leader ID
		party.set_leader_id(recv.read_int());

		// Get party member maps (0 if N/A)
		for (int i = 0; i < size; i++)
		{
			members[i].map = recv.read_int();
			// do leader check here to avoid another loop
			members[i].leader = members[i].cid == party.get_leader_id();
		}
		recv.skip((Party::MAX_PARTY_SIZE - size) * 4);

		// Mystic door info: destination (int), origin (int), x-coord (int), y-coord (int)
		// = 4 bytes * 4 vars * 6 members = 96
		// unused right now
		recv.skip(Party::MAX_PARTY_SIZE * 16);

		party.set_size(size);

		// TODO determine if UI needs update (round 2)
		update_ui(party);
	}

	void PartyOperationHandler::update_ui(Optional<Party> party) const
	{
		if (auto userlist = UI::get().get_element<UIUserList>())
			userlist->update_party(party);
	}
}
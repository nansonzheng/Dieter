#include "Party.h"
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

#include "Party.h"

namespace ms 
{
	ms::Party::Party(int32_t pid) : party_id(pid) {}

	ms::Party::Party() : party_id(-1) {}

	bool Party::add_to_party(int32_t charId, PartyMember party_char)
	{
		if (!is_in_party())
			return false;
		if (size >= MAX_PARTY_SIZE)
			return false;
		party_members[size++] = party_char;
		return true;
	}

	bool Party::remove_from_party(int32_t charId)
	{
		// TODO determine if needed
		for (int i = 0; i < size; i++)
		{
			if (party_members[i].cid == charId)
			{
				party_members[i].clear();
				size--;
				// Party update should handle leaves
				return true;
			}
		}
		return false;
	}

	bool Party::remove_from_party(std::string char_name)
	{
		// TODO determine if needed
		return false;
	}

	bool Party::pass_leadership(int32_t charId)
	{
		leader_id = charId;
		// TODO update components
		return true;
	}

	bool Party::disband()
	{
		// TODO determine if needed

		return false;
	}

	bool Party::is_in_party()
	{
		return false;
	}

	int32_t Party::get_id()
	{
		return party_id;
	}

	void Party::set_id(int32_t new_id)
	{
		party_id = new_id;
	}

	int32_t ms::Party::get_leader_id()
	{
		return leader_id;
	}

	void ms::Party::set_leader_id(int32_t new_id)
	{
		leader_id = new_id;
	}

	PartyMember *Party::get_party_members()
	{
		return party_members;
	}

	const PartyMember *Party::get_party_members() const
	{
		return party_members;
	}
	uint8_t Party::get_size()
	{
		return size;
	}
	void Party::set_size(uint8_t s)
	{
		size = s;
	}
}
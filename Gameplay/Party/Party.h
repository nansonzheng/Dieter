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
#pragma once

#include "../../Character/Char.h"
#include "../../Character/Job.h"

#include <map>

namespace ms
{
	struct PartyMember
	{
		int32_t cid;
		std::string name;
		uint16_t maxHp;
		uint16_t currentHp;
		uint16_t level;
		uint16_t job;
		int32_t channel;
		int32_t map;
		bool online;
		bool leader;
		// TODO determine if char/player reference is needed

		void clear()
		{
			cid = 0;
			name = "";
			maxHp = 0;
			currentHp = 0;
			level = 0;
			job = 0;
			channel = -2;
			map = 999999999;
			online = false;
			leader = false;
		}
	};

	class Party
	{
	public:
		static const int MAX_PARTY_SIZE = 6;

		Party(int32_t pid);
		Party();	// Something is calling default constructor

		bool add_to_party(int32_t charId, PartyMember party_char);

		bool remove_from_party(int32_t charId);
		bool remove_from_party(std::string char_name);

		bool pass_leadership(int32_t charId);

		bool disband();

		bool is_in_party();

		int32_t get_id();
		void set_id(int32_t new_id);

		int32_t get_leader_id();
		void set_leader_id(int32_t new_id);

		PartyMember *get_party_members();
		const PartyMember *get_party_members() const;

		uint8_t get_size();
		void set_size(uint8_t s);
		
	private:
		int32_t party_id;
		PartyMember party_members[MAX_PARTY_SIZE];
		uint8_t size;
		int32_t leader_id;
	};
}
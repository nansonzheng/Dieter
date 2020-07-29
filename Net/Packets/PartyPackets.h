#pragma once
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

#include "../OutPacket.h"

namespace ms 
{
	class PartyCreatePacket : public OutPacket 
	{
	public:
		PartyCreatePacket() : OutPacket(OutPacket::Opcode::PARTY_OPERATION)
		{
			write_byte(1);
		}
	};

	// Party Leave also handles Disband
	class PartyLeavePacket : public OutPacket
	{
	public:
		PartyLeavePacket() : OutPacket(OutPacket::Opcode::PARTY_OPERATION)
		{
			write_byte(2);
		}
	};

	class PartyJoinPacket : public OutPacket
	{
	public:
		PartyJoinPacket(int32_t partyId) : OutPacket(OutPacket::Opcode::PARTY_OPERATION)
		{
			write_byte(3);
			write_int(partyId);
		}
	};

	class PartyInvitePacket : public OutPacket
	{
	public:
		PartyInvitePacket(std::string char_name) : OutPacket(OutPacket::Opcode::PARTY_OPERATION)
		{
			write_byte(4);
			write_string(char_name);
		}
	};

	class PartyExpelPacket : public OutPacket
	{
	public:
		PartyExpelPacket(int32_t charId) : OutPacket(OutPacket::Opcode::PARTY_OPERATION)
		{
			write_byte(5);
			write_int(charId);
		}
	};

	class PartyChangeLeaderPacket : public OutPacket
	{
	public:
		PartyChangeLeaderPacket(int32_t charId) : OutPacket(OutPacket::Opcode::PARTY_OPERATION)
		{
			write_byte(6);
			write_int(charId);
		}
	};

	class DenyPartyRequestPacket : public OutPacket
	{
	public:
		DenyPartyRequestPacket(std::string own_name) : OutPacket(OutPacket::Opcode::DENY_PARTY_REQUEST)
		{
			skip(1);
			write_string(own_name);
		}
	};
}
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

#include "../PacketHandler.h"

#include "../../Gameplay/Party/Party.h"

#include "../../Template/Optional.h"

namespace ms
{
	class PartyOperationHandler : public PacketHandler
	{
	public:
		void handle(InPacket& recv) const override;

	private:
		void create_party(InPacket& recv) const;
		void update_party(InPacket& recv) const;
		void leave_party(InPacket& recv) const;
		void join_party(InPacket& recv) const;
		void edit_party(Party& party, InPacket& recv) const;
		void update_ui(Optional<Party> party) const;
	};

	class PartyMemberHPHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};
}

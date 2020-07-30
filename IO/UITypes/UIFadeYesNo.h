#pragma once
//////////////////////////////////////////////////////////////////////////////////
//  This file is part of the continued Journey MMORPG client                    //
//  Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton, Nanson Zheng        //
//                                                                              //
//  This program is free software: you can redistribute it and/or modify        //
//  it under the terms of the GNU Affero General Public License as published by //
//  the Free Software Foundation, either version 3 of the License, or           //
//  (at your option) any later version.                                         //
//                                                                              //
//  This program is distributed in the hope that it will be useful,             //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of              //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               //
//  GNU Affero General Public License for more details.                         //
//                                                                              //
//  You should have received a copy of the GNU Affero General Public License    //
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.      //
//////////////////////////////////////////////////////////////////////////////////

#include "../Components/Button.h"
#include "../UIElement.h"

namespace ms
{
    class UIFadeYesNo : private UIElement
    {
    public:

        static constexpr Type TYPE = UIElement::Type::FADEYESNO;
        static constexpr bool FOCUSED = false;
        static constexpr bool TOGGLED = false;

        UIFadeYesNo(Type type, std::function<void(bool yes)> handler);

        Type get_type();

    protected:

        enum Type
        {
            PARTY_INVITE,
            BUDDY_INVITE,
            GUILD_INVITE,
            ALLIANCE_INVITE,
            BUDDY_LOGIN,
            GUILD_LOGIN,
            ALLIANCE_LOGIN,
            GUILD_ANNOUNCEMENT,
            QUEST_COMPLETE,
            NUM_TYPES
        };

        enum Buttons
        {
            YES,
            NO,
            NUM_BUTTONS
        };

        void draw(float alpha) const override;
        void update() override;
        Button::State button_pressed(uint16_t buttonid) override;

    private:
        Type type;
        std::function<void(bool yes)> handler;
    };

    class UIFadeYesNoInvite : public UIFadeYesNo
    {
    public:

        enum InviteType
        {
            PARTY,
            BUDDY,
            GUILD,
            ALLIANCE,
            TRADE
        };

        UIFadeYesNoInvite(std::string inviter);

    protected:
        void draw(float alpha) const override;
        void update() override;

    private:
        std::string text;
    };
}
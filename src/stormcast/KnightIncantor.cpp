/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightIncantor.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>
#include <Board.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    KnightIncantor::Create,
    KnightIncantor::ValueToString,
    KnightIncantor::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Storm", {.m_intValue = (int)LoreOfTheStorm::None}, (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::Stormcaller, 1},
        {ParamType::Enum, "Lore of Invigoration", {.m_intValue = (int)LoreOfInvigoration::None}, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool KnightIncantor::s_registered = false;

KnightIncantor::KnightIncantor() :
    StormcastEternal("Knight-Incantor", 5, WOUNDS, 9, 3, false),
    m_staff(Weapon::Type::Melee, "Incantor's Staff", 2, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, KNIGHT_INCANTOR};
    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool KnightIncantor::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration)
{
    if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None))
    {
        return false;
    }

    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_staff);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateSpiritStorm(this)));
    if (storm != LoreOfTheStorm::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightIncantor::Create(const ParameterList &parameters)
{
    auto unit = new KnightIncantor();
    auto storm = (LoreOfTheStorm)GetEnumParam("Lore of the Storm", parameters, (int)LoreOfTheStorm::None);
    auto invigoration = (LoreOfInvigoration)GetEnumParam("Lore of Invigoration", parameters, (int)LoreOfInvigoration::None);

    bool ok = unit->configure(storm, invigoration);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KnightIncantor::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Knight-Incantor", factoryMethod);
    }
}

void KnightIncantor::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_staff);
}

std::string KnightIncantor::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Lore of the Storm")
    {
        return ToString((LoreOfTheStorm) parameter.m_intValue);
    }
    else if (parameter.m_name == "Lore of Invigoration")
    {
        return ToString((LoreOfInvigoration) parameter.m_intValue);
    }
    return ParameterValueToString(parameter);
}

int KnightIncantor::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheStorm storm;
    LoreOfInvigoration invigoration;
    if (FromString(enumString, storm))
    {
        return (int)storm;
    }
    else if (FromString(enumString, invigoration))
    {
        return (int) invigoration;
    }
    return 0;
}


} // namespace StormcastEternals
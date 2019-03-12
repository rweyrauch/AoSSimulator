/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/Archmage.h>
#include <UnitFactory.h>
#include <spells/ArcaneBolt.h>
#include <spells/MysticShield.h>

namespace EldritchCouncil
{
static FactoryMethod factoryMethod = {
    Archmage::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Boolean, "Steed", {.m_boolValue = false}, false, false, false},
    },
    ORDER,
    ELDRITCH_COUNCIL
};

bool Archmage::s_registered = false;

Archmage::Archmage() :
    Unit("Archmage", 6, WOUNDS, 7, 6, false),
    m_seerstaff(Weapon::Type::Melee, "Seerstaff", 2, 1, 4, 3, -1, 1),
    m_steedHooves(Weapon::Type::Melee, "Aelven Steed's Swift Hooves", 1, 2, 4, 5, 0, 1)
{
    m_keywords = {ORDER, AELF, ELDRITCH_COUNCIL, HERO, WIZARD, ARCHMAGE};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Archmage::configure(bool steed)
{
    m_steed = steed;

    Model model;
    model.addMeleeWeapon(&m_seerstaff);
    if (m_steed)
    {
        model.addMeleeWeapon(&m_steedHooves);
        m_move = 14;
    }
    addModel(model);

    m_knownSpells.push_back(std::make_unique<ArcaneBolt>(this));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void Archmage::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_seerstaff);
    if (m_steed)
    {
        visitor(&m_steedHooves);
    }
}

Unit *Archmage::Create(const ParameterList &parameters)
{
    auto unit = new Archmage();
    bool steed = GetBoolParam("Steed", parameters, false);

    bool ok = unit->configure(steed);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Archmage::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Archmage", factoryMethod);
    }
}

} //namespace EldritchCouncil
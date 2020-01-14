/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/TzaangorShaman.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    TzaangorShaman::Create,
    TzaangorShaman::ValueToString,
    TzaangorShaman::EnumStringToInt,
    {
    },
    CHAOS,
    { TZEENTCH, BEASTS_OF_CHAOS }
};

bool TzaangorShaman::s_registered = false;

Unit *TzaangorShaman::Create(const ParameterList &parameters)
{
    auto unit = new TzaangorShaman();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string TzaangorShaman::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int TzaangorShaman::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void TzaangorShaman::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tzaangor Shaman", factoryMethod);
    }
}

TzaangorShaman::TzaangorShaman() :
    Unit("Tzaangor Shaman", 16, WOUNDS, 6, 5, true),
    m_staff(Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3),
    m_dagger(Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1),
    m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, HERO, WIZARD, TZAANGOR_SHAMAN};
    m_weapons = {&m_staff, &m_dagger, &m_teethAndHorns};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool TzaangorShaman::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_dagger);
    model->addMeleeWeapon(&m_teethAndHorns);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch
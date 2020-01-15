/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "mawtribes/Slaughtermaster.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    Slaughtermaster::Create,
    nullptr,
    nullptr,
    Slaughtermaster::ComputePoints,
    {
    },
    DESTRUCTION,
    { OGOR_MAWTRIBES }
};

bool Slaughtermaster::s_registered = false;

Unit *Slaughtermaster::Create(const ParameterList &parameters)
{
    auto unit = new Slaughtermaster();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}


void Slaughtermaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Slaughtermaster", factoryMethod);
    }
}

Slaughtermaster::Slaughtermaster() :
    MawtribesBase("Slaughtermaster", 6, WOUNDS, 8, 5, false),
    m_stumpBlades(Weapon::Type::Melee, "Stump Blades", 1, RAND_2D6, 3, 3, 0, 1),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
    m_assortedWeapons(Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 3, 5, 5, 0, 1)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, WIZARD, BUTCHER, SLAUGHTERMASTER};
    m_weapons = {&m_stumpBlades, &m_bite, &m_assortedWeapons};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Slaughtermaster::configure()
{
    auto model = new Model(BASESIZE, wounds());

    model->addMeleeWeapon(&m_stumpBlades);
    model->addMeleeWeapon(&m_assortedWeapons);
    model->addMeleeWeapon(&m_bite);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    //m_knownSpells.push_back(std::make_unique<Rockchomper>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace OgorMawtribes

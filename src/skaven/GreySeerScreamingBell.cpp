/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeerScreamingBell.h>
#include <UnitFactory.h>

namespace Skaven
{
bool GreySeerOnScreamingBell::s_registered = false;

Unit *GreySeerOnScreamingBell::Create(const ParameterList &parameters)
{
    auto unit = new GreySeerOnScreamingBell();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GreySeerOnScreamingBell::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Grey Seer on Screaming Bell", *factoryMethod);
    }
}

GreySeerOnScreamingBell::GreySeerOnScreamingBell() :
    Skaventide("Grey Seer on Screaming Bell", 6, WOUNDS, 6, 4, false),
    m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 3, 4, 4, -1, 1),
    m_clawsAndFangs(Weapon::Type::Melee, "Tearing Claws and Fangs", 1, 4, 4, 3, -1, 2),
    m_spikes(Weapon::Type::Melee, "Rusty Spikes", 1, RAND_D6, 2, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, WAR_MACHINE, HERO, WIZARD, SCREAMING_BELL,
        GREY_SEER};
    m_weapons = {&m_staff, &m_clawsAndFangs, &m_spikes};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool GreySeerOnScreamingBell::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_clawsAndFangs);
    model->addMeleeWeapon(&m_spikes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace Skaven

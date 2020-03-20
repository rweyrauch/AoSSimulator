/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/ContortedEpitome.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace Slaanesh
{

bool TheContortedEpitome::s_registered = false;

TheContortedEpitome::TheContortedEpitome() :
    SlaaneshBase("The Contorted Epitome", 12, WOUNDS, 10, 5, false),
    m_ravagingClaws(Weapon::Type::Melee, "Ravaging Claws", 1, 9, 3, 4, -1, 1),
    m_coiledTentacles(Weapon::Type::Melee, "Coiled Tentacles", 3, 2, 3, 4, -2, 2)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, THE_CONTORTED_EPITOME};
    m_weapons = {&m_ravagingClaws, &m_coiledTentacles};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool TheContortedEpitome::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_ravagingClaws);
    model->addMeleeWeapon(&m_coiledTentacles);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *TheContortedEpitome::Create(const ParameterList &parameters)
{
    auto unit = new TheContortedEpitome();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TheContortedEpitome::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            TheContortedEpitome::Create,
            SlaaneshBase::ValueToString,
            SlaaneshBase::EnumStringToInt,
            TheContortedEpitome::ComputePoints,
            {
                {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
            },
            CHAOS,
            { SLAANESH }
        };
        s_registered = UnitFactory::Register("The Contorted Epitome", factoryMethod);
    }
}

Wounds TheContortedEpitome::applyWoundSave(const Wounds &wounds)
{
    // Swallow Energy
    Dice::RollResult mortalSaves;
    Dice::rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.mortal -= mortalSaves.rollsGE(2);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

} // Slannesh

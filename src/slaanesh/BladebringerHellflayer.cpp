/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/BladebringerHellflayer.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace Slaanesh
{

bool BladebringerOnHellflayer::s_registered = false;

BladebringerOnHellflayer::BladebringerOnHellflayer() :
    SlaaneshBase("Bladebringer, Herald on Hellflayer", 12, WOUNDS, 10, 4, false),
    m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 6, 3, 4, -1, 1),
    m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, HELLFLAYER, BLADEBRINGER};
    m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool BladebringerOnHellflayer::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_flensingWhips);
    model->addMeleeWeapon(&m_piercingClaws);
    model->addMeleeWeapon(&m_poisonedTongues);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *BladebringerOnHellflayer::Create(const ParameterList &parameters)
{
    auto unit = new BladebringerOnHellflayer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BladebringerOnHellflayer::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            BladebringerOnHellflayer::Create,
            SlaaneshBase::ValueToString,
            SlaaneshBase::EnumStringToInt,
            BladebringerOnHellflayer::ComputePoints,
            {
                {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
            },
            CHAOS,
            { SLAANESH }
        };
        s_registered = UnitFactory::Register("Bladebringer, Herald on Hellflayer", factoryMethod);
    }
}

} // Slannesh

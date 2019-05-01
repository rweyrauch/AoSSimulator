/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Hellflayer.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    Hellflayer::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAANESH
};

bool Hellflayer::s_registered = false;

Hellflayer::Hellflayer() :
    Unit("Hellflayer", 12, WOUNDS, 10, 4, false),
    m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 4, 3, 4, -1, 1),
    m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HELLFLAYER};
}

bool Hellflayer::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_flensingWhips);
    model.addMeleeWeapon(&m_piercingClaws);
    model.addMeleeWeapon(&m_poisonedTongues);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Hellflayer::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_flensingWhips);
    visitor(&m_piercingClaws);
    visitor(&m_poisonedTongues);
}

Unit *Hellflayer::Create(const ParameterList &parameters)
{
    auto unit = new Hellflayer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Hellflayer::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Hellflayer", factoryMethod);
    }
}

} // Slannesh
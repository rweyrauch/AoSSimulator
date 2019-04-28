/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Viceleader.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    ViceleaderHeraldOfSlaanesh::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAANESH
};

bool ViceleaderHeraldOfSlaanesh::s_registered = false;

ViceleaderHeraldOfSlaanesh::ViceleaderHeraldOfSlaanesh() :
    Unit("Viceleader Herald of Slaanesh", 6, WOUNDS, 10, 5, false),
    m_ravagingClaws(Weapon::Type::Melee, "Ravaging Claws", 1, 6, 3, 4, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, VICELEADER};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool ViceleaderHeraldOfSlaanesh::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_ravagingClaws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void ViceleaderHeraldOfSlaanesh::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ravagingClaws);
}

Unit *ViceleaderHeraldOfSlaanesh::Create(const ParameterList &parameters)
{
    auto unit = new ViceleaderHeraldOfSlaanesh();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ViceleaderHeraldOfSlaanesh::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Viceleader Herald of Slaanesh", factoryMethod);
    }
}

} // Slannesh

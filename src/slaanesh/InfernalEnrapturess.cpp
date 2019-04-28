/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/InfernalEnrapturess.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    InfernalEnrapturess::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAANESH
};

bool InfernalEnrapturess::s_registered = false;

InfernalEnrapturess::InfernalEnrapturess() :
    Unit("Infernal Enrapturess Herald of Slaanesh", 6, WOUNDS, 10, 5, false),
    m_ravagingClaw(Weapon::Type::Melee, "Ravaging Claw", 1, 3, 3, 4, -1, 1),
    m_lyreCacophonousMelody(Weapon::Type::Missile, "Heartstring Lyre: Cacophonous Melody", 18, 6, 3, 4, -1, 1),
    m_lyreEuphonicBlast(Weapon::Type::Missile, "Heartstring Lyre: Euphonic Blast", 24, 1, 2, 3, -3, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, HERALD_OF_SLAANESH, INFERNAL_ENRAPTURESS};

    // Discordant Disruption
    m_totalUnbinds = 1;
}

bool InfernalEnrapturess::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMissileWeapon(&m_lyreCacophonousMelody);
    model.addMissileWeapon(&m_lyreEuphonicBlast);
    model.addMeleeWeapon(&m_ravagingClaw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void InfernalEnrapturess::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ravagingClaw);
    visitor(&m_lyreCacophonousMelody);
    visitor(&m_lyreEuphonicBlast);
}

Unit *InfernalEnrapturess::Create(const ParameterList &parameters)
{
    auto unit = new InfernalEnrapturess();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void InfernalEnrapturess::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Infernal Enrapturess Herald of Slaanesh", factoryMethod);
    }
}

void InfernalEnrapturess::onStartShooting(PlayerId player)
{
    if (player == m_owningPlayer)
    {
        auto enemyRoster = Board::Instance()->getPlayerRoster(GetEnemyId(m_owningPlayer));

        Unit* nearestUnit = enemyRoster ? enemyRoster->nearestUnit(this) : nullptr;
        if (nearestUnit)
        {
            float rangeTo = distanceTo(nearestUnit);
            if (rangeTo < m_lyreCacophonousMelody.range())
            {
                m_lyreCacophonousMelody.activate(true);
                m_lyreEuphonicBlast.activate(false);
            }
            else
            {
                m_lyreCacophonousMelody.activate(false);
                m_lyreEuphonicBlast.activate(true);
            }
        }
    }
}

} // Slannesh

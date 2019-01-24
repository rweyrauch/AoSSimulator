/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/CelestarBallista.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include <iostream>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    CelestarBallista::Create,
    nullptr,
    nullptr,
    {
    }
};

bool CelestarBallista::s_registered = false;

Weapon CelestarBallista::s_stormboltsSingle(Weapon::Type::Missile, "Celestar Stormbolts: Single Shot", 36, 1, 3, 3, -2, 1);
Weapon CelestarBallista::s_stormboltsRapid(Weapon::Type::Missile, "Celestar Stormbolts: Rapid Fire", 18, 4, 5, 3, -2, 1);
Weapon CelestarBallista::s_sigmariteBlades(Weapon::Type::Melee, "Sigmarite Blades", 1, 4, 4, 4, 0, 1);

CelestarBallista::CelestarBallista() :
    StormcastEternal("Celestar Ballista", 3, WOUNDS, 7, 4, false)
{
    // Burst of Celestial Energy
    s_stormboltsSingle.setHitsPerAttack(RAND_D6);
    s_stormboltsRapid.setHitsPerAttack(RAND_D6);

    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, ORDINATOS, WAR_MACHINE, CELESTAR_BALLISTA};
}

bool CelestarBallista::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&s_sigmariteBlades);

    m_stormboltsRapid = new Weapon(s_stormboltsRapid);
    m_stormboltsRapid->activate(true);
    m_stormboltsSingle = new Weapon(s_stormboltsSingle);
    m_stormboltsSingle->activate(false);

    model.addMissileWeapon(m_stormboltsSingle);
    model.addMissileWeapon(m_stormboltsRapid);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_stormboltsRapid.name() << ": " << s_stormboltsRapid.strength() << std::endl;
        std::cout << "\t" << s_stormboltsSingle.name() << ": " << s_stormboltsSingle.strength() << std::endl;
    }

    return true;
}

Unit *CelestarBallista::Create(const ParameterList &parameters)
{
    auto ballista = new CelestarBallista();

    bool ok = ballista->configure();
    if (!ok)
    {
        delete ballista;
        ballista = nullptr;
    }
    return ballista;
}

void CelestarBallista::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Celestar Ballista", factoryMethod);
    }
}

void CelestarBallista::shooting(PlayerId player)
{
    auto board = Board::Instance();
    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
    if (nearestUnit)
    {
        float rangeTo = distanceTo(nearestUnit);
        if (rangeTo < m_stormboltsRapid->range())
        {
            m_stormboltsRapid->activate(true);
            m_stormboltsSingle->activate(false);
        }
        else
        {
            m_stormboltsRapid->activate(false);
            m_stormboltsSingle->activate(true);
        }
    }
    StormcastEternal::shooting(player);
}

} // namespace StormcastEternals

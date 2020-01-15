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
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    CelestarBallista::ComputePoints,
    {
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    { STORMCAST_ETERNAL }
};

bool CelestarBallista::s_registered = false;

CelestarBallista::CelestarBallista() :
    StormcastEternal("Celestar Ballista", 3, WOUNDS, 7, 4, false),
    m_stormboltsSingle(Weapon::Type::Missile, "Celestar Stormbolts: Single Shot", 36, 1, 3, 3, -2, 1),
    m_stormboltsRapid(Weapon::Type::Missile, "Celestar Stormbolts: Rapid Fire", 18, 4, 5, 3, -2, 1),
    m_sigmariteBlades(Weapon::Type::Melee, "Sigmarite Blades", 1, 4, 4, 4, 0, 1)
{
    // Burst of Celestial Energy
    m_stormboltsSingle.setHitsPerAttack(RAND_D6);
    m_stormboltsRapid.setHitsPerAttack(RAND_D6);

    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, ORDINATOS, WAR_MACHINE, CELESTAR_BALLISTA};
    m_weapons = {&m_stormboltsSingle, &m_stormboltsRapid, &m_sigmariteBlades};
}

bool CelestarBallista::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_sigmariteBlades);

    m_stormboltsRapid.activate(true);
    m_stormboltsSingle.activate(false);

    model->addMissileWeapon(&m_stormboltsSingle);
    model->addMissileWeapon(&m_stormboltsRapid);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *CelestarBallista::Create(const ParameterList &parameters)
{
    auto ballista = new CelestarBallista();

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    ballista->setStormhost(stormhost);

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

void CelestarBallista::onStartShooting(PlayerId player)
{
    auto board = Board::Instance();
    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
    {
        otherPlayer = PlayerId::Blue;
    }
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
    if (nearestUnit)
    {
        float rangeTo = distanceTo(nearestUnit);
        if (rangeTo < m_stormboltsRapid.range())
        {
            m_stormboltsRapid.activate(true);
            m_stormboltsSingle.activate(false);
        }
        else
        {
            m_stormboltsRapid.activate(false);
            m_stormboltsSingle.activate(true);
        }
    }
}

} // namespace StormcastEternals

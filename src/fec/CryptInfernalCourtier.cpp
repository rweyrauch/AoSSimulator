/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptInfernalCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptInfernalCourtier::Create,
    FleshEaterCourts::ValueToString,
    FleshEaterCourts::EnumStringToInt,
    CryptInfernalCourtier::ComputePoints,
    {
        {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt, FleshEaterCourts::Gristlegore, 1},
        {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None, FleshEaterCourts::DefendersOfTheRealm, 1},
    },
    DEATH,
    { FLESH_EATER_COURTS }
};

bool CryptInfernalCourtier::s_registered = false;

CryptInfernalCourtier::CryptInfernalCourtier() :
    FleshEaterCourts("Crypt Infernal Courtier", 12, WOUNDS, 10, 4, true),
    m_foetidBreath(Weapon::Type::Missile, "Foetid Breath", 9, 1, 4, 3, -1, RAND_D3),
    m_skeweringTalons(Weapon::Type::Melee, "Skewering Talons", 1, 5, 4, 3, -1, 2)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_INFERNAL_COURTIER};
    m_weapons = {&m_foetidBreath, &m_skeweringTalons};
}

bool CryptInfernalCourtier::configure()
{
    auto infernal = new Model(BASESIZE, WOUNDS);
    infernal->addMissileWeapon(&m_foetidBreath);
    infernal->addMeleeWeapon(&m_skeweringTalons);
    addModel(infernal);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *CryptInfernalCourtier::Create(const ParameterList &parameters)
{
    auto unit = new CryptInfernalCourtier();

    auto court = (GrandCourt)GetEnumParam("Grand Court", parameters, NoCourt);
    auto delusion = (Delusion)GetEnumParam("Delusion", parameters, None);
    // TODO: error checks - can only select delusion if GrandCourt is NoCourt.
    unit->setGrandCourt(court);
    unit->setCourtsOfDelusion(delusion);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptInfernalCourtier::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Infernal Courtier", factoryMethod);
    }
}

Wounds CryptInfernalCourtier::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Skewering Strike
    if ((hitRoll == 6) && weapon->name() == m_skeweringTalons.name())
    {
        // Two mortal wounds.
        return {0, 2};
    }
    return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace FleshEaterCourt

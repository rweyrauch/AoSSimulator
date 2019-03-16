/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptHaunterCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptHaunterCourtier::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool CryptHaunterCourtier::s_registered = false;

CryptHaunterCourtier::CryptHaunterCourtier() :
    FleshEaterCourts("Crypt Haunter Courtier", 12, WOUNDS, 10, 4, true),
    m_massiveBoneClub(Weapon::Type::Melee, "Massive Bone Clube", 1, 3, 4, 3, 0, 3),
    m_rancidTalons(Weapon::Type::Melee, "Rancid Talons", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_HAUNTER_COURTIER};
}

bool CryptHaunterCourtier::configure()
{
    Model infernal(BASESIZE, WOUNDS);
    infernal.addMeleeWeapon(&m_massiveBoneClub);
    infernal.addMeleeWeapon(&m_rancidTalons);
    addModel(infernal);

    m_points = POINTS_PER_UNIT;

    return true;
}

void CryptHaunterCourtier::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_massiveBoneClub);
    visitor(&m_rancidTalons);
}

Unit *CryptHaunterCourtier::Create(const ParameterList &parameters)
{
    auto unit = new CryptHaunterCourtier();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptHaunterCourtier::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Haunter Courtier", factoryMethod);
    }
}

} // namespace FleshEaterCourt

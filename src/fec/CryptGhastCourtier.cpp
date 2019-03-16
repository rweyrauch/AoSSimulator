/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptGhastCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptGhastCourtier::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool CryptGhastCourtier::s_registered = false;

CryptGhastCourtier::CryptGhastCourtier() :
    FleshEaterCourts("Crypt Ghast Courtier",6, WOUNDS, 10, 5, false),
    m_boneClub(Weapon::Type::Melee, "Bone Club", 1, 3, 3, 3, 0, 1),
    m_filthyClaws(Weapon::Type::Melee, "Filthy Claws", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_GHAST_COURTIER};
}

bool CryptGhastCourtier::configure()
{
    Model infernal(BASESIZE, WOUNDS);
    infernal.addMeleeWeapon(&m_boneClub);
    infernal.addMeleeWeapon(&m_filthyClaws);
    addModel(infernal);

    m_points = POINTS_PER_UNIT;

    return true;
}

void CryptGhastCourtier::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boneClub);
    visitor(&m_filthyClaws);
}

Unit *CryptGhastCourtier::Create(const ParameterList &parameters)
{
    auto unit = new CryptGhastCourtier();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptGhastCourtier::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Ghast Courtier", factoryMethod);
    }
}

} // namespace FleshEaterCourt

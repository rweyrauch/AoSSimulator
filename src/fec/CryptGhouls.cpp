/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptGhouls.h>
#include <UnitFactory.h>
#include <Board.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptGhouls::Create,
    FleshEaterCourts::ValueToString,
    FleshEaterCourts::EnumStringToInt,
    CryptGhouls::ComputePoints,
    {
        {
            ParamType::Integer, "Models", CryptGhouls::MIN_UNIT_SIZE, CryptGhouls::MIN_UNIT_SIZE,
            CryptGhouls::MAX_UNIT_SIZE, CryptGhouls::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt, FleshEaterCourts::Gristlegore, 1},
        {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None, FleshEaterCourts::DefendersOfTheRealm, 1},
    },
    DEATH,
    { FLESH_EATER_COURTS }
};

bool CryptGhouls::s_registered = false;

CryptGhouls::CryptGhouls() :
    FleshEaterCourts("Crypt Ghouls", 6, WOUNDS, 10, 6, false),
    m_teethAndClaws(Weapon::Type::Melee, "Sharpened Teeth and Filthy Claws", 1, 2, 4, 4, 0, 1),
    m_teethAndClawsGhast(Weapon::Type::Melee, "Sharpened Teeth and Filthy Claws", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, SERFS, CRYPT_GHOULS};
    m_weapons = {&m_teethAndClaws, &m_teethAndClawsGhast};
}

bool CryptGhouls::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto ghast = new Model(BASESIZE, WOUNDS);
    ghast->addMeleeWeapon(&m_teethAndClawsGhast);
    addModel(ghast);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_teethAndClaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *CryptGhouls::Create(const ParameterList &parameters)
{
    auto unit = new CryptGhouls();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto court = (GrandCourt)GetEnumParam("Grand Court", parameters, NoCourt);
    auto delusion = (Delusion)GetEnumParam("Delusion", parameters, None);
    // TODO: error checks - can only select delusion if GrandCourt is NoCourt.
    unit->setGrandCourt(court);
    unit->setCourtsOfDelusion(delusion);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptGhouls::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Ghouls", factoryMethod);
    }
}

int CryptGhouls::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = FleshEaterCourts::extraAttacks(nullptr, weapon, target);

    // Boundless Ferocity
    if (remainingModels() >= 20)
    {
        attacks += 1;
    }

    return attacks;
}

Rerolls CryptGhouls::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Royal Approval
    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ABHORRANT, 18.0f);
    if (unit != nullptr)
    {
        return RerollOnes;
    }
    return FleshEaterCourts::toHitRerolls(weapon, target);
}

int CryptGhouls::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace FleshEaterCourt

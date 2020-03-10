/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/RipperdactylRiders.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{

bool RipperdactylRiders::s_registered = false;

RipperdactylRiders::RipperdactylRiders() :
    SeraphonBase("Ripperdactyl Riders", 12, WOUNDS, 5, 5, true),
    m_spear(Weapon::Type::Melee, "Moonstone Warspear", 1, 1, 4, 4, 0, 1),
    m_spearAlpha(Weapon::Type::Melee, "Moonstone Warspear", 1, 2, 4, 4, 0, 1),
    m_jaws(Weapon::Type::Melee, "Tearing Jaws", 1, 3, 4, 3, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINK, RIPPERDACTYL, RIPPERDACTYL_RIDERS};
    m_weapons = {&m_spear, &m_spearAlpha, &m_jaws};
}

bool RipperdactylRiders::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // Add the Alpha
    auto alpha = new Model(BASESIZE, WOUNDS);
    alpha->addMeleeWeapon(&m_spearAlpha);
    alpha->addMeleeWeapon(&m_jaws);
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *RipperdactylRiders::Create(const ParameterList &parameters)
{
    auto unit = new RipperdactylRiders();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void RipperdactylRiders::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            RipperdactylRiders::Create,
            SeraphonBase::ValueToString,
            SeraphonBase::EnumStringToInt,
            RipperdactylRiders::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", RipperdactylRiders::MIN_UNIT_SIZE, RipperdactylRiders::MIN_UNIT_SIZE,
                    RipperdactylRiders::MAX_UNIT_SIZE, RipperdactylRiders::MIN_UNIT_SIZE
                },
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Ripperdactyl Riders", *factoryMethod);
    }
}

int RipperdactylRiders::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int RipperdactylRiders::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Voracious Appetite
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_jaws.name()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

Rerolls RipperdactylRiders::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if (!m_usedToad && (weapon->name() == m_jaws.name()))
    {
        m_usedToad = true;
        return RerollFailed;
    }

    return Unit::toHitRerolls(weapon, target);
}

} //namespace Seraphon

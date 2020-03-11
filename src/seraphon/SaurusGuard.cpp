/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusGuard.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{

bool SaurusGuard::s_registered = false;

SaurusGuard::SaurusGuard() :
    SeraphonBase("Saurus Guard", 5, WOUNDS, 8, 4, false),
    m_celestitePolearm(Weapon::Type::Melee, "Celestite Polearm", 1, 2, 3, 3, -1, 1),
    m_celestitePolearmAlpha(Weapon::Type::Melee, "Celestite Polearm", 1, 3, 3, 3, -1, 1),
    m_jaws(Weapon::Type::Melee, "Powerful Jaws", 1, 1, 5, 4, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SAURUS, SAURUS_GUARD};
    m_weapons = {&m_celestitePolearm, &m_celestitePolearmAlpha, &m_jaws};

    s_globalBraveryMod.connect(this, &SaurusGuard::stardrakeIcon, &m_connection);
}

SaurusGuard::~SaurusGuard()
{
    m_connection.disconnect();
}

bool SaurusGuard::configure(int numModels, bool iconBearer, bool wardrum)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_wardrum = wardrum;

    // Add the Alpha
    auto alpha = new Model(BASESIZE, WOUNDS);
    alpha->addMeleeWeapon(&m_celestitePolearmAlpha);
    alpha->addMeleeWeapon(&m_jaws);
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_celestitePolearm);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *SaurusGuard::Create(const ParameterList &parameters)
{
    auto unit = new SaurusGuard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
    bool wardrum = GetBoolParam("Wardrum", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, wardrum);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusGuard::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            SaurusGuard::Create,
            SeraphonBase::ValueToString,
            SeraphonBase::EnumStringToInt,
            SaurusGuard::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", SaurusGuard::MIN_UNIT_SIZE, SaurusGuard::MIN_UNIT_SIZE,
                    SaurusGuard::MAX_UNIT_SIZE, SaurusGuard::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Stardrake Icon", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Wardrum", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Saurus Guard", *factoryMethod);
    }
}

int SaurusGuard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

Rerolls SaurusGuard::chargeRerolls() const
{
    if (m_wardrum) return RerollFailed;

    return SeraphonBase::chargeRerolls();
}

int SaurusGuard::stardrakeIcon(const Unit *target)
{
    // Icon Bearer
    if (m_iconBearer && (target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 6.0f))
    {
        return -1;
    }

    return 0;
}

} //namespace Seraphon

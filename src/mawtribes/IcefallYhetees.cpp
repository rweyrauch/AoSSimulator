/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/IcefallYhetees.h"

namespace OgorMawtribes
{

bool IcefallYhetees::s_registered = false;

Unit *IcefallYhetees::Create(const ParameterList &parameters)
{
    auto unit = new IcefallYhetees();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void IcefallYhetees::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            IcefallYhetees::Create,
            MawtribesBase::ValueToString,
            MawtribesBase::EnumStringToInt,
            IcefallYhetees::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", IcefallYhetees::MIN_UNIT_SIZE, IcefallYhetees::MIN_UNIT_SIZE,
                    IcefallYhetees::MAX_UNIT_SIZE, IcefallYhetees::MIN_UNIT_SIZE
                },
                {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
            },
            DESTRUCTION,
            { OGOR_MAWTRIBES }
        };
        s_registered = UnitFactory::Register("Icefall Yhetees", factoryMethod);
    }
}

IcefallYhetees::IcefallYhetees() :
    MawtribesBase("Icefall Yhetees", 9, WOUNDS, 6, 6, false),
    m_clawsAndClubs(Weapon::Type::Melee, "Claws and Ice-encrusted Clubs", 1, 3, 4, 3, -1, 2)
{
    m_keywords = {DESTRUCTION, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, ICEFALL_YHETESS};
    m_weapons = {&m_clawsAndClubs};

    // Bounding Leaps
    m_pileInMove = 6;
}

bool IcefallYhetees::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_clawsAndClubs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int IcefallYhetees::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int IcefallYhetees::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    auto mod = Unit::targetHitModifier(weapon, attacker);

    // Aura of Frost
    if (!weapon->isMissile()) mod--;

    return mod;
}

} // namespace OgorMawtribes

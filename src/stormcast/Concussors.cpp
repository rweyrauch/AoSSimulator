/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Concussors.h>
#include <UnitFactory.h>

namespace StormcastEternals
{

bool Concussors::s_registered = false;

Concussors::Concussors() :
    StormcastEternal("Concussors", 10, WOUNDS, 7, 3, false),
    m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
    m_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, CONCUSSORS};
    m_weapons = {&m_stormBlast, &m_lightningHammer, &m_clawsAndFangs};
}

bool Concussors::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (int i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_stormBlast);
        model->addMeleeWeapon(&m_lightningHammer);
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Concussors::Create(const ParameterList &parameters)
{
    auto unit = new Concussors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Concussors::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            StormcastEternal::ValueToString,
            StormcastEternal::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Concussors", *factoryMethod);
    }
}

Wounds Concussors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Blast to Ashes
    if ((hitRoll == 6) && (weapon->name() == m_lightningHammer.name()))
    {
        return {0, 2};
    }

    // Intolerable Damage
    if ((hitRoll == 6) && (weapon->name() == m_clawsAndFangs.name()))
    {
        return {Dice::rollD6(), 0};
    }

    // Storm Blast
    if (weapon->name() == m_stormBlast.name())
    {
        return {0, Dice::rollD3()};
    }

    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls Concussors::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    return RerollOnes;
}

int Concussors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace StormcastEternals
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
static FactoryMethod factoryMethod = {
    Concussors::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = Concussors::MIN_UNIT_SIZE}, Concussors::MIN_UNIT_SIZE,
            Concussors::MAX_UNIT_SIZE, Concussors::MIN_UNIT_SIZE
        },
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Concussors::s_registered = false;

Concussors::Concussors() :
    StormcastEternal("Concussors", 10, WOUNDS, 7, 3, false),
    m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
    m_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, CONCUSSORS};
}

bool Concussors::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (int i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_stormBlast);
        model.addMeleeWeapon(&m_lightningHammer);
        model.addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Concussors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_stormBlast);
    visitor(&m_lightningHammer);
    visitor(&m_clawsAndFangs);
}

Unit *Concussors::Create(const ParameterList &parameters)
{
    auto unit = new Concussors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
        s_registered = UnitFactory::Register("Concussors", factoryMethod);
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
        Dice dice;
        return {dice.rollD6(), 0};
    }

    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls Concussors::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    return RerollOnes;
}

} //namespace StormcastEternals
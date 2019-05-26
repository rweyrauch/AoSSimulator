/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Fulminators.h>
#include <UnitFactory.h>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Fulminators::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Fulminators::MIN_UNIT_SIZE, Fulminators::MIN_UNIT_SIZE,
            Fulminators::MAX_UNIT_SIZE, Fulminators::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Fulminators::s_registered = false;

Fulminators::Fulminators() :
    StormcastEternal("Fulminators", 10, WOUNDS, 7, 3, false),
    m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
    m_stormstrkeGlaive(Weapon::Type::Melee, "Stormstrike Glaive", 2, 3, 3, 3, -1, 1),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, FULMINATORS};
}

bool Fulminators::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (int i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_stormBlast);
        model.addMeleeWeapon(&m_stormstrkeGlaive);
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

void Fulminators::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_stormBlast);
    visitor(&m_stormstrkeGlaive);
    visitor(&m_clawsAndFangs);
}

Unit *Fulminators::Create(const ParameterList &parameters)
{
    auto unit = new Fulminators();
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

void Fulminators::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Fulminators", factoryMethod);
    }
}

Wounds Fulminators::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Impaling Strike
    if (m_charged && (weapon->name() == m_clawsAndFangs.name()))
    {
        return {weapon->damage()+2, 0};
    }

    // Intolerable Damage
    if ((hitRoll == 6) && (weapon->name() == m_clawsAndFangs.name()))
    {
        Dice dice;
        return {dice.rollD6(), 0};
    }

    // Storm Blast
    if (weapon->name() == m_stormBlast.name())
    {
        Dice dice;
        return {0, dice.rollD3()};
    }

    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls Fulminators::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    return RerollOnes;
}

int Fulminators::toSaveModifier(const Weapon *weapon) const
{
    int modifier = StormcastEternal::toSaveModifier(weapon);
    // Glaivewall
    if (weapon->isMissile())
    {
        modifier += 1;
    }
    return modifier;
}
} //namespace StormcastEternals
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Tempestors.h>
#include <UnitFactory.h>
#include <Board.h>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Tempestors::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Tempestors::MIN_UNIT_SIZE, Tempestors::MIN_UNIT_SIZE,
            Tempestors::MAX_UNIT_SIZE, Tempestors::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Tempestors::s_registered = false;

Tempestors::Tempestors() :
    StormcastEternal("Tempestors", 10, WOUNDS, 7, 3, false),
    m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
    m_volleystormCrossbow(Weapon::Type::Missile, "Volleystorm Crossbow", 12, 4, 3, 4, 0, 1),
    m_warblade(Weapon::Type::Melee, "Warblade", 1, 3, 3, 4, 0, 1),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, TEMPESTORS};
    m_weapons = {&m_stormBlast, &m_volleystormCrossbow, &m_warblade, &m_clawsAndFangs};
}

bool Tempestors::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (int i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_stormBlast);
        model->addMissileWeapon(&m_volleystormCrossbow);
        model->addMeleeWeapon(&m_warblade);
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *Tempestors::Create(const ParameterList &parameters)
{
    auto unit = new Tempestors();
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

void Tempestors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tempestors", factoryMethod);
    }
}

Wounds Tempestors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
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

Rerolls Tempestors::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    return RerollOnes;
}

int Tempestors::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = StormcastEternal::targetHitModifier(weapon, attacker);

    // Disruptive Fire
    if (weapon->isMissile())
    {
        auto unit = Board::Instance()->getUnitWithKeyword(attacker, m_owningPlayer, TEMPESTORS, 12);
        if (unit != nullptr)
            modifier -= 1;
    }
    return modifier;
}
} //namespace StormcastEternals
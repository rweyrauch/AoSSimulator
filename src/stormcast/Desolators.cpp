/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Desolators.h>
#include <UnitFactory.h>

namespace StormcastEternals
{
static const int BASESIZE = 90; // x52 oval
static const int WOUNDS = 5;
static const int MIN_UNIT_SIZE = 2;
static const int MAX_UNIT_SIZE = 12;
static const int POINTS_PER_BLOCK = 220;
static const int POINTS_MAX_UNIT_SIZE = 1320;

bool Desolators::s_registered = false;

Desolators::Desolators() :
    StormcastEternal("Desolators", 10, WOUNDS, 7, 3, false),
    m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
    m_thunderAxe(Weapon::Type::Melee, "Thunderaxe", 2, 3, 4, 3, -1, 2),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, DESOLATORS};
    m_weapons = {&m_stormBlast, &m_thunderAxe, &m_clawsAndFangs};
}

bool Desolators::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (int i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_stormBlast);
        model->addMeleeWeapon(&m_thunderAxe);
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Desolators::Create(const ParameterList &parameters)
{
    auto unit = new Desolators();
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

void Desolators::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
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

        s_registered = UnitFactory::Register("Desolators", factoryMethod);
    }
}

Wounds Desolators::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
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

Rerolls Desolators::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    return RerollOnes;
}

int Desolators::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = StormcastEternal::extraAttacks(nullptr, weapon, target);
    if (weapon->name() == m_thunderAxe.name())
    {
        if (remainingModels() >= 6)
            attacks += 2;
        else if (remainingModels() >= 4)
            attacks += 1;
    }
    return attacks;
}

int Desolators::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace StormcastEternals
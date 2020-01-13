/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Stabbas.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz
{

static FactoryMethod factoryMethod = {
    Stabbas::Create,
    Stabbas::ValueToString,
    Stabbas::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Stabbas::MIN_UNIT_SIZE,
            Stabbas::MIN_UNIT_SIZE, Stabbas::MAX_UNIT_SIZE, Stabbas::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", Stabbas::Stabba, Stabbas::Stabba, Stabbas::PokinSpear, 1},
        {ParamType::Enum, "Boss Weapon", Stabbas::Stabba, Stabbas::Stabba, Stabbas::PokinSpear, 1},
        {ParamType::Integer, "Barbed Nets", 3, 0, 3 * Stabbas::MAX_UNIT_SIZE / Stabbas::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Gong Bashers", 1, 0, Stabbas::MAX_UNIT_SIZE / Stabbas::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Flag Bearers", 1, 0, Stabbas::MAX_UNIT_SIZE / Stabbas::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Icon Bearers", 0, 0, Stabbas::MAX_UNIT_SIZE / Stabbas::MIN_UNIT_SIZE, 1},
    },
    DESTRUCTION,
    { GLOOMSPITE_GITZ }
};

bool Stabbas::s_registered = false;

Stabbas::Stabbas() :
    GloomspiteGitzBase("Stabbas", 5, WOUNDS, 4, 6, false),
    m_stabba(Weapon::Type::Melee, "Stabba", 1, 1, 4, 4, 0, 1),
    m_stabbaBoss(Weapon::Type::Melee, "Stabba", 1, 1, 3, 4, 0, 1),
    m_pokinSpear(Weapon::Type::Melee, "Pokin' Spear", 2, 1, 5, 4, 0, 1),
    m_pokinSpearBoss(Weapon::Type::Melee, "Pokin' Spear", 2, 1, 4, 4, 0, 1),
    m_barbedNet(Weapon::Type::Melee, "Barbed Net", 2, 3, 4, 5, 0, 1)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, STABBAS};
    m_weapons = {&m_stabba, &m_stabbaBoss, &m_pokinSpear, &m_pokinSpearBoss, &m_barbedNet};
}

bool Stabbas::configure(int numModels, WeaponOption weapons, WeaponOption bossWeapon, int numBarbedNets,
                        int numGongbashers, int numFlagbearers, int numIconbearers)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    if (numBarbedNets > 3 * numModels / MIN_UNIT_SIZE)
    {
        return false;
    }
    if (numGongbashers > numModels / MIN_UNIT_SIZE)
    {
        return false;
    }
    if (numFlagbearers + numIconbearers > numModels / MIN_UNIT_SIZE)
    {
        return false;
    }

    m_numFlagbearers = numFlagbearers;
    m_numGongbashers = numGongbashers;
    m_numIconbearers = numIconbearers;

    // Add the boss
    auto boss = new Model(BASESIZE, WOUNDS);
    if (bossWeapon == Stabba)
    {
        boss->addMeleeWeapon(&m_stabbaBoss);
        m_ranks = 2;
    }
    else if (bossWeapon == PokinSpear)
    {
        boss->addMeleeWeapon(&m_pokinSpearBoss);
        m_ranks = 3;
    }
    addModel(boss);

    // add the nets
    for (auto i = 1; i < numBarbedNets; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_barbedNet);
        addModel(model);
    }

    // and the rest
    auto currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == Stabba)
        {
            model->addMeleeWeapon(&m_stabba);
        }
        else if (weapons == PokinSpear)
        {
            model->addMeleeWeapon(&m_pokinSpear);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *Stabbas::Create(const ParameterList &parameters)
{
    auto unit = new Stabbas();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Stabba);
    WeaponOption bossWeapon = (WeaponOption) GetEnumParam("Boss Weapon", parameters, Stabba);
    int numBarbedNets = GetIntParam("Barbed Nets", parameters, 0);
    int numGongbashers = GetIntParam("Gong Bashers", parameters, 0);
    int numFlagbearers = GetIntParam("Flag Bearers", parameters, 0);
    int numIconbearers = GetIntParam("Icon Bearers", parameters, 0);

    bool ok = unit->configure(numModels, weapons, bossWeapon, numBarbedNets, numGongbashers, numFlagbearers, numIconbearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Stabbas::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Stabbas", factoryMethod);
    }
}

int Stabbas::toWoundModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = GloomspiteGitzBase::toWoundModifier(weapon, unit);
    // Backstabbing Mob
    if (!weapon->isMissile())
    {
        if (remainingModels() >= 30)
        {
            modifier += 2;
        }
        else if (remainingModels() >= 15)
        {
            modifier += 1;
        }
    }
    return modifier;
}

int Stabbas::runModifier() const
{
    int modifier = GloomspiteGitzBase::runModifier();
    if (m_numGongbashers > 0)
    {
        modifier += 2;
    }
    return modifier;
}

int Stabbas::toSaveModifier(const Weapon *weapon) const
{
    int modifier = GloomspiteGitzBase::toSaveModifier(weapon);
    if ((m_numIconbearers > 0) && weapon->isMissile())
    {
        modifier += 1;
    }
    // Moonshields
    if (remainingModels() >= 10)
    {
        modifier += 1;
    }
    return modifier;
}

int Stabbas::braveryModifier() const
{
    int modifier = GloomspiteGitzBase::braveryModifier();
    if (m_numFlagbearers > 0)
    {
        modifier += 1;
    }
    return modifier;
}

std::string Stabbas::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons" || std::string(parameter.name) == "Boss Weapon")
    {
        if (parameter.intValue == Stabba)
        {
            return "Stabba";
        }
        else if (parameter.intValue == PokinSpear)
        {
            return "Pokin' Spear";
        }
    }
    return ParameterValueToString(parameter);
}

int Stabbas::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Stabba")
    {
        return Stabba;
    }
    else if (enumString == "Pokin' Spear")
    {
        return PokinSpear;
    }
    return 0;
}

int Stabbas::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
    // Netters
    if (distanceTo(attacker) <= 2.0f)
    {
        bool hasNets = false;
        for (const auto& ip : m_models)
        {
            for (auto wip = ip->meleeWeaponBegin(); wip != ip->meleeWeaponEnd(); ++wip)
            {
                const Weapon *w = *wip;

                if (w->name() == m_barbedNet.name())
                {
                    hasNets = true;
                    break;
                }
            }
        }
        if (hasNets)
        {
            modifier -= 1;
        }
    }
    return modifier;
}

} // namespace GloomspiteGitz
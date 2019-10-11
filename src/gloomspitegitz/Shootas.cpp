/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Shootas.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz
{

static FactoryMethod factoryMethod = {
    Shootas::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Shootas::MIN_UNIT_SIZE,
            Shootas::MIN_UNIT_SIZE, Shootas::MAX_UNIT_SIZE, Shootas::MIN_UNIT_SIZE
        },
        {ParamType::Integer, "Barbed Nets", 0, 0, 3 * Shootas::MAX_UNIT_SIZE / Shootas::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Gong Bashers", 1, 0, Shootas::MAX_UNIT_SIZE / Shootas::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Flag Bearers", 1, 0, Shootas::MAX_UNIT_SIZE / Shootas::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Icon Bearers", 0, 0, Shootas::MAX_UNIT_SIZE / Shootas::MIN_UNIT_SIZE, 1},
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool Shootas::s_registered = false;

Shootas::Shootas() :
    GloomspiteGitzBase("Shootas", 5, WOUNDS, 4, 6, false),
    m_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1),
    m_slittaBoss(Weapon::Type::Melee, "Slitta", 1, 1, 4, 5, 0, 1),
    m_moonclanBow(Weapon::Type::Missile, "Moonclan Bow", 16, 1, 5, 5, 0, 1),
    m_moonclanBowBoss(Weapon::Type::Missile, "Moonclan Bow", 16, 1, 4, 5, 0, 1),
    m_barbedNet(Weapon::Type::Melee, "Barbed Net", 2, 3, 4, 5, 0, 1)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, SHOOTAS};
}


Unit *Shootas::Create(const ParameterList &parameters)
{
    auto unit = new Shootas();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numBarbedNets = GetIntParam("Barbed Nets", parameters, 0);
    int numGongbashers = GetIntParam("Gong Bashers", parameters, 0);
    int numFlagbearers = GetIntParam("Flag Bearers", parameters, 0);
    int numIconbearers = GetIntParam("Icon Bearers", parameters, 0);

    bool ok = unit->configure(numModels, numBarbedNets, numGongbashers, numFlagbearers, numIconbearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Shootas::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Shootas", factoryMethod);
    }
}

bool Shootas::configure(int numModels, int numBarbedNets, int numGongbashers, int numFlagbearers, int numIconbearers)
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
    Model boss(BASESIZE, WOUNDS);
    boss.addMeleeWeapon(&m_slittaBoss);
    boss.addMissileWeapon(&m_moonclanBowBoss);
    addModel(boss);

    // add the nets
    for (auto i = 1; i < numBarbedNets; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_barbedNet);
        addModel(model);
    }

    // and the rest
    auto currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_slitta);
        model.addMissileWeapon(&m_moonclanBow);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    m_ranks = 4;

    return true;
}

int Shootas::toWoundModifier(const Weapon *weapon, const Unit *unit) const
{
    // Backstabbing Mob
    int modifier = GloomspiteGitzBase::toWoundModifier(weapon, unit);
    if (!weapon->isMissile())
    {
        if (remainingModels() >= 30)
        { modifier += 2; }
        else if (remainingModels() >= 15)
        { modifier += 1; }
    }
    return modifier;
}

int Shootas::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = GloomspiteGitzBase::toHitModifier(weapon, unit);
    // Moonclan Bows
    if (remainingModels() >= 15 && weapon->isMissile())
    { modifier += 1; }
    return modifier;
}

int Shootas::runModifier() const
{
    int modifier = GloomspiteGitzBase::runModifier();
    if (m_numGongbashers > 0)
    { modifier += 2; }
    return modifier;
}

int Shootas::braveryModifier() const
{
    int modifier = GloomspiteGitzBase::braveryModifier();
    if (m_numFlagbearers > 0)
    { modifier += 1; }
    return modifier;
}

int Shootas::toSaveModifier(const Weapon *weapon) const
{
    int modifier = GloomspiteGitzBase::toSaveModifier(weapon);
    if (m_numIconbearers > 0 && weapon->isMissile())
    {
        modifier += 1;
    }
    return modifier;
}

void Shootas::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_slitta);
    visitor(&m_slittaBoss);
    visitor(&m_moonclanBow);
    visitor(&m_moonclanBowBoss);
    visitor(&m_barbedNet);
}

int Shootas::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
    // Netters
    if (distanceTo(attacker) <= 2.0f)
    {
        bool hasNets = false;
        for (auto ip : m_models)
        {
            for (auto wip = ip.meleeWeaponBegin(); wip != ip.meleeWeaponEnd(); ++wip)
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
        { modifier -= 1; }
    }
    return modifier;
}

} // namespace GloomspiteGitz
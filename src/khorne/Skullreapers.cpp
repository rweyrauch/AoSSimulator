/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Skullreapers.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Skullreapers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = Skullreapers::MIN_UNIT_SIZE}, Skullreapers::MIN_UNIT_SIZE,
            Skullreapers::MAX_UNIT_SIZE, Skullreapers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", {.m_boolValue=true}, false, false, false},
    },
    CHAOS,
    KHORNE
};

bool Skullreapers::s_registered = false;

Skullreapers::Skullreapers() :
    Unit("Skullreapers", 5, WOUNDS, 7, 4, false),
    m_blades(Weapon::Type::Melee, "Gore-slick Blades, Daemonblade, Spinecleavers and Soultearers", 1, 4, 3, 3, 0, 1),
    m_viciousMutation(Weapon::Type::Melee, "Vicious Mutation", 1, 1, 3, 4, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, SKULLREAPERS};
}

bool Skullreapers::configure(int numModels, bool iconBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    if (m_iconBearer)
    {
        m_bravery += 1;
    }

    Model skullseeker(BASESIZE, WOUNDS);
    skullseeker.addMeleeWeapon(&m_viciousMutation);
    skullseeker.addMeleeWeapon(&m_blades);
    addModel(skullseeker);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_blades);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Skullreapers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_blades);
    visitor(&m_viciousMutation);
}

Unit *Skullreapers::Create(const ParameterList &parameters)
{
    auto unit = new Skullreapers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, true);

    bool ok = unit->configure(numModels, iconBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skullreapers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skullreapers", factoryMethod);
    }
}

Rerolls Skullreapers::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Trial of skulls.
    if (target->remainingModels() >= 5)
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

Wounds Skullreapers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Daemonforged Weapons
    if ((weapon->name() == m_blades.name()) && (hitRoll == 6))
    {
        Wounds wounds = {weapon->damage(), 1};
        return wounds;
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace Khorne
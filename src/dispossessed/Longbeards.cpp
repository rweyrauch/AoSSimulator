/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Longbeards.h>
#include <UnitFactory.h>

namespace Dispossessed
{
static FactoryMethod factoryMethod = {
    Longbeards::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Longbeards::MIN_UNIT_SIZE}, Longbeards::MIN_UNIT_SIZE,
            Longbeards::MAX_UNIT_SIZE, Longbeards::MIN_UNIT_SIZE
        },
        {
            ParamType::Integer, "weapons", {.m_intValue = Longbeards::AncestralAxesOrHammers}, Longbeards::AncestralAxesOrHammers,
            Longbeards::AncestralGreatAxe, 1
        },
        {ParamType::Boolean, "gromrilShields", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "standardBearer", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "musician", {.m_boolValue = false}, false, false, false}
    }
};

bool Longbeards::s_registered = false;

Longbeards::Longbeards() :
    Unit("Longbeards", 4, WOUNDS, 7, 4, false),
    m_ancestralAxeHammer(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer", 1, 1, 3, 4, 0, 1),
    m_ancestralAxeHammerOldGuard(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer (Old Guard)", 1, 2, 3, 4, 0, 1),
    m_ancestralGreatAxe(Weapon::Type::Melee, "Ancestral Great Axe", 1, 1, 4, 3, -1, 1),
    m_ancestralGreatAxeOldGuard(Weapon::Type::Melee, "Ancestral Great Axe (Old Guard)", 1, 2, 4, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, LONGBEARDS};
}

bool Longbeards::configure(int numModels, WeaponOptions weapons, bool gromrilShields, bool standardBearer, bool musician)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_musician = musician;
    m_gromrilShields = gromrilShields;

    Model oldguard(BASESIZE, WOUNDS);
    if (weapons == AncestralAxesOrHammers)
    {
        oldguard.addMeleeWeapon(&m_ancestralAxeHammerOldGuard);
    }
    else if (weapons == AncestralGreatAxe)
    {
        oldguard.addMeleeWeapon(&m_ancestralGreatAxeOldGuard);
    }
    addModel(oldguard);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == AncestralAxesOrHammers)
        {
            model.addMeleeWeapon(&m_ancestralAxeHammer);
        }
        else if (weapons == AncestralGreatAxe)
        {
            model.addMeleeWeapon(&m_ancestralGreatAxe);
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

void Longbeards::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ancestralAxeHammer);
    visitor(&m_ancestralAxeHammerOldGuard);
    visitor(&m_ancestralGreatAxe);
    visitor(&m_ancestralGreatAxeOldGuard);
}

Unit *Longbeards::Create(const ParameterList &parameters)
{
    auto unit = new Longbeards();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOptions)GetIntParam("weapons", parameters, AncestralAxesOrHammers);
    bool gromrilShields = GetBoolParam("gromrilShields", parameters, false);
    bool standardBearer = GetBoolParam("standardBearer", parameters, false);
    bool musician = GetBoolParam("musician", parameters, false);

    bool ok = unit->configure(numModels, weapons, gromrilShields, standardBearer, musician);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Longbeards::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Longbeards", factoryMethod);
    }
}

Rerolls Longbeards::toSaveRerolls(const Weapon *weapon) const
{
    // Gromril Shields
    if (!m_ran && !m_charged)
    {
        if (!weapon->isMissile())
            return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

} // namespace Dispossessed

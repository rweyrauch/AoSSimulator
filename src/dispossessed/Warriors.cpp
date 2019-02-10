/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Warriors.h>
#include <UnitFactory.h>

namespace Dispossessed
{
static FactoryMethod factoryMethod = {
    Warriors::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Warriors::MIN_UNIT_SIZE}, Warriors::MIN_UNIT_SIZE,
            Warriors::MAX_UNIT_SIZE, Warriors::MIN_UNIT_SIZE
        },
        {
            ParamType::Integer, "weapons", {.m_intValue = Warriors::DuardinAxeOrHammer}, Warriors::DuardinAxeOrHammer,
            Warriors::DoubleHandedDuardinAxe, 1
        },
        {ParamType::Boolean, "duardinShields", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "standardBearer", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = false}, false, false, false}
    }
};

bool Warriors::s_registered = false;

Warriors::Warriors() :
    Unit("Warriors", 4, WOUNDS, 6, 5, false),
    m_duardinAxeOrHammer(Weapon::Type::Melee, "Duardin Axe or Duardin Hammer", 1, 1, 3, 4, 0, 1),
    m_duardinAxeOrHammerVeteran(Weapon::Type::Melee, "Duardin Axe or Duardin Hammer (Veteran)", 1, 2, 3, 4, 0, 1),
    m_doubleHandedAxe(Weapon::Type::Melee, "Double-handed Duardin Axe", 1, 1, 4, 3, -1, 1),
    m_doubleHandedAxeVeteran(Weapon::Type::Melee, "Double-handed Duardin Axe (Veteran)", 1, 2, 4, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, WARRIORS};
}

bool Warriors::configure(int numModels, WeaponOptions weapons, bool duardinShields, StandardOptions standard, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standard = standard;
    m_hornblowers = hornblowers;
    m_duardinShields = duardinShields;

    Model veteran(BASESIZE, WOUNDS);
    if (weapons == DuardinAxeOrHammer)
    {
        veteran.addMeleeWeapon(&m_duardinAxeOrHammerVeteran);
    }
    else if (weapons == DoubleHandedDuardinAxe)
    {
        veteran.addMeleeWeapon(&m_doubleHandedAxeVeteran);
    }
    addModel(veteran);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == DuardinAxeOrHammer)
        {
            model.addMeleeWeapon(&m_duardinAxeOrHammer);
        }
        else if (weapons == DoubleHandedDuardinAxe)
        {
            model.addMeleeWeapon(&m_doubleHandedAxe);
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

void Warriors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_duardinAxeOrHammer);
    visitor(&m_duardinAxeOrHammerVeteran);
    visitor(&m_doubleHandedAxe);
    visitor(&m_doubleHandedAxeVeteran);
}

Unit *Warriors::Create(const ParameterList &parameters)
{
    auto unit = new Warriors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOptions)GetIntParam("weapons", parameters, DuardinAxeOrHammer);
    bool duardinShields = GetBoolParam("duardinShields", parameters, false);
    auto standard = (StandardOptions)GetIntParam("standard", parameters, None);
    bool hornblower = GetBoolParam("hornblower", parameters, false);

    bool ok = unit->configure(numModels, weapons, duardinShields, standard, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Warriors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Warriors", factoryMethod);
    }
}

Rerolls Warriors::toSaveRerolls(const Weapon *weapon) const
{
    // Duardin Shields
    if (!m_ran && !m_charged)
    {
        if (!weapon->isMissile())
            return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

Rerolls Warriors::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Resolute in Defence
    if (m_opponentsCombat)
    {
        if (remainingModels() >= 20)
            return RerollFailed;
        else
            return RerollOnes;
    }
    return Unit::toWoundRerolls(weapon, target);
}

void Warriors::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    m_opponentsCombat = (player != m_owningPlayer);
}

} // namespace Dispossessed

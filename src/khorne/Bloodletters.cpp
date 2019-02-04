/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/Bloodletters.h>
#include <UnitFactory.h>
#include <iostream>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Bloodletters::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Bloodletters::MIN_UNIT_SIZE}, Bloodletters::MIN_UNIT_SIZE,
         Bloodletters::MAX_UNIT_SIZE, Bloodletters::MIN_UNIT_SIZE},
        {ParamType::Boolean, "iconBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "standardBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = true}, false, false}
    }
};

bool Bloodletters::s_registered = false;

Bloodletters::Bloodletters() :
    Unit("Bloodletters", 5, WOUNDS, 10, 5, false),
    m_hellblade(Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1),
    m_hellbladeReaper(Weapon::Type::Melee, "Hellblade (Reaper)", 1, 2, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTERS};
}


bool Bloodletters::configure(int numModels, bool iconBearer, bool standardBearer, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_standarBearer = standardBearer;
    m_hornblower = hornblowers;

    // Add the Hellreaper
    Model reaperModel(BASESIZE, WOUNDS);
    reaperModel.addMeleeWeapon(&m_hellbladeReaper);
    addModel(reaperModel);

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_hellblade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

int Bloodletters::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits, const WoundingHits& wounds)
{
    // Decapitating Blow
    if (hits.rolls.numUnmodified6s() > 0)
    {
        return 1;
    }
    return Unit::generateMortalWounds(weapon, unit, hits, wounds);
}

Unit *Bloodletters::Create(const ParameterList &parameters)
{
    auto unit = new Bloodletters();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("iconBearer", parameters, false);
    bool standardBearer = GetBoolParam("standardBearer", parameters, false);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, standardBearer, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bloodletters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodletters", factoryMethod);
    }
}

int Bloodletters::extraAttacks(const Weapon *weapon) const
{
    // Murderous Tide
    int attacks = Unit::extraAttacks(weapon);
    if (remainingModels() >= 20)
        attacks += 1;
    return attacks;
}

void Bloodletters::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_hellblade);
    visitor(&m_hellbladeReaper);
}

} // namespace Khorne
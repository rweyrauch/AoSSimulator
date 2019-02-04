/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Seekers.h>
#include <Weapon.h>
#include <UnitFactory.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    Seekers::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Seekers::MIN_UNIT_SIZE}, Seekers::MIN_UNIT_SIZE,
         Seekers::MAX_UNIT_SIZE, Seekers::MIN_UNIT_SIZE},
        {ParamType::Boolean, "iconBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "standardBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblower", {.m_boolValue = true}, false, false},
    }
};

bool Seekers::s_registered = false;

Seekers::Seekers() :
    Unit("Seekers", 14, WOUNDS, 10, 5, false),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 4, 4, -1, 1),
    m_piercingClawsHeartseeker(Weapon::Type::Melee, "Piercing Claws (Heartseeker)", 1, 3, 4, 4, -1, 1),
    m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 4, 4, 0, 1)
{
    m_keywords = { CHAOS, DAEMON, DAEMONETTES, SLAANESH, SEEKERS };

    // Quicksilver Speed
    m_runAndCharge = true;
}

bool Seekers::configure(int numModels, bool iconBearer, bool standardBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Heartseeker
    Model reaperModel(BASESIZE, WOUNDS);
    reaperModel.addMeleeWeapon(&m_piercingClawsHeartseeker);
    addModel(reaperModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_piercingClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void Seekers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_piercingClaws);
    visitor(&m_piercingClawsHeartseeker);
    visitor(&m_poisonedTongue);
}

Unit *Seekers::Create(const ParameterList &parameters)
{
    auto unit = new Seekers();
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

void Seekers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Seekers", factoryMethod);
    }
}

int Seekers::runModifier() const
{
    // Quicksilver Speed
    Dice dice;
    return dice.rollD6();
}


Rerolls Seekers::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    if (m_standardBearer) return RerollOnes;
    return Unit::toHitRerolls(weapon, unit);
}

Hits Seekers::applyHitModifiers(const Weapon *weapon, const Unit *unit, const Hits &hits) const
{
    Hits modifiedHits = hits;

    if (weapon->name() == m_piercingClaws.name())
    {
        // Sadistic Killers
        if (remainingModels() >= 20)
        {
            modifiedHits.numHits += hits.rolls.rollsGE(5);
        }
        else
        {
            modifiedHits.numHits += hits.rolls.numUnmodified6s();
        }
    }

    // Modifiers accumulate
    return Unit::applyHitModifiers(weapon, unit, modifiedHits);
}

} // namespace Slaanesh
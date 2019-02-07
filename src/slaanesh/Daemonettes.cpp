/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Daemonettes.h>
#include <Weapon.h>
#include <UnitFactory.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    Daemonettes::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Daemonettes::MIN_UNIT_SIZE}, Daemonettes::MIN_UNIT_SIZE,
            Daemonettes::MAX_UNIT_SIZE, Daemonettes::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "iconBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "standardBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblower", {.m_boolValue = true}, false, false},
    }
};

bool Daemonettes::s_registered = false;

Daemonettes::Daemonettes() :
    Unit("Daemonettes", 6, WOUNDS, 10, 5, false),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 4, 4, -1, 1),
    m_piercingClawsAlluress(Weapon::Type::Melee, "Piercing Claws (Alluress)", 1, 3, 4, 4, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, SLAANESH, DAEMONETTES};

    // Lithe and Swift
    m_runAndCharge = true;
}

bool Daemonettes::configure(int numModels, bool iconBearer, bool standardBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Alluress
    Model reaperModel(BASESIZE, WOUNDS);
    reaperModel.addMeleeWeapon(&m_piercingClawsAlluress);
    addModel(reaperModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_piercingClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Daemonettes::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_piercingClaws);
    visitor(&m_piercingClawsAlluress);
}

Unit *Daemonettes::Create(const ParameterList &parameters)
{
    auto unit = new Daemonettes();
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

void Daemonettes::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Daemonettes", factoryMethod);
    }
}

Rerolls Daemonettes::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    if (m_standardBearer)
    { return RerollOnes; }
    return Unit::toHitRerolls(weapon, unit);
}

int Daemonettes::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    if (weapon->name() == m_piercingClaws.name())
    {
        // Sadistic Killers
        if (remainingModels() >= 20 && (unmodifiedHitRoll >= 5))
        {
            return 2;
        }
        else if (unmodifiedHitRoll == 6)
        {
            return 2;
        }
    }

    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

} // namespace Slaanesh
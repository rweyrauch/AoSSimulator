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
            ParamType::Integer, "Models", Daemonettes::MIN_UNIT_SIZE, Daemonettes::MIN_UNIT_SIZE,
            Daemonettes::MAX_UNIT_SIZE, Daemonettes::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    SLAANESH
};

bool Daemonettes::s_registered = false;

Daemonettes::Daemonettes() :
    Unit("Daemonettes", 6, WOUNDS, 10, 5, false),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 4, 4, -1, 1),
    m_piercingClawsAlluress(Weapon::Type::Melee, "Piercing Claws (Alluress)", 1, 3, 4, 4, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, SLAANESH, HEDONITE, DAEMONETTES};

    // Lithe and Swift
    m_runAndCharge = true;
}

bool Daemonettes::configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_bannerBearer = bannerBearer;
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
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, bannerBearer, hornblowers);
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

void Daemonettes::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const
{
    Unit::computeBattleshockEffect(roll, numFled, numAdded);
    if (m_iconBearer)
    {
        // Icon Bearer
        if (roll == 1)
        {
            Dice dice;
            numAdded = dice.rollD6();
        }
    }
}

void Daemonettes::restoreModels(int numModels)
{
    // Icon Bearer
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_piercingClaws);
    for (auto i = 0; i < numModels; i++)
    {
        addModel(model);
    }
}

Rerolls Daemonettes::chargeRerolls() const
{
    if (m_bannerBearer)
    {
        return RerollFailed;
    }
    return Unit::chargeRerolls();
}

} // namespace Slaanesh
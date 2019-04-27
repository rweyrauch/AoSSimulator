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
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    Seekers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Seekers::MIN_UNIT_SIZE, Seekers::MIN_UNIT_SIZE,
            Seekers::MAX_UNIT_SIZE, Seekers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    SLAANESH
};

bool Seekers::s_registered = false;

Seekers::Seekers() :
    Unit("Seekers", 14, WOUNDS, 10, 5, false),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 3, 4, -1, 1),
    m_piercingClawsHeartseeker(Weapon::Type::Melee, "Piercing Claws (Heartseeker)", 1, 3, 3, 4, -1, 1),
    m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTES, SLAANESH, HEDONITE, SEEKERS};

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
    m_bannerBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Heartseeker
    Model reaperModel(BASESIZE, WOUNDS);
    reaperModel.addMeleeWeapon(&m_piercingClawsHeartseeker);
    reaperModel.addMeleeWeapon(&m_poisonedTongue);
    addModel(reaperModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_piercingClaws);
        model.addMeleeWeapon(&m_poisonedTongue);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

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
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

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

void Seekers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const
{
    Unit::computeBattleshockEffect(roll, numFled, numAdded);
    if (m_iconBearer)
    {
        // Icon Bearer
        if (roll == 1)
        {
            Dice dice;
            numAdded = dice.rollD3();
        }
    }
}

void Seekers::restoreModels(int numModels)
{
    // Icon Bearer
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_piercingClaws);
    model.addMeleeWeapon(&m_poisonedTongue);
    for (auto i = 0; i < numModels; i++)
    {
        addModel(model);
    }
}

Rerolls Seekers::chargeRerolls() const
{
    if (m_bannerBearer)
    {
        return RerollFailed;
    }
    return Unit::chargeRerolls();
}

int Seekers::runModifier() const
{
    // Quicksilver Speed
    Dice dice;
    return dice.rollD6();
}

} // namespace Slaanesh
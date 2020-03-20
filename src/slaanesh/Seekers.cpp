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
bool Seekers::s_registered = false;

Seekers::Seekers() :
    SlaaneshBase("Seekers", 14, WOUNDS, 10, 5, false),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 3, 4, -1, 1),
    m_piercingClawsHeartseeker(Weapon::Type::Melee, "Piercing Claws", 1, 3, 3, 4, -1, 1),
    m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTES, SLAANESH, HEDONITE, SEEKERS};
    m_weapons = {&m_piercingClaws, &m_piercingClawsHeartseeker, &m_poisonedTongue};

    // Quicksilver Speed
    m_runAndCharge = true;

    s_globalBattleshockReroll.connect(this, &Seekers::hornblowerBattleshockReroll, &m_hornblowerSlot);
}

Seekers::~Seekers()
{
    m_hornblowerSlot.disconnect();
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
    auto reaperModel = new Model(BASESIZE, wounds());
    reaperModel->addMeleeWeapon(&m_piercingClawsHeartseeker);
    reaperModel->addMeleeWeapon(&m_poisonedTongue);
    addModel(reaperModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongue);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
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
        static FactoryMethod factoryMethod = {
            Create,
            SlaaneshBase::ValueToString,
            SlaaneshBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
            },
            CHAOS,
            { SLAANESH }
        };
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
            numAdded = Dice::rollD3();
        }
    }
}

void Seekers::restoreModels(int numModels)
{
    // Icon Bearer
    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongue);
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
    return Dice::rollD6();
}

int Seekers::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

Rerolls Seekers::hornblowerBattleshockReroll(const Unit *unit)
{
    if (!isFriendly(unit) && m_hornblower && (distanceTo(unit) <= 6.0f)) return RerollOnes;

    return NoRerolls;
}

} // namespace Slaanesh
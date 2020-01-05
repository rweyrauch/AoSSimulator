/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzaangorEnlightened.h>
#include <UnitFactory.h>

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    TzaangorEnlightened::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", TzaangorEnlightened::MIN_UNIT_SIZE, TzaangorEnlightened::MIN_UNIT_SIZE,
         TzaangorEnlightened::MAX_UNIT_SIZE, TzaangorEnlightened::MIN_UNIT_SIZE},
        {ParamType::Boolean, "Disks Of Tzeentch", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    TZEENTCH
};

bool TzaangorEnlightened::s_registered = false;

TzaangorEnlightened::TzaangorEnlightened() :
    Unit("Tzaangor Enlightened", 6, WOUNDS, 6, 5, false),
    m_tzeentchianSpear(Weapon::Type::Melee, "Tzeentchian Spear", 2, 3, 4, 3, -1, 2),
    m_tzeentchianSpearAviarch(Weapon::Type::Melee, "Tzeentchian Spear", 2, 4, 4, 3, -1, 2),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
    m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, TZAANGOR_ENLIGHTENED};
    m_weapons = {&m_tzeentchianSpear, &m_tzeentchianSpearAviarch, &m_viciousBeak, &m_teethAndHorns};
}

bool TzaangorEnlightened::configure(int numModels, bool disksOfTzeentch)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    if (disksOfTzeentch)
    {
        m_wounds = WOUNDS_WITH_DISK;
        m_move = 16;
        m_fly = true;
        addKeyword(DAEMON);
    }

    auto aviarch = new Model(BASESIZE, disksOfTzeentch ? WOUNDS_WITH_DISK : WOUNDS);
    aviarch->addMeleeWeapon(&m_tzeentchianSpearAviarch);
    aviarch->addMeleeWeapon(&m_viciousBeak);
    if (disksOfTzeentch)
        aviarch->addMeleeWeapon(&m_teethAndHorns);
    addModel(aviarch);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, disksOfTzeentch ? WOUNDS_WITH_DISK : WOUNDS);
        model->addMeleeWeapon(&m_tzeentchianSpear);
        model->addMeleeWeapon(&m_viciousBeak);
        if (disksOfTzeentch)
            model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);
    }

    if (disksOfTzeentch)
    {
        m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK_WITH_DISK;
        if (numModels == MAX_UNIT_SIZE)
        {
            m_points = POINTS_MAX_UNIT_SIZE_WITH_DISK;
        }
    }
    else
    {
        m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE)
        {
            m_points = POINTS_MAX_UNIT_SIZE;
        }
    }

    return true;
}

Unit *TzaangorEnlightened::Create(const ParameterList &parameters)
{
    auto *unit = new TzaangorEnlightened();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool disksOfTzeentch = GetBoolParam("Disks Of Tzeentch", parameters, false);

    bool ok = unit->configure(numModels, disksOfTzeentch);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TzaangorEnlightened::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tzaangor Enlightened", factoryMethod);
    }
}

} //namespace Tzeentch
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/DoomfireWarlocks.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>

namespace DaughtersOfKhaine
{
bool DoomfireWarlocks::s_registered = false;

DoomfireWarlocks::DoomfireWarlocks() :
    DaughterOfKhaine("Doomfire Warlocks", 14, WOUNDS, 6, 5, false),
    m_crossBow(Weapon::Type::Missile, "Doomfire Crossbow", 10, 2, 4, 4, 0, 1),
    m_scimitar(Weapon::Type::Melee, "Cursed Scimitar", 1, 2, 4, 4, -1, 1),
    m_crossBowMaster(Weapon::Type::Missile, "Doomfire Crossbow", 10, 2, 3, 4, 0, 1),
    m_scimitarMaster(Weapon::Type::Melee, "Cursed Scimitar", 1, 2, 3, 4, -1, 1),
    m_steedsBite(Weapon::Type::Melee, "Dark Steed's Vicious Bite", 1, 2, 4, 5, 0, 1)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, WIZARD, DOOMFIRE_WARLOCKS};
    m_weapons = {&m_crossBow, &m_scimitar, &m_crossBowMaster, &m_scimitarMaster, &m_steedsBite};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool DoomfireWarlocks::configure(int numModels, bool crossbows)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto master = new Model(BASESIZE, WOUNDS);
    master->addMeleeWeapon(&m_scimitarMaster);
    master->addMeleeWeapon(&m_steedsBite);
    if (crossbows)
    {
        master->addMissileWeapon(&m_crossBowMaster);
    }
    addModel(master);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_scimitar);
        model->addMeleeWeapon(&m_steedsBite);
        if (crossbows)
        {
            model->addMissileWeapon(&m_crossBow);
        }
        addModel(model);
    }

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = ComputePoints(numModels);

    return true;
}

Unit *DoomfireWarlocks::Create(const ParameterList &parameters)
{
    auto unit = new DoomfireWarlocks();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool crossbows = GetBoolParam("Crossbows", parameters, false);

    bool ok = unit->configure(numModels, crossbows);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DoomfireWarlocks::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            DoomfireWarlocks::Create,
            nullptr,
            nullptr,
            DoomfireWarlocks::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", DoomfireWarlocks::MIN_UNIT_SIZE, DoomfireWarlocks::MIN_UNIT_SIZE,
                    DoomfireWarlocks::MAX_UNIT_SIZE, DoomfireWarlocks::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Crossbows", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
            },
            ORDER,
            { DAUGHTERS_OF_KHAINE }
        };
        s_registered = UnitFactory::Register("Doomfire Warlocks", *factoryMethod);
    }
}

int DoomfireWarlocks::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace DaughtersOfKhaine
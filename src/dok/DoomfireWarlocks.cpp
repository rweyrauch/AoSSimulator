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

namespace DaughtersOfKhaine
{

static FactoryMethod factoryMethod = {
    DoomfireWarlocks::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", DoomfireWarlocks::MIN_UNIT_SIZE, DoomfireWarlocks::MIN_UNIT_SIZE,
            DoomfireWarlocks::MAX_UNIT_SIZE, DoomfireWarlocks::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Crossbows", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
    },
    ORDER,
    DAUGHTERS_OF_KHAINE
};

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

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

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
        s_registered = UnitFactory::Register("Doomfire Warlocks", factoryMethod);
    }
}

void DoomfireWarlocks::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_crossBow);
    visitor(m_crossBowMaster);
    visitor(m_scimitar);
    visitor(m_scimitarMaster);
    visitor(m_steedsBite);
}

} // namespace DaughtersOfKhaine
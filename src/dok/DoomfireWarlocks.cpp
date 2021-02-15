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
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize/g_minUnitSize)*g_pointsPerBlock;

    bool DoomfireWarlocks::s_registered = false;

    DoomfireWarlocks::DoomfireWarlocks() :
            DaughterOfKhaine("Doomfire Warlocks", 14, g_wounds, 6, 5, false),
            m_crossBow(Weapon::Type::Missile, "Doomfire Crossbow", 10, 2, 4, 4, 0, 1),
            m_scimitar(Weapon::Type::Melee, "Cursed Scimitar", 1, 2, 4, 4, -1, 1),
            m_crossBowMaster(Weapon::Type::Missile, "Doomfire Crossbow", 10, 2, 3, 4, 0, 1),
            m_scimitarMaster(Weapon::Type::Melee, "Cursed Scimitar", 1, 2, 3, 4, -1, 1),
            m_steedsBite(Weapon::Type::Melee, "Dark Steed's Vicious Bite", 1, 2, 4, 5, 0, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, WIZARD, DOOMFIRE_WARLOCKS};
        m_weapons = {&m_crossBow, &m_scimitar, &m_crossBowMaster, &m_scimitarMaster, &m_steedsBite};
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool DoomfireWarlocks::configure(int numModels, bool crossbows) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto master = new Model(g_basesize, wounds());
        master->addMeleeWeapon(&m_scimitarMaster);
        master->addMeleeWeapon(&m_steedsBite);
        if (crossbows) {
            master->addMissileWeapon(&m_crossBowMaster);
        }
        addModel(master);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_scimitar);
            model->addMeleeWeapon(&m_steedsBite);
            if (crossbows) {
                model->addMissileWeapon(&m_crossBow);
            }
            addModel(model);
        }

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *DoomfireWarlocks::Create(const ParameterList &parameters) {
        auto unit = new DoomfireWarlocks();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool crossbows = GetBoolParam("Crossbows", parameters, false);

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels, crossbows);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DoomfireWarlocks::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Crossbows"),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Doomfire Warlocks", factoryMethod);
        }
    }

    int DoomfireWarlocks::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int DoomfireWarlocks::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();

        // Doomfire Coven
        if (remainingModels() >= 10) mod++;

        return mod;
    }

    int DoomfireWarlocks::castingModifier() const {
        auto mod = Unit::castingModifier();

        // Doomfire Coven
        if (remainingModels() >= 10) mod++;

        return mod;
    }

} // namespace DaughtersOfKhaine
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/BloodSisters.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize/g_minUnitSize)*g_pointsPerBlock;

    bool BloodSisters::s_registered = false;

    BloodSisters::BloodSisters() :
            DaughterOfKhaine("Blood Sisters", 8, g_wounds, 8, 5, false),
            m_heartshardGlaive(Weapon::Type::Melee, "Heartshard Glaive", 2, 3, 3, 3, -1, 1),
            m_heartshardGlaiveGorgai(Weapon::Type::Melee, "Heartshard Glaive", 2, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, MELUSAI, BLOOD_SISTERS};
        m_weapons = {&m_heartshardGlaive, &m_heartshardGlaiveGorgai};
    }

    bool BloodSisters::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto gorgai = new Model(g_basesize, wounds());
        gorgai->addMeleeWeapon(&m_heartshardGlaiveGorgai);
        addModel(gorgai);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_heartshardGlaive);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BloodSisters::Create(const ParameterList &parameters) {
        auto unit = new BloodSisters();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BloodSisters::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Blood Sisters", factoryMethod);
        }
    }

    int BloodSisters::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds BloodSisters::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        // Turned to Crystal
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
            if (!units.empty()) {
                auto unit = units.front();
                Dice::RollResult result;
                Dice::RollD6(unit->remainingModels(), result);
                wounds.mortal += result.rollsGE(3);
            }
        }
        return wounds;
    }

} // namespace DaughtersOfKhaine
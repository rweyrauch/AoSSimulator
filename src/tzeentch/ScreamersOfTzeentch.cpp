/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/ScreamersOfTzeentch.h>
#include <UnitFactory.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 32;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 320;


    bool ScreamersOfTzeentch::s_registered = false;

    ScreamersOfTzeentch::ScreamersOfTzeentch() :
            TzeentchBase("Screamers of Tzeentch", 16, g_wounds, 10, 5, true),
            m_bite(Weapon::Type::Melee, "Lamprey Bite", 1, 3, 4, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, SCREAMERS};
        m_weapons = {&m_bite};
    }

    bool ScreamersOfTzeentch::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *ScreamersOfTzeentch::Create(const ParameterList &parameters) {
        auto unit = new ScreamersOfTzeentch();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ScreamersOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ScreamersOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    ScreamersOfTzeentch::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Screamers of Tzeentch", factoryMethod);
        }
    }

    Wounds
    ScreamersOfTzeentch::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sky-sharks
        if (target->hasKeyword(MONSTER)) {
            return {Dice::RollD3(), 0};
        }
        return TzeentchBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ScreamersOfTzeentch::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch
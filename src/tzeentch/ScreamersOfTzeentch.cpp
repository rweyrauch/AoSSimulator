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

    ScreamersOfTzeentch::ScreamersOfTzeentch(ChangeCoven coven, int numModels, int points) :
            TzeentchBase(coven, "Screamers of Tzeentch", 16, g_wounds, 10, 5, true, points),
            m_bite(Weapon::Type::Melee, "Lamprey Bite", 1, 3, 4, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, SCREAMERS};
        m_weapons = {&m_bite};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }
    }

    Unit *ScreamersOfTzeentch::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        auto numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new ScreamersOfTzeentch(coven, numModels, ComputePoints(parameters));
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
    ScreamersOfTzeentch::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sky-sharks
        if (target->hasKeyword(MONSTER)) {
            return {Dice::RollD3(), 0};
        }
        return TzeentchBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int ScreamersOfTzeentch::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Plaguebearers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 300;

    bool Plaguebearers::s_registered = false;

    Plaguebearers::Plaguebearers() :
            NurgleBase("Plaguebearers", 4, g_wounds, 10, 5, false),
            m_plaguesword(Weapon::Type::Melee, "Plaguesword", 1, 1, 4, 3, 0, 1),
            m_plagueswordPlagueRidden(Weapon::Type::Melee, "Plaguesword", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, PLAGUEBEARERS};
        m_weapons = {&m_plaguesword, &m_plagueswordPlagueRidden};
        m_battleFieldRole = Role::Battleline;
    }

    bool Plaguebearers::configure(int numModels, bool iconBearer, bool pipers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Add the Plagueridden
        auto leader = new Model(g_basesize, wounds());
        leader->addMeleeWeapon(&m_plagueswordPlagueRidden);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_plaguesword);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            }
            else if (pipers) {
                model->setName(Model::Piper);
                pipers = false;
            }

            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Plaguebearers::Create(const ParameterList &parameters) {
        auto unit = new Plaguebearers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool pipers = GetBoolParam("Pipers", parameters, false);

        auto legion = (PlagueLegion)GetEnumParam("Plague Legion", parameters, (int)PlagueLegion::None);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, iconBearer, pipers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Plaguebearers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Plaguebearers::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    Plaguebearers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Piper"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Plaguebearers", factoryMethod);
        }
    }

    Wounds Plaguebearers::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    int Plaguebearers::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        // Cloud of Flies
        int modifier = Unit::targetHitModifier(weapon, attacker);
        if (remainingModels() >= 20) {
            modifier -= 2;
        } else {
            modifier -= 1;
        }
        return modifier;
    }

    int Plaguebearers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls Plaguebearers::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Locus of Fecundity
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 7.0);
        for (auto unit : units) {
            if (unit->hasKeyword(DAEMON) && unit->hasKeyword(NURGLE) && unit->hasKeyword(HERO)) return Rerolls::Ones;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

} // namespace Nurgle


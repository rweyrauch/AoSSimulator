/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/BeastsOfNurgle.h>
#include "NurglePrivate.h"
#include <UnitFactory.h>
#include <Board.h>

namespace Nurgle {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 6;

    bool BeastsOfNurgle::s_registered = false;

    BeastsOfNurgle::BeastsOfNurgle() :
            NurgleBase("Beasts of Nurgle", 5, g_wounds, 10, 5, false),
            m_clawsAndTentacles(Weapon::Type::Melee, "Claws and Tentacles", 1, RAND_D6, 4, 3, 0, 1),
            m_slobberingTongue(Weapon::Type::Melee, "Slobbering Tongue", 2, 1, 3, 3, 0, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, NURGLE, BEASTS_OF_NURGLE};
        m_weapons = {&m_clawsAndTentacles, &m_slobberingTongue};
    }

    bool BeastsOfNurgle::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_clawsAndTentacles);
            model->addMeleeWeapon(&m_slobberingTongue);
            addModel(model);
        }

        // Attention Seekers
        m_runAndCharge = true;
        m_retreatAndCharge = true;

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BeastsOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new BeastsOfNurgle();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BeastsOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BeastsOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    BeastsOfNurgle::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Beasts of Nurgle", factoryMethod);
        }
    }

    Wounds BeastsOfNurgle::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    Wounds BeastsOfNurgle::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Locus of Virulence
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 7.0);
        for (auto ip : units) {
            if (ip->hasKeyword(DAEMON) && ip->hasKeyword(NURGLE) && ip->hasKeyword(HERO)) {
                return {weapon->damage() + 1, 0};
            }
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BeastsOfNurgle::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nurgle


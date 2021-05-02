/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/TuskgorChariots.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 105; // x70 ovals
    static const int g_wounds = 6;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 4;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 200;

    bool TuskgorChariots::s_registered = false;

    TuskgorChariots::TuskgorChariots(Greatfray fray, int numModels, int points) :
            BeastsOfChaosBase("Tuskgor Chariots", 10, g_wounds, 6, 4, false, points),
            m_despoilerAxe(Weapon::Type::Melee, "Despoiler Axe", 1, 2, 4, 3, -1, 1),
            m_gnarledSpear(Weapon::Type::Melee, "Gnarled Spear", 2, 1, 4, 4, 0, 1),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 4, 4, 3, 0, 1) {
        m_keywords = {CHAOS, GORS, BEASTS_OF_CHAOS, BRAYHERD, TUSKGOR_CHARIOTS};
        m_weapons = {&m_despoilerAxe, &m_gnarledSpear, &m_tusksAndHooves};
        m_hasMount = true;
        m_tusksAndHooves.setMount(true);

        setGreatfray(fray);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_despoilerAxe);
            model->addMeleeWeapon(&m_gnarledSpear);
            model->addMeleeWeapon(&m_tusksAndHooves);
            addModel(model);
        }
    }

    Unit *TuskgorChariots::Create(const ParameterList &parameters) {
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        return new TuskgorChariots(fray, numModels, ComputePoints(parameters));
    }

    void TuskgorChariots::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Tuskgor Chariots", factoryMethod);
        }
    }

    std::string TuskgorChariots::ValueToString(const Parameter &parameter) {
        return BeastsOfChaosBase::ValueToString(parameter);
    }

    int TuskgorChariots::EnumStringToInt(const std::string &enumString) {
        return BeastsOfChaosBase::EnumStringToInt(enumString);
    }

    int TuskgorChariots::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        // Despoilers
        int modifier = BeastsOfChaosBase::toHitModifier(weapon, unit);
        if (unit->remainingModels() >= 10) {
            modifier += 1;
        }
        return modifier;
    }

    Rerolls TuskgorChariots::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Despoilers
        if (unit->hasKeyword(ORDER)) {
            return Rerolls::Ones;
        }
        return BeastsOfChaosBase::toHitRerolls(weapon, unit);
    }

    int TuskgorChariots::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = BeastsOfChaosBase::extraAttacks(attackingModel, weapon, target);
        // Tuskgor Charge
        if (m_charged) {
            extra++;
        }
        return extra;
    }

    Rerolls TuskgorChariots::chargeRerolls() const {
        // Tuskgor Charge
        return Rerolls::Failed;
    }

    int TuskgorChariots::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos
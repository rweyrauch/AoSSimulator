/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Bestigors.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 300;

    bool Bestigors::s_registered = false;

    Bestigors::Bestigors(Greatfray fray, int numModels, bool brayhorn, bool bannerBearer, int points) :
            BeastsOfChaosBase("Bestigors", 6, g_wounds, 6, 4, false, points) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, BESTIGORS};
        m_weapons = {&m_despoilerAxe, &m_despoilerAxeGougeHorn};

        setGreatfray(fray);

        m_runAndCharge = brayhorn;

        auto gougehorn = new Model(g_basesize, wounds());
        gougehorn->addMeleeWeapon(&m_despoilerAxeGougeHorn);
        addModel(gougehorn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_despoilerAxe);
            addModel(model);
            if (bannerBearer) {
                model->setName(Model::BannerBearer);
                bannerBearer = false;
            } else if (brayhorn) {
                model->setName("Brayhorn");
                brayhorn = false;
            }
        }
    }

    bool Bestigors::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *Bestigors::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            int numModels = GetIntParam("Models", parameters, g_minUnitSize);
            bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
            bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
            auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
            return new Bestigors(fray, numModels, brayhorn, bannerBearer, ComputePoints(parameters));
        }
        return nullptr;
    }

    void Bestigors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Brayhorn"),
                            BoolParameter("Banner Bearer"),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Bestigors", factoryMethod);
        }
    }

    int Bestigors::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        // Despoilers
        int modifier = BeastsOfChaosBase::toHitModifier(weapon, unit);
        if (unit->remainingModels() >= 10) {
            modifier += 1;
        }
        return modifier;
    }

    Rerolls Bestigors::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Despoilers
        if (unit->hasKeyword(ORDER)) {
            return Rerolls::Ones;
        }
        return BeastsOfChaosBase::toHitRerolls(weapon, unit);
    }

    int Bestigors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Beastial Charge
        int attacks = BeastsOfChaosBase::extraAttacks(attackingModel, weapon, target);
        if (m_charged) {
            attacks += 1;
        }
        return attacks;
    }

    int Bestigors::runModifier() const {
        int modifier = BeastsOfChaosBase::runModifier();
        if (isNamedModelAlive(Model::BannerBearer)) {
            modifier += 1;
        }
        return modifier;
    }

    int Bestigors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }

        return points;
    }

} // namespace BeastsOfChaos
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Centigors.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 320;

    bool Centigors::s_registered = false;

    Centigors::Centigors() :
            BeastsOfChaosBase("Centigors", 14, g_wounds, 5, 5, false) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, CENTIGORS};
        m_weapons = {&m_centigorSpear, &m_centigorSpearGorehoof, &m_clawedForelimbs};
    }

    Centigors::Centigors(Greatfray fray, int numModels, bool brayhorn, bool bannerBearer) :
            Centigors() {

        setGreatfray(fray);

        m_runAndCharge = brayhorn;

        auto gorehoof = new Model(g_basesize, wounds());
        gorehoof->addMeleeWeapon(&m_centigorSpearGorehoof);
        gorehoof->addMeleeWeapon(&m_clawedForelimbs);
        addModel(gorehoof);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_centigorSpear);
            model->addMeleeWeapon(&m_clawedForelimbs);
            if (bannerBearer) {
                model->setName(Model::BannerBearer);
                bannerBearer = false;
            } else if (brayhorn) {
                model->setName("Brayhorn");
                brayhorn = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    bool Centigors::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *Centigors::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            int numModels = GetIntParam("Models", parameters, g_minUnitSize);
            bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
            bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
            auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
            return new Centigors(fray, numModels, brayhorn, bannerBearer);
        }
        return nullptr;
    }

    void Centigors::Init() {
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

            s_registered = UnitFactory::Register("Centigors", factoryMethod);
        }
    }

    int Centigors::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = BeastsOfChaosBase::toHitModifier(weapon, unit);
        // Drunken Revelry
        if (m_drunkenRevelry) {
            modifier += 1;
        }
        return modifier;
    }

    int Centigors::runModifier() const {
        int modifier = BeastsOfChaosBase::runModifier();
        if (isNamedModelAlive(Model::Drummer)) {
            modifier += 1;
        }
        return modifier;
    }

    int Centigors::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = BeastsOfChaosBase::targetHitModifier(weapon, attacker);
        // Drunken Revelry
        if (m_drunkenRevelry) {
            modifier += 1;
        }
        return modifier;
    }

    int Centigors::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = BeastsOfChaosBase::toSaveModifier(weapon, attacker);

        // Beastbucklers
        if (!weapon->isMissile()) {
            modifier += 1;
        }
        return modifier;
    }

    Rerolls Centigors::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Charging Spear
        if (m_charged && (weapon->name() == m_centigorSpear.name())) {
            return Rerolls::Failed;
        }
        return BeastsOfChaosBase::toWoundRerolls(weapon, target);
    }

    int Centigors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos
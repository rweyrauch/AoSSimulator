/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Gors.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 200;

    bool Gors::s_registered = false;

    Gors::Gors(Greatfray fray, int numModels, bool pairedBlades, bool brayhorn, bool bannerBearer, int points) :
            BeastsOfChaosBase("Gors", 6, g_wounds, 5, 5, false, points),
            m_gorBlade(Weapon::Type::Melee, "Gor Blade", 1, 1, 4, 3, 0, 1),
            m_gorBladeFoeRender(Weapon::Type::Melee, "Gor Blade", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, GORS};
        m_weapons = {&m_gorBlade, &m_gorBladeFoeRender};
        m_battleFieldRole = Role::Battleline;

        setGreatfray(fray);

        m_pairedBlades = pairedBlades;

        auto foe = new Model(g_basesize, wounds());
        foe->addMeleeWeapon(&m_gorBladeFoeRender);
        addModel(foe);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_gorBlade);
            if (bannerBearer) {
                model->setName(Model::BannerBearer);
                bannerBearer = false;
            } else if (brayhorn) {
                model->setName("Brayhorn");
                brayhorn = false;
            }
            addModel(model);
        }
    }

    Unit *Gors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool pairedBlades = GetBoolParam("Paired Blades", parameters, false);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);

        return new Gors(fray, numModels, pairedBlades, brayhorn, bannerBearer, ComputePoints(parameters));
    }

    void Gors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Paired Blades"),
                            BoolParameter("Brayhorn"),
                            BoolParameter("Banner Bearer"),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Gors", factoryMethod);
        }
    }

    Rerolls Gors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Rend and Tear
        if (m_pairedBlades) { return Rerolls::Ones; }
        return BeastsOfChaosBase::toHitRerolls(weapon, target);
    }

    int Gors::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = BeastsOfChaosBase::toSaveModifier(weapon, attacker);
        // Beastsheilds
        if (!m_pairedBlades) { modifier += 1; }
        return modifier;
    }

    int Gors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = BeastsOfChaosBase::extraAttacks(attackingModel, weapon, target);
        // Anarchy and Mayhem
        if (remainingModels() >= 20) {
            attacks += 1;
        }
        return attacks;
    }

    int Gors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos
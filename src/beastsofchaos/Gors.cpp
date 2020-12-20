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

    Gors::Gors() :
            BeastsOfChaosBase("Gors", 6, g_wounds, 5, 5, false),
            m_gorBlade(Weapon::Type::Melee, "Gor Blade", 1, 1, 4, 3, 0, 1),
            m_gorBladeFoeRender(Weapon::Type::Melee, "Gor Blade", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, GORS};
        m_weapons = {&m_gorBlade, &m_gorBladeFoeRender};
        m_battleFieldRole = Battleline;
    }

    bool Gors::configure(int numModels, bool pairedBlades, bool brayhorn, bool bannerBearer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

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
            }
            else if (brayhorn) {
                model->setName("Brayhorn");
                brayhorn = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Gors::Create(const ParameterList &parameters) {
        auto unit = new Gors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool pairedBlades = GetBoolParam("Paired Blades", parameters, false);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels, pairedBlades, brayhorn, bannerBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
        if (m_pairedBlades) { return Reroll_Ones; }
        return Unit::toHitRerolls(weapon, target);
    }

    int Gors::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);
        // Beastsheilds
        if (!m_pairedBlades) { modifier += 1; }
        return modifier;
    }

    int Gors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Unit::extraAttacks(attackingModel, weapon, target);
        // Anarchy and Mayhem
        if (remainingModels() >= 20) {
            attacks += 1;
        }
        return attacks;
    }

    int Gors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos
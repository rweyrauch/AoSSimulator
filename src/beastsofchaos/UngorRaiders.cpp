/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/UngorRaiders.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 320;

    bool UngorRaiders::s_registered = false;

    UngorRaiders::UngorRaiders(Greatfray fray, int numModels, bool brayhorn, bool bannerBearer) :
            BeastsOfChaosBase("Ungor Raiders", 6, g_wounds, 4, 6, false),
            m_raiderBow(Weapon::Type::Missile, "Raider Bow", 18, 1, 4, 4, 0, 1),
            m_raiderBowHalfhorn(Weapon::Type::Missile, "Raider Bow", 18, 1, 3, 4, 0, 1),
            m_jaggedShank(Weapon::Type::Melee, "Jagged Shank", 1, 1, 5, 5, 0, 1) {
        m_keywords = {CHAOS, UNGOR, BEASTS_OF_CHAOS, BRAYHERD, UNGOR_RAIDERS};
        m_weapons = {&m_raiderBow, &m_raiderBowHalfhorn, &m_jaggedShank};

        setGreatfray(fray);

        m_runAndCharge = brayhorn;

        auto halfhorn = new Model(g_basesize, wounds());
        halfhorn->addMissileWeapon(&m_raiderBowHalfhorn);
        halfhorn->addMeleeWeapon(&m_jaggedShank);
        addModel(halfhorn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_raiderBow);
            model->addMeleeWeapon(&m_jaggedShank);
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

    Unit *UngorRaiders::Create(const ParameterList &parameters) {
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        return new UngorRaiders(fray, numModels, brayhorn, bannerBearer);
    }

    void UngorRaiders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
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

            s_registered = UnitFactory::Register("Ungor Raiders", factoryMethod);
        }
    }

    std::string UngorRaiders::ValueToString(const Parameter &parameter) {
        return BeastsOfChaosBase::ValueToString(parameter);
    }

    int UngorRaiders::EnumStringToInt(const std::string &enumString) {
        return BeastsOfChaosBase::EnumStringToInt(enumString);
    }

    Rerolls UngorRaiders::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Baying Anger
        if (weapon->isMissile()) {
            if (remainingModels() >= 30) {
                return Rerolls::Ones_And_Twos;
            } else if (remainingModels() >= 20) {
                return Rerolls::Ones;
            }
        }
        return BeastsOfChaosBase::toHitRerolls(weapon, target);
    }

    int UngorRaiders::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/UngorRaiders.h>
#include <UnitFactory.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool UngorRaiders::s_registered = false;

    UngorRaiders::UngorRaiders() :
            BeastsOfChaosBase("Ungor Raiders", 6, WOUNDS, 4, 6, false),
            m_raiderBow(Weapon::Type::Missile, "Raider Bow", 18, 1, 4, 4, 0, 1),
            m_raiderBowHalfhorn(Weapon::Type::Missile, "Raider Bow", 18, 1, 3, 4, 0, 1),
            m_jaggedShank(Weapon::Type::Melee, "Jagged Shank", 1, 1, 5, 5, 0, 1) {
        m_keywords = {CHAOS, UNGOR, BEASTS_OF_CHAOS, BRAYHERD, UNGOR_RAIDERS};
        m_weapons = {&m_raiderBow, &m_raiderBowHalfhorn, &m_jaggedShank};
    }

    bool UngorRaiders::configure(int numModels, bool brayhorn, bool bannerBearer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_brayhorn = brayhorn;
        m_bannerBearer = bannerBearer;

        m_runAndCharge = m_brayhorn;

        auto halfhorn = new Model(BASESIZE, wounds());
        halfhorn->addMissileWeapon(&m_raiderBowHalfhorn);
        halfhorn->addMeleeWeapon(&m_jaggedShank);
        addModel(halfhorn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_raiderBow);
            model->addMeleeWeapon(&m_jaggedShank);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *UngorRaiders::Create(const ParameterList &parameters) {
        auto unit = new UngorRaiders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels, brayhorn, bannerBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void UngorRaiders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
                return RerollOnesAndTwos;
            } else if (remainingModels() >= 20) {
                return RerollOnes;
            }
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int UngorRaiders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace BeastsOfChaos
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Ungors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 200;

    bool Ungors::s_registered = false;

    Ungors::Ungors() :
            BeastsOfChaosBase("Ungors", 6, WOUNDS, 4, 6, false),
            m_ungorBlade(Weapon::Type::Melee, "Ungor Blade", 1, 1, 4, 4, 0, 1),
            m_ungorBladeHalfhorn(Weapon::Type::Melee, "Ungor Blade", 1, 2, 4, 4, 0, 1),
            m_gnarledShortspear(Weapon::Type::Melee, "Gnarled Shortspear", 2, 1, 5, 4, 0, 1),
            m_gnarledShortspearHalfhorn(Weapon::Type::Melee, "Gnarled Shortspear", 2, 2, 5, 4, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, UNGORS};
        m_weapons = {&m_ungorBlade, &m_ungorBladeHalfhorn, &m_gnarledShortspear, &m_gnarledShortspearHalfhorn};
    }

    bool Ungors::configure(int numModels, WeaponOptions weapons,
                           bool brayhorn, bool bannerBearer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_brayhorn = brayhorn;
        m_bannerBearer = bannerBearer;

        m_runAndCharge = m_brayhorn;

        auto halfhorn = new Model(BASESIZE, wounds());
        if (weapons == UngorBlade) {
            halfhorn->addMeleeWeapon(&m_ungorBladeHalfhorn);
        } else if (weapons == GnarledShortspear) {
            halfhorn->addMeleeWeapon(&m_gnarledShortspearHalfhorn);
        }
        addModel(halfhorn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == UngorBlade) {
                model->addMeleeWeapon(&m_ungorBlade);
            } else if (weapons == GnarledShortspear) {
                model->addMeleeWeapon(&m_gnarledShortspear);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Ungors::Create(const ParameterList &parameters) {
        auto unit = new Ungors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapon = (WeaponOptions) GetEnumParam("Weapons", parameters, UngorBlade);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels, weapon, brayhorn, bannerBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Ungors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Weapons", UngorBlade, UngorBlade, GnarledShortspear, 1},
                            {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None,
                             BeastsOfChaosBase::Gavespawn, 1},
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Ungors", factoryMethod);
        }
    }

    std::string Ungors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == UngorBlade) { return "Ungor Blade"; }
            else if (parameter.intValue == GnarledShortspear) { return "Gnarled Shortspear"; }
        }
        return BeastsOfChaosBase::ValueToString(parameter);
    }

    int Ungors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ungor Blade") { return UngorBlade; }
        else if (enumString == "Gnarled Shortspear") { return GnarledShortspear; }

        return BeastsOfChaosBase::EnumStringToInt(enumString);
    }

    Rerolls Ungors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Braying Hatred
        if (remainingModels() >= 30) {
            return RerollOnesAndTwos;
        } else if (remainingModels() >= 20) {
            return RerollOnes;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int Ungors::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);
        // Half-shields
        if (!weapon->isMissile()) {
            modifier += 1;
        }
        return modifier;
    }

    int Ungors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace BeastsOfChaos
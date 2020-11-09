/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Ungors.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 200;

    bool Ungors::s_registered = false;

    Ungors::Ungors() :
            BeastsOfChaosBase("Ungors", 6, g_wounds, 4, 6, false),
            m_ungorBlade(Weapon::Type::Melee, "Ungor Blade", 1, 1, 4, 4, 0, 1),
            m_ungorBladeHalfhorn(Weapon::Type::Melee, "Ungor Blade", 1, 2, 4, 4, 0, 1),
            m_gnarledShortspear(Weapon::Type::Melee, "Gnarled Shortspear", 2, 1, 5, 4, 0, 1),
            m_gnarledShortspearHalfhorn(Weapon::Type::Melee, "Gnarled Shortspear", 2, 2, 5, 4, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, UNGORS};
        m_weapons = {&m_ungorBlade, &m_ungorBladeHalfhorn, &m_gnarledShortspear, &m_gnarledShortspearHalfhorn};
        m_battleFieldRole = Battleline;
    }

    bool Ungors::configure(int numModels, WeaponOptions weapons,
                           bool brayhorn, bool bannerBearer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_brayhorn = brayhorn;
        m_bannerBearer = bannerBearer;

        m_runAndCharge = m_brayhorn;

        auto halfhorn = new Model(g_basesize, wounds());
        if (weapons == Ungor_Blade) {
            halfhorn->addMeleeWeapon(&m_ungorBladeHalfhorn);
        } else if (weapons == Gnarled_Shortspear) {
            halfhorn->addMeleeWeapon(&m_gnarledShortspearHalfhorn);
        }
        addModel(halfhorn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Ungor_Blade) {
                model->addMeleeWeapon(&m_ungorBlade);
            } else if (weapons == Gnarled_Shortspear) {
                model->addMeleeWeapon(&m_gnarledShortspear);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Ungors::Create(const ParameterList &parameters) {
        auto unit = new Ungors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapon = (WeaponOptions) GetEnumParam("Weapons", parameters, Ungor_Blade);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
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
            static const std::array<int, 2> weapons = {Ungor_Blade, Gnarled_Shortspear};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter<2>("Weapons", weapons[0], weapons),
                            BoolParameter("Brayhorn"),
                            BoolParameter("Banner Bearer"),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Ungors", factoryMethod);
        }
    }

    std::string Ungors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Ungor_Blade) { return "Ungor Blade"; }
            else if (parameter.intValue == Gnarled_Shortspear) { return "Gnarled Shortspear"; }
        }
        return BeastsOfChaosBase::ValueToString(parameter);
    }

    int Ungors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ungor Blade") { return Ungor_Blade; }
        else if (enumString == "Gnarled Shortspear") { return Gnarled_Shortspear; }

        return BeastsOfChaosBase::EnumStringToInt(enumString);
    }

    Rerolls Ungors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Braying Hatred
        if (remainingModels() >= 30) {
            return Reroll_Ones_And_Twos;
        } else if (remainingModels() >= 20) {
            return Reroll_Ones;
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos
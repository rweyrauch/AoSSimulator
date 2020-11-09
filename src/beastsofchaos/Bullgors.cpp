/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Bullgors.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Bullgors::s_registered = false;

    Bullgors::Bullgors() :
            BeastsOfChaosBase("Bullgors", 7, g_wounds, 6, 5, false),
            m_bullgorHorns(Weapon::Type::Melee, "Bullgor Horns", 1, 2, 4, 4, 0, 1),
            m_bullgorAxe(Weapon::Type::Melee, "Bullgor Axe", 1, 3, 4, 3, -1, 2),
            m_bullgorAxeBloodkine(Weapon::Type::Melee, "Bullgor Axe", 1, 4, 4, 3, -1, 2),
            m_bullgorGreatAxe(Weapon::Type::Melee, "Bullgor Great Axe", 1, 2, 4, 3, -2, 3),
            m_bullgorGreatAxeBloodkine(Weapon::Type::Melee, "Bullgor Great Axe", 1, 3, 4, 3, -2, 3) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, WARHERD, BULLGORS};
        m_weapons = {&m_bullgorHorns, &m_bullgorAxe, &m_bullgorAxeBloodkine, &m_bullgorGreatAxe,
                     &m_bullgorGreatAxeBloodkine};
    }

    bool Bullgors::configure(int numModels, WeaponOptions options,
                             bool drummer, bool bannerBearer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_drummer = drummer;
        m_bannerBearer = bannerBearer;
        m_pairedAxes = (options == Paired_Bullgor_Axes);

        auto bloodkine = new Model(g_basesize, wounds());
        bloodkine->addMeleeWeapon(&m_bullgorHorns);
        if (options == Bullgor_Axe || options == Paired_Bullgor_Axes) {
            bloodkine->addMeleeWeapon(&m_bullgorAxeBloodkine);
        } else if (options == Bullgor_Great_Axe) {
            bloodkine->addMeleeWeapon(&m_bullgorGreatAxeBloodkine);
        }
        addModel(bloodkine);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_bullgorHorns);
            if (options == Bullgor_Axe || options == Paired_Bullgor_Axes) {
                model->addMeleeWeapon(&m_bullgorAxe);
            } else if (options == Bullgor_Great_Axe) {
                model->addMeleeWeapon(&m_bullgorGreatAxe);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Bullgors::Create(const ParameterList &parameters) {
        auto unit = new Bullgors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapon = (WeaponOptions) GetEnumParam("Weapons", parameters, Bullgor_Axe);
        bool drummer = GetBoolParam("Drummer", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels, weapon, drummer, bannerBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Bullgors::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Bullgor_Axe, Paired_Bullgor_Axes, Bullgor_Great_Axe};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", weapons[0], weapons),
                            BoolParameter("Drummer"),
                            BoolParameter("Banner Bearer"),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Bullgors", factoryMethod);
        }
    }

    std::string Bullgors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Bullgor_Axe) { return "Bullgor Axe"; }
            else if (parameter.intValue == Paired_Bullgor_Axes) { return "Paired Bullgor Axes"; }
            else if (parameter.intValue == Bullgor_Great_Axe) { return "Bullgor Great Axe"; }
        }
        return BeastsOfChaosBase::ValueToString(parameter);
    }

    int Bullgors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Bullgor Axe") { return Bullgor_Axe; }
        else if (enumString == "Paired Bullgor Axes") { return Paired_Bullgor_Axes; }
        else if (enumString == "Bullgor Great Axe") { return Bullgor_Great_Axe; }
        return BeastsOfChaosBase::EnumStringToInt(enumString);
    }

    int Bullgors::chargeModifier() const {
        int modifier = Unit::chargeModifier();
        if (m_drummer) {
            modifier += 1;
        }
        return modifier;
    }

    Rerolls Bullgors::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        if (m_pairedAxes) {
            return Reroll_Ones;
        }
        return Unit::toHitRerolls(weapon, unit);
    }

    int Bullgors::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);
        if (!m_pairedAxes) {
            modifier += 1;
        }
        return modifier;
    }

    Wounds Bullgors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Bloodgreed
        if (woundRoll == 6) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Bullgors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos
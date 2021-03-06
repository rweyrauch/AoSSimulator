/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/GoreGruntas.h>
#include <UnitFactory.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 90;  // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool OrrukGoreGruntas::s_registered = false;

    OrrukGoreGruntas::OrrukGoreGruntas() :
            Ironjawz("Orruk Gore-gruntas", 9, g_wounds, 7, 4, false),
            m_pigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa", 1, 4, 3, 3, -1, 1),
            m_jaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 3, 3, 3, -1, 1),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 4, 4, 4, 0, 1),
            m_bossPigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa", 1, 5, 3, 3, -1, 1),
            m_bossJaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 5, 3, 3, -1, 1) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, GORE_GRUNTAS};
        m_weapons = {&m_pigironChoppa, &m_jaggedGorehacka, &m_tusksAndHooves, &m_bossPigironChoppa,
                     &m_bossJaggedGorehacka};
        m_hasMount = true;
        m_tusksAndHooves.setMount(true);
    }

    bool OrrukGoreGruntas::configure(int numModels, WeaponOption weapons) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Boss
        auto bossModel = new Model(g_basesize, wounds());
        if (weapons == Pig_Iron_Choppa) {
            bossModel->addMeleeWeapon(&m_bossPigironChoppa);
        } else if (weapons == Jagged_Gorehacka) {
            bossModel->addMeleeWeapon(&m_bossJaggedGorehacka);
        }
        bossModel->addMeleeWeapon(&m_tusksAndHooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Pig_Iron_Choppa) {
                model->addMeleeWeapon(&m_pigironChoppa);
            } else if (weapons == Jagged_Gorehacka) {
                model->addMeleeWeapon(&m_jaggedGorehacka);
            }
            model->addMeleeWeapon(&m_tusksAndHooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *OrrukGoreGruntas::Create(const ParameterList &parameters) {
        auto unit = new OrrukGoreGruntas();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Pig_Iron_Choppa);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string OrrukGoreGruntas::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "weapons") {
            if (parameter.intValue == Pig_Iron_Choppa) {
                return "Pig-iron Choppa";
            } else if (parameter.intValue == Jagged_Gorehacka) {
                return "Jagged Gore-hacka";
            }
        }
        return Ironjawz::ValueToString(parameter);
    }

    int OrrukGoreGruntas::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Pig-iron Choppa") {
            return Pig_Iron_Choppa;
        } else if (enumString == "Jagged Gore-hacka") {
            return Jagged_Gorehacka;
        }
        return Ironjawz::EnumStringToInt(enumString);
    }

    void OrrukGoreGruntas::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Pig_Iron_Choppa, Jagged_Gorehacka};
            static FactoryMethod factoryMethod = {
                    OrrukGoreGruntas::Create,
                    OrrukGoreGruntas::ValueToString,
                    OrrukGoreGruntas::EnumStringToInt,
                    OrrukGoreGruntas::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Pig_Iron_Choppa, weapons),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Gore-gruntas", factoryMethod);
        }
    }

    int OrrukGoreGruntas::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void OrrukGoreGruntas::onCharged() {
        // Gore-grunta Charge
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            Dice::RollResult rolls;
            Dice::RollD6(remainingModels(), rolls);
            Wounds wounds = {0, 0};
            wounds.mortal = rolls.rollsGE(4);

            PLOG_INFO.printf("%s Gore-grunta Charge inflicted %d mortal wounds on %s\n",
                             name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds, this);
        }
        Ironjawz::onCharged();
    }

    int OrrukGoreGruntas::toHitModifier(const Weapon *weapon, const Unit *target) const {
        // Gore-grunta Charge
        auto mod = Ironjawz::toHitModifier(weapon, target);
        if (m_charged && (weapon->name() == m_tusksAndHooves.name() || weapon->name() == m_jaggedGorehacka.name())) {
            mod++;
        }
        return mod;
    }

    int OrrukGoreGruntas::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        // Gore-grunta Charge
        auto mod = Ironjawz::toWoundModifier(weapon, target);
        if (m_charged && (weapon->name() == m_tusksAndHooves.name() || weapon->name() == m_jaggedGorehacka.name())) {
            mod++;
        }
        return mod;
    }

} // namespace Ironjawz
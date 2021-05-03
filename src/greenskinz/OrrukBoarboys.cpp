/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukBoarboys.h>
#include <UnitFactory.h>
#include <Board.h>
#include <array>

namespace Greenskinz {
    static const int g_basesize = 60; // x35 ovals
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool OrrukBoarboys::s_registered = false;

    OrrukBoarboys::OrrukBoarboys(int numModels, WeaponOption weapons, bool glyphBearer, bool horns, int points) :
            Unit("Orruk Boarboys", 9, g_wounds, 5, 5, false, points),
            m_choppa(Weapon::Type::Melee, "Choppa", 1, 1, 4, 4, -1, 1),
            m_pigstikkaSpear(Weapon::Type::Melee, "Pigstikka Spear", 2, 1, 4, 4, 0, 1),
            m_warBoarsTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 2, 4, 4, 0, 1),
            m_choppaBoss(Weapon::Type::Melee, "Choppa", 1, 2, 4, 4, -1, 1),
            m_pigstikkaSpearBoss(Weapon::Type::Melee, "Pigstikka Spear", 2, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, ORRUK_BOARBOYS};
        m_weapons = {&m_choppa, &m_pigstikkaSpear, &m_warBoarsTusks, &m_choppaBoss, &m_pigstikkaSpearBoss};
        m_hasMount = true;
        m_warBoarsTusks.setMount(true);

        m_weaponOption = weapons;

        // Add the boss
        auto boss = new Model(g_basesize, wounds());
        switch (weapons) {
            case Choppa:
                boss->addMeleeWeapon(&m_choppaBoss);
                break;
            case Pigstikka_Spear:
                boss->addMeleeWeapon(&m_pigstikkaSpearBoss);
                break;
        }
        boss->addMeleeWeapon(&m_warBoarsTusks);
        addModel(boss);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            switch (weapons) {
                case Choppa:
                    model->addMeleeWeapon(&m_choppa);
                    break;
                case Pigstikka_Spear:
                    model->addMeleeWeapon(&m_pigstikkaSpear);
                    break;
            }
            model->addMeleeWeapon(&m_warBoarsTusks);
            if (horns) {
                model->setName("Waaagh Horn");
                horns = false;
            } else if (glyphBearer) {
                model->setName("Glyph Bearer");
                glyphBearer = false;
            }
            addModel(model);
        }
    }

    Unit *OrrukBoarboys::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Choppa);
        bool glyphBearer = GetBoolParam("Glyph Bearer", parameters, false);
        bool horns = GetBoolParam("Waaagh! Horns", parameters, false);
        return new OrrukBoarboys(numModels, weapons, glyphBearer, horns, ComputePoints(parameters));
    }

    void OrrukBoarboys::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Choppa, Pigstikka_Spear};
            static FactoryMethod factoryMethod = {
                    OrrukBoarboys::Create,
                    OrrukBoarboys::ValueToString,
                    OrrukBoarboys::EnumStringToInt,
                    OrrukBoarboys::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Choppa, weapons),
                            BoolParameter("Glyph Bearer"),
                            BoolParameter("Waaagh! Horns"),
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruk Boarboys", factoryMethod);
        }
    }

    std::string OrrukBoarboys::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Choppa) {
                return "Choppa";
            } else if (parameter.intValue == Pigstikka_Spear) {
                return "Pigstikka Spear";
            }
        }
        return ParameterValueToString(parameter);
    }

    int OrrukBoarboys::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Choppa") {
            return Choppa;
        } else if (enumString == "Pigstikka Spear") {
            return Pigstikka_Spear;
        }
        return 0;
    }

    int OrrukBoarboys::chargeModifier() const {
        auto modifier = Unit::chargeModifier();
        if (isNamedModelAlive("Waaagh Horn"))
            modifier += 2;
        return modifier;
    }

    int OrrukBoarboys::braveryModifier() const {
        auto modifier = Unit::braveryModifier();
        if (isNamedModelAlive("Glyph Bearer")) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            if (!units.empty()) {
                modifier += 2;
            }
        }
        return modifier;
    }

    int OrrukBoarboys::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        // Tusker Charge
        if (m_charged && weapon->name() == m_warBoarsTusks.name()) {
            mod++;
        }
        return mod;
    }

    Rerolls OrrukBoarboys::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Tusker Shield
        if (!weapon->isMissile()) {
            return Rerolls::Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int OrrukBoarboys::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Greenskinz

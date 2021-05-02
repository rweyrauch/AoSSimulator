/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Clanrats.h>
#include <UnitFactory.h>
#include <array>

namespace Skaven {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 20;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 200;

    bool Clanrats::s_registered = false;

    Clanrats::Clanrats(int points) :
            Skaventide("Clanrats", 6, g_wounds, 4, 6, false, points),
            m_rustySpear(Weapon::Type::Melee, "Rusty Spear", 2, 1, 5, 4, 0, 1),
            m_rustySpearLeader(Weapon::Type::Melee, "Rusty Spear", 2, 2, 5, 4, 0, 1),
            m_rustyBlade(Weapon::Type::Melee, "Rusty Blade", 1, 1, 4, 4, 0, 1),
            m_rustyBladeLeader(Weapon::Type::Melee, "Rusty Blade", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, CLANRATS};
        m_weapons = {&m_rustySpear, &m_rustySpearLeader, &m_rustyBlade, &m_rustyBladeLeader};
        m_battleFieldRole = Role::Battleline;
    }

    bool Clanrats::configure(int numModels, Clanrats::WeaponOptions weapons, bool clanshields, int standardBearers,
                             int bellRingers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }
        int maxStandardBearers = numModels / g_minUnitSize;
        int maxBellRingers = numModels / g_minUnitSize;
        if (standardBearers > maxStandardBearers || bellRingers > maxBellRingers) {
            return false;
        }

        m_clanshields = clanshields;
        m_numStandardBearers = standardBearers;
        m_numBellRingers = bellRingers;

        // Standard Bearers
        m_retreatAndCharge = (standardBearers > 0);

        auto leader = new Model(g_basesize, wounds());
        if (weapons == Rusty_Spear) {
            leader->addMeleeWeapon(&m_rustySpearLeader);
        } else if (weapons == Rusty_Blade) {
            leader->addMeleeWeapon(&m_rustyBladeLeader);
        }
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Rusty_Spear) {
                model->addMeleeWeapon(&m_rustySpear);
            } else if (weapons == Rusty_Blade) {
                model->addMeleeWeapon(&m_rustyBlade);
            }
            addModel(model);
        }

        return true;
    }

    Unit *Clanrats::Create(const ParameterList &parameters) {
        auto unit = new Clanrats(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOptions weapon = (WeaponOptions) GetEnumParam("Weapons", parameters, (int) Rusty_Spear);
        bool clanshields = GetBoolParam("Clanshields", parameters, false);
        int standardBearers = GetIntParam("Standard Bearers", parameters, 0);
        int bellRingers = GetIntParam("Bell Ringers", parameters, 0);

        bool ok = unit->configure(numModels, weapon, clanshields, standardBearers, bellRingers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Clanrats::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Rusty_Spear, Rusty_Blade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Rusty_Spear, weapons),
                            BoolParameter("Clanshields"),
                            IntegerParameter("Standard Bearers", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Bell Ringers", 0, 0, g_maxUnitSize / g_minUnitSize, 1)
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Clanrats", factoryMethod);
        }
    }

    std::string Clanrats::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Rusty_Spear) {
                return "Rusty Spear";
            } else if (parameter.intValue == Rusty_Blade) {
                return "Rusty Blade";
            }
        }

        return ParameterValueToString(parameter);
    }

    int Clanrats::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Rusty Spear") {
            return Rusty_Spear;
        } else if (enumString == "Rusty Blade") {
            return Rusty_Blade;
        }
        return 0;
    }

    int Clanrats::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Skaventide::toSaveModifier(weapon, attacker);
        // Clanshields
        if (m_clanshields && remainingModels() >= 10)
            modifier += 1;
        return modifier;
    }

    int Clanrats::runModifier() const {
        int modifier = Skaventide::runModifier();
        // Bell-ringers
        if (m_numBellRingers > 0)
            modifier += 2;

        return modifier;
    }

    int Clanrats::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Skaven

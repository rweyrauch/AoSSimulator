/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/SkeletonWarriors.h>
#include <Board.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 85;
    static const int g_pointsMaxUnitSize = 3 * g_pointsPerBlock;

    bool DeathrattleSkeletons::s_registered = false;

    DeathrattleSkeletons::DeathrattleSkeletons(CursedBloodline bloodline, int numModels, ChampionWeaponOption weapons, bool standardBearers, int points) :
            SoulblightBase(bloodline, "Deathrattle Skeletons", 4, g_wounds, 10, 6, false, points) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, SUMMONABLE, DEATHRATTLE_SKELETONS};
        m_weapons = {&m_ancientBladeOrSpear, &m_championsMaceOrHalberd};
        m_battleFieldRole = Role::Battleline;

        auto champion = new Model(g_basesize, wounds());
        if (weapons == Ancient_Blade_Or_Spear) {
            champion->addMeleeWeapon(&m_ancientBladeOrSpear);
        } else if (weapons == Champions_Mace_Or_Halberd) {
            champion->addMeleeWeapon(&m_championsMaceOrHalberd);
        }
        addModel(champion);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_ancientBladeOrSpear);
            if (standardBearers) {
                model->setName(Model::StandardBearer);
                standardBearers = false;
            }
            addModel(model);
        }
    }

    DeathrattleSkeletons::~DeathrattleSkeletons() {
    }

    Unit *DeathrattleSkeletons::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (ChampionWeaponOption) GetEnumParam("Weapon", parameters, Champions_Mace_Or_Halberd);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        return new DeathrattleSkeletons(bloodline, numModels, weapons, standardBearers, ComputePoints(parameters));
    }

    void DeathrattleSkeletons::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Ancient_Blade_Or_Spear, Champions_Mace_Or_Halberd};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapon", Champions_Mace_Or_Halberd, weapons),
                            BoolParameter("Standard Bearers"),
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Deathrattle Skeletons", factoryMethod);
        }
    }

    std::string DeathrattleSkeletons::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Ancient_Blade_Or_Spear) { return "Ancient Blade or Spear"; }
            else if (parameter.intValue == Champions_Mace_Or_Halberd) { return "Champion's Mace or Halberd"; }
        }
        return SoulblightBase::ValueToString(parameter);
    }

    int DeathrattleSkeletons::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ancient Blade or Spear") { return Ancient_Blade_Or_Spear; }
        else if (enumString == "Champion's Mace or Halberd") { return Champions_Mace_Or_Halberd; }
        return SoulblightBase::EnumStringToInt(enumString);
    }

    int DeathrattleSkeletons::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Soulblight

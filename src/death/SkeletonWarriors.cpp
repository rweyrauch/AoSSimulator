/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/SkeletonWarriors.h>
#include <Board.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool SkeletonWarriors::s_registered = false;

    SkeletonWarriors::SkeletonWarriors(Legion legion, int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers, int points) :
            LegionOfNagashBase(legion, "Skeleton Warriors", 4, g_wounds, 10, 6, false, points),
            m_ancientBlade(Weapon::Type::Melee, "Ancient Blade", 1, 1, 4, 4, 0, 1),
            m_ancientBladeChampion(Weapon::Type::Melee, "Ancient Blade", 1, 2, 4, 4, 0, 1),
            m_ancientSpear(Weapon::Type::Melee, "Ancient Spear", 2, 1, 5, 4, 0, 1),
            m_ancientSpearChampion(Weapon::Type::Melee, "Ancient Spear", 2, 2, 5, 4, 0, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, SKELETON_WARRIORS};
        m_weapons = {&m_ancientBlade, &m_ancientBladeChampion, &m_ancientSpear, &m_ancientSpearChampion};
        m_battleFieldRole = Role::Battleline;
        s_globalBraveryMod.connect(this, &SkeletonWarriors::standardBearerBraveryMod, &m_standardSlot);

        auto champion = new Model(g_basesize, wounds());
        if (weapons == Ancient_Blade) {
            champion->addMeleeWeapon(&m_ancientBladeChampion);
        } else if (weapons == Ancient_Spear) {
            champion->addMeleeWeapon(&m_ancientSpearChampion);
        }
        addModel(champion);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Ancient_Blade) {
                model->addMeleeWeapon(&m_ancientBlade);
            } else if (weapons == Ancient_Spear) {
                model->addMeleeWeapon(&m_ancientSpear);
            }
            if (standardBearers) {
                model->setName(Model::StandardBearer);
                standardBearers = false;
            } else if (hornblowers) {
                model->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(model);
        }
    }

    SkeletonWarriors::~SkeletonWarriors() {
        m_standardSlot.disconnect();
    }

    Unit *SkeletonWarriors::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Ancient_Blade);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
        return new SkeletonWarriors(legion, numModels, weapons, standardBearers, hornblowers, ComputePoints(parameters));
    }

    void SkeletonWarriors::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Ancient_Blade, Ancient_Spear};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Ancient_Blade, weapons),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {DEATHRATTLE}
            };
            s_registered = UnitFactory::Register("Skeleton Warriors", factoryMethod);
        }
    }

    std::string SkeletonWarriors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Ancient_Blade) { return "Ancient Blade"; }
            else if (parameter.intValue == Ancient_Spear) { return "Ancient Spear"; }
        }
        return LegionOfNagashBase::ValueToString(parameter);
    }

    int SkeletonWarriors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ancient Blade") { return Ancient_Blade; }
        else if (enumString == "Ancient Spear") { return Ancient_Spear; }
        return LegionOfNagashBase::EnumStringToInt(enumString);
    }

    int SkeletonWarriors::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = LegionOfNagashBase::toSaveModifier(weapon, attacker);

        // Crypt Shields
        if (weapon->rend() == 0) {
            modifier += 1;
        }

        return modifier;
    }

    int
    SkeletonWarriors::extraAttacks(const Model * /*attackingModel*/, const Weapon *weapon, const Unit *target) const {
        int attacks = LegionOfNagashBase::extraAttacks(nullptr, weapon, target);
        // Skeleton Legion
        if (remainingModels() >= 30) {
            attacks += 2;
        } else if (remainingModels() >= 20) {
            attacks += 1;
        }
        return attacks;
    }

    int SkeletonWarriors::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = LegionOfNagashBase::toHitModifier(weapon, target);

        // Serve in Death
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
        for (auto ip : units) {
            if (ip->hasKeyword(DEATH) && ip->hasKeyword(HERO)) {
                modifier += 1;
                break;
            }
        }

        return modifier;
    }

    int SkeletonWarriors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int SkeletonWarriors::rollChargeDistance() {
        // Hornblower
        auto dist = LegionOfNagashBase::rollChargeDistance();
        if (isNamedModelAlive(Model::Hornblower))
            return std::max(6, dist);
        return dist;
    }

    int SkeletonWarriors::standardBearerBraveryMod(const Unit *unit) {
        if (isNamedModelAlive(Model::StandardBearer) && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

} //namespace Death

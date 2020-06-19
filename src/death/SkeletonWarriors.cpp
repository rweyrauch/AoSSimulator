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
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool SkeletonWarriors::s_registered = false;

    SkeletonWarriors::SkeletonWarriors() :
            LegionOfNagashBase("Skeleton Warriors", 4, WOUNDS, 10, 6, false),
            m_ancientBlade(Weapon::Type::Melee, "Ancient Blade", 1, 1, 4, 4, 0, 1),
            m_ancientBladeChampion(Weapon::Type::Melee, "Ancient Blade", 1, 2, 4, 4, 0, 1),
            m_ancientSpear(Weapon::Type::Melee, "Ancient Spear", 2, 1, 5, 4, 0, 1),
            m_ancientSpearChampion(Weapon::Type::Melee, "Ancient Spear", 2, 2, 5, 4, 0, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, SKELETON_WARRIORS};
        m_weapons = {&m_ancientBlade, &m_ancientBladeChampion, &m_ancientSpear, &m_ancientSpearChampion};
        s_globalBraveryMod.connect(this, &SkeletonWarriors::standardBearerBraveryMod, &m_standardSlot);
    }

    SkeletonWarriors::~SkeletonWarriors() {
        m_standardSlot.disconnect();
    }

    bool SkeletonWarriors::configure(int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearers = standardBearers;
        m_hornblowers = hornblowers;

        auto champion = new Model(BASESIZE, wounds());
        if (weapons == AncientBlade) {
            champion->addMeleeWeapon(&m_ancientBladeChampion);
        } else if (weapons == AncientSpear) {
            champion->addMeleeWeapon(&m_ancientSpearChampion);
        }
        addModel(champion);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == AncientBlade) {
                model->addMeleeWeapon(&m_ancientBlade);
            } else if (weapons == AncientSpear) {
                model->addMeleeWeapon(&m_ancientSpear);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SkeletonWarriors::Create(const ParameterList &parameters) {
        auto unit = new SkeletonWarriors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, AncientBlade);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, weapons, standardBearers, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SkeletonWarriors::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {AncientBlade, AncientSpear};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", AncientBlade, weapons),
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
            if (parameter.intValue == AncientBlade) { return "Ancient Blade"; }
            else if (parameter.intValue == AncientSpear) { return "Ancient Spear"; }
        }
        return LegionOfNagashBase::ValueToString(parameter);
    }

    int SkeletonWarriors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ancient Blade") { return AncientBlade; }
        else if (enumString == "Ancient Spear") { return AncientSpear; }
        return LegionOfNagashBase::EnumStringToInt(enumString);
    }

    int SkeletonWarriors::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);

        // Crypt Shields
        if (weapon->rend() == 0) {
            modifier += 1;
        }

        return modifier;
    }

    int
    SkeletonWarriors::extraAttacks(const Model * /*attackingModel*/, const Weapon *weapon, const Unit *target) const {
        int attacks = Unit::extraAttacks(nullptr, weapon, target);
        // Skeleton Legion
        if (remainingModels() >= 30) {
            attacks += 2;
        } else if (remainingModels() >= 20) {
            attacks += 1;
        }
        return attacks;
    }

    int SkeletonWarriors::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toHitModifier(weapon, target);

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

    int SkeletonWarriors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int SkeletonWarriors::rollChargeDistance() const {
        // Hornblower
        auto dist = Unit::rollChargeDistance();
        return std::max(6, dist);
    }

    int SkeletonWarriors::standardBearerBraveryMod(const Unit *unit) {
        if (m_standardBearers && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

} //namespace Death

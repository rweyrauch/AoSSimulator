/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosWarriors.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 540;

    bool ChaosWarriors::s_registered = false;

    Unit *ChaosWarriors::Create(const ParameterList &parameters) {
        auto unit = new ChaosWarriors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Paired_Hand_Weapons);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure(numModels, weapons, standardBearer, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosWarriors::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Hand_Weapon_And_Shield, Halberd_And_Shield, Great_Blade, Paired_Hand_Weapons};

            static FactoryMethod factoryMethod = {
                    ChaosWarriors::Create,
                    ChaosWarriors::ValueToString,
                    ChaosWarriors::EnumStringToInt,
                    ChaosWarriors::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Hand_Weapon_And_Shield, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Warriors", factoryMethod);
        }
    }

    ChaosWarriors::ChaosWarriors() :
            SlavesToDarknessBase("Chaos Warriors", 5, g_wounds, 7, 4, false),
            m_handWeapons(Weapon::Type::Melee, "Chaos Hand Weapons", 1, 2, 3, 3, 0, 1),
            m_halberd(Weapon::Type::Melee, "Chaos Halberd", 2, 2, 3, 4, 0, 1),
            m_greatBlade(Weapon::Type::Melee, "Chaos Greatblade", 1, 2, 4, 3, -1, 1),
            m_handWeaponsChampion(Weapon::Type::Melee, "Chaos Hand Weapons", 1, 3, 3, 3, 0, 1),
            m_halberdChampion(Weapon::Type::Melee, "Chaos Halberd", 2, 3, 3, 4, 0, 1),
            m_greatBladeChampion(Weapon::Type::Melee, "Chaos Greatblade", 1, 3, 4, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_WARRIORS};
        m_weapons = {&m_handWeapons, &m_halberd, &m_greatBlade, &m_handWeaponsChampion, &m_halberdChampion,
                     &m_greatBladeChampion};
        m_battleFieldRole = Battleline;
    }

    bool ChaosWarriors::configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        m_hasShields = false;
        m_pairedWeapons = false;

        auto champion = new Model(g_basesize, wounds());
        if (weapons == Hand_Weapon_And_Shield) {
            champion->addMeleeWeapon(&m_handWeaponsChampion);
            m_hasShields = true;
        } else if (weapons == Halberd_And_Shield) {
            champion->addMeleeWeapon(&m_halberdChampion);
            m_hasShields = true;
        } else if (weapons == Great_Blade) {
            champion->addMeleeWeapon(&m_greatBladeChampion);
        } else if (weapons == Paired_Hand_Weapons) {
            champion->addMeleeWeapon(&m_handWeaponsChampion);
            m_pairedWeapons = true;
        }
        champion->setName("Aspiring Champion");
        addModel(champion);

        if (m_standardBearer) {
            auto model = new Model(g_basesize, wounds());
            model->setName("Standard Bearer");
            if (weapons == Hand_Weapon_And_Shield)
                model->addMeleeWeapon(&m_handWeapons);
            else if (weapons == Halberd_And_Shield)
                model->addMeleeWeapon(&m_halberd);
            else if (weapons == Great_Blade)
                model->addMeleeWeapon(&m_greatBlade);
            else if (weapons == Paired_Hand_Weapons)
                model->addMeleeWeapon(&m_handWeapons);
            addModel(model);
        }

        if (m_hornblower) {
            auto model = new Model(g_basesize, wounds());
            model->setName("Hornblower");
            if (weapons == Hand_Weapon_And_Shield)
                model->addMeleeWeapon(&m_handWeapons);
            else if (weapons == Halberd_And_Shield)
                model->addMeleeWeapon(&m_halberd);
            else if (weapons == Great_Blade)
                model->addMeleeWeapon(&m_greatBlade);
            else if (weapons == Paired_Hand_Weapons)
                model->addMeleeWeapon(&m_handWeapons);
            addModel(model);
        }

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Hand_Weapon_And_Shield)
                model->addMeleeWeapon(&m_handWeapons);
            else if (weapons == Halberd_And_Shield)
                model->addMeleeWeapon(&m_halberd);
            else if (weapons == Great_Blade)
                model->addMeleeWeapon(&m_greatBlade);
            else if (weapons == Paired_Hand_Weapons)
                model->addMeleeWeapon(&m_handWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    std::string ChaosWarriors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Hand_Weapon_And_Shield) {
                return "Hand Weapon and Shield";
            } else if (parameter.intValue == Halberd_And_Shield) {
                return "Halberd and Shield";
            } else if (parameter.intValue == Great_Blade) {
                return "Greatblade";
            } else if (parameter.intValue == Paired_Hand_Weapons) {
                return "Paired Hand Weapons";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosWarriors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Hand Weapon and Shield") {
            return Hand_Weapon_And_Shield;
        } else if (enumString == "Halberd and Shield") {
            return Halberd_And_Shield;
        } else if (enumString == "Greatblade") {
            return Great_Blade;
        } else if (enumString == "Paired Hand Weapons") {
            return Paired_Hand_Weapons;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    void ChaosWarriors::onWounded() {
        Unit::onWounded();

        // Check for special models
        for (const auto &ip : m_models) {
            if (ip->slain() && (ip->getName() == "Hornblower")) {
                m_hornblower = false;
            }
            if (ip->slain() && (ip->getName() == "Standard Bearer")) {
                m_standardBearer = false;
            }
        }
    }

    Wounds ChaosWarriors::applyWoundSave(const Wounds &wounds) {
        if (m_hasShields) {
            // Chaos Runeshield
            Dice::RollResult mortalSaves;
            Dice::RollD6(wounds.mortal, mortalSaves);

            Wounds totalWounds = wounds;
            totalWounds.mortal -= mortalSaves.rollsGE(5);
            totalWounds.mortal = std::max(totalWounds.mortal, 0);

            return totalWounds;
        }
        return Unit::applyWoundSave(wounds);
    }

    int ChaosWarriors::runModifier() const {
        auto modifier = Unit::runModifier();
        if (m_hornblower) modifier += 1;
        return modifier;
    }

    int ChaosWarriors::chargeModifier() const {
        auto modifier = Unit::chargeModifier();
        if (m_hornblower) modifier += 1;
        return modifier;
    }

    int ChaosWarriors::braveryModifier() const {
        auto modifier = Unit::braveryModifier();
        if (m_standardBearer) modifier += 1;
        return modifier;
    }

    Rerolls ChaosWarriors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Pair of Chaos Hand Weapons
        if (m_pairedWeapons)
            return Reroll_Ones;
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls ChaosWarriors::toSaveRerolls(const Weapon *weapon) const {
        // Legions of Chaos
        if (remainingModels() >= 10)
            return Reroll_Failed;
        return Unit::toSaveRerolls(weapon);
    }

    void ChaosWarriors::onRestore() {
        Unit::onRestore();

        // Check for special models
        for (const auto &ip : m_models) {
            if (ip->getName() == "Hornblower") {
                m_hornblower = true;
            }
            if (ip->getName() == "Standard Bearer") {
                m_standardBearer = true;
            }
        }
    }

    int ChaosWarriors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //SlavesToDarkness
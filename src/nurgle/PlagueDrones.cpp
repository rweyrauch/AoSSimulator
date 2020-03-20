/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/PlagueDrones.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Nurgle {
    static const int BASESIZE = 60;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 800;

    bool PlagueDrones::s_registered = false;

    PlagueDrones::PlagueDrones() :
            NurgleBase("Plague Drones", 8, WOUNDS, 10, 5, true),
            m_plaguesword(Weapon::Type::Melee, "Plaguesword", 1, 1, 4, 3, 0, 1),
            m_plagueswordPlaguebringer(Weapon::Type::Melee, "Plaguesword", 1, 2, 4, 3, 0, 1),
            m_deathsHead(Weapon::Type::Missile, "Death's Head", 14, 1, 4, 3, 0, 1),
            m_proboscis(Weapon::Type::Melee, "Prehensile Proboscis", 1, 3, 3, 4, 0, 1),
            m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
            m_venemousSting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, PLAGUE_DRONES};
        m_weapons = {&m_plaguesword, &m_plagueswordPlaguebringer, &m_deathsHead, &m_proboscis, &m_mouthparts,
                     &m_venemousSting};

        s_globalBattleshockReroll.connect(this, &PlagueDrones::bellTollersBattleshockReroll, &m_bellTollerSlot);
    }

    PlagueDrones::~PlagueDrones() {
        m_bellTollerSlot.disconnect();
    }

    bool PlagueDrones::configure(int numModels, WeaponOption weapons, bool iconBearer, bool bellTollers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_bellTollers = bellTollers;
        m_weapon = weapons;

        // Add the Plaguebringer
        auto leader = new Model(BASESIZE, wounds());
        leader->addMissileWeapon(&m_deathsHead);
        leader->addMeleeWeapon(&m_plagueswordPlaguebringer);
        if (weapons == PrehensileProboscis) {
            leader->addMeleeWeapon(&m_proboscis);
        } else if (weapons == FoulMouthparts) {
            leader->addMeleeWeapon(&m_mouthparts);
        }
        leader->addMeleeWeapon(&m_venemousSting);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_deathsHead);
            model->addMeleeWeapon(&m_plaguesword);
            if (weapons == PrehensileProboscis) {
                model->addMeleeWeapon(&m_proboscis);
            } else if (weapons == FoulMouthparts) {
                model->addMeleeWeapon(&m_mouthparts);
            }
            model->addMeleeWeapon(&m_venemousSting);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *PlagueDrones::Create(const ParameterList &parameters) {
        auto unit = new PlagueDrones();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, PrehensileProboscis);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool bells = GetBoolParam("Bell Tollers", parameters, false);

        bool ok = unit->configure(numModels, weapons, iconBearer, bells);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void PlagueDrones::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    PlagueDrones::Create,
                    PlagueDrones::ValueToString,
                    PlagueDrones::EnumStringToInt,
                    PlagueDrones::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Weapons", PlagueDrones::PrehensileProboscis,
                             PlagueDrones::PrehensileProboscis, PlagueDrones::FoulMouthparts},
                            {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Bell Tollers", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Plague Drones", factoryMethod);
        }
    }

    Wounds PlagueDrones::applyWoundSave(const Wounds &wounds) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    void PlagueDrones::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Unit::computeBattleshockEffect(roll, numFled, numAdded);
        if (m_iconBearer) {
            // Icon Bearer
            if (roll == 1) {
                numAdded = Dice::rollD6();
            }
        }
    }

    void PlagueDrones::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_deathsHead);
            model->addMeleeWeapon(&m_plaguesword);
            if (m_weapon == PrehensileProboscis) {
                model->addMeleeWeapon(&m_proboscis);
            } else if (m_weapon == FoulMouthparts) {
                model->addMeleeWeapon(&m_mouthparts);
            }
            model->addMeleeWeapon(&m_venemousSting);
            addModel(model);
        }
    }

    std::string PlagueDrones::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == PrehensileProboscis) { return "Prehensile Proboscis"; }
            else if (parameter.intValue == FoulMouthparts) { return "Foul Mouthparts"; }
        }
        return NurgleBase::ValueToString(parameter);
    }

    int PlagueDrones::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Prehensile Proboscis") { return PrehensileProboscis; }
        else if (enumString == "Foul Mouthparts") { return FoulMouthparts; }
        return NurgleBase::EnumStringToInt(enumString);
    }

    int PlagueDrones::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int PlagueDrones::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Locus of Contagion
        if (!weapon->isMissile()) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 7.0f);
            for (auto unit : units) {
                if (unit->hasKeyword(DAEMON) && unit->hasKeyword(NURGLE) && unit->hasKeyword(HERO)) {
                    extra++;
                    break;
                }
            }
        }
        return extra;
    }

    Rerolls PlagueDrones::bellTollersBattleshockReroll(const Unit *unit) {
        // Bell Tollers
        if (m_bellTollers && !isFriendly(unit)) {
            if (distanceTo(unit) <= 6.0f) return RerollOnes;
        }
        return RerollOnes;
    }

} // namespace Nurgle


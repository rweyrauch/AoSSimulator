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
#include <array>

namespace Nurgle {
    static const int g_basesize = 60;
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool PlagueDrones::s_registered = false;

    PlagueDrones::PlagueDrones() :
            NurgleBase("Plague Drones", 8, g_wounds, 10, 5, true),
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
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_weapon = weapons;

        // Add the Plaguebringer
        auto leader = new Model(g_basesize, wounds());
        leader->addMissileWeapon(&m_deathsHead);
        leader->addMeleeWeapon(&m_plagueswordPlaguebringer);
        if (weapons == Prehensile_Proboscis) {
            leader->addMeleeWeapon(&m_proboscis);
        } else if (weapons == Foul_Mouthparts) {
            leader->addMeleeWeapon(&m_mouthparts);
        }
        leader->addMeleeWeapon(&m_venemousSting);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_deathsHead);
            model->addMeleeWeapon(&m_plaguesword);
            if (weapons == Prehensile_Proboscis) {
                model->addMeleeWeapon(&m_proboscis);
            } else if (weapons == Foul_Mouthparts) {
                model->addMeleeWeapon(&m_mouthparts);
            }
            model->addMeleeWeapon(&m_venemousSting);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            }
            else if (bellTollers) {
                model->setName("Bell Tollers");
                bellTollers = false;
            }

            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *PlagueDrones::Create(const ParameterList &parameters) {
        auto unit = new PlagueDrones();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Prehensile_Proboscis);
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
            static const std::array<int, 2> weapons = {Prehensile_Proboscis, Foul_Mouthparts};
            static FactoryMethod factoryMethod = {
                    PlagueDrones::Create,
                    PlagueDrones::ValueToString,
                    PlagueDrones::EnumStringToInt,
                    PlagueDrones::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Prehensile_Proboscis, weapons),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Bell Tollers"),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Plague Drones", factoryMethod);
        }
    }

    Wounds PlagueDrones::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(wounds.normal, woundSaves);
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    void PlagueDrones::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Unit::computeBattleshockEffect(roll, numFled, numAdded);
        if (isNamedModelAlive(Model::IconBearer)) {
            // Icon Bearer
            if (roll == 1) {
                numAdded = Dice::RollD6();
            }
        }
    }

    void PlagueDrones::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_deathsHead);
            model->addMeleeWeapon(&m_plaguesword);
            if (m_weapon == Prehensile_Proboscis) {
                model->addMeleeWeapon(&m_proboscis);
            } else if (m_weapon == Foul_Mouthparts) {
                model->addMeleeWeapon(&m_mouthparts);
            }
            model->addMeleeWeapon(&m_venemousSting);
            addModel(model);
        }
    }

    std::string PlagueDrones::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Prehensile_Proboscis) { return "Prehensile Proboscis"; }
            else if (parameter.intValue == Foul_Mouthparts) { return "Foul Mouthparts"; }
        }
        return NurgleBase::ValueToString(parameter);
    }

    int PlagueDrones::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Prehensile Proboscis") { return Prehensile_Proboscis; }
        else if (enumString == "Foul Mouthparts") { return Foul_Mouthparts; }
        return NurgleBase::EnumStringToInt(enumString);
    }

    int PlagueDrones::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int PlagueDrones::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Locus of Contagion
        if (!weapon->isMissile()) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 7.0);
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
        if (isNamedModelAlive("Bell Tollers") && !isFriendly(unit)) {
            if (distanceTo(unit) <= 6.0) return Reroll_Ones;
        }
        return Reroll_Ones;
    }

} // namespace Nurgle


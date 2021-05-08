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
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 60;
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool PlagueDrones::s_registered = false;

    PlagueDrones::PlagueDrones(PlagueLegion legion, int numModels, WeaponOption weapon, bool iconBearer, bool bellTollers, int points) :
            NurgleBase(legion,"Plague Drones", 8, g_wounds, 10, 5, true, points),
            m_plaguesword(Weapon::Type::Melee, "Plaguesword", 1, 1, 4, 3, 0, 1),
            m_plagueswordPlaguebringer(Weapon::Type::Melee, "Plaguesword", 1, 2, 4, 3, 0, 1),
            m_deathsHead(Weapon::Type::Missile, "Death's Head", 14, 1, 4, 3, 0, 1),
            m_proboscis(Weapon::Type::Melee, "Prehensile Proboscis", 1, 3, 3, 4, 0, 1),
            m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
            m_venemousSting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, PLAGUE_DRONES};
        m_weapons = {&m_plaguesword, &m_plagueswordPlaguebringer, &m_deathsHead, &m_proboscis, &m_mouthparts,
                     &m_venemousSting};
        m_hasMount = true;
        m_proboscis.setMount(true);
        m_mouthparts.setMount(true);
        m_venemousSting.setMount(true);
        s_globalBattleshockReroll.connect(this, &PlagueDrones::bellTollersBattleshockReroll, &m_bellTollerSlot);

        m_weapon = weapon;

        // Add the Plaguebringer
        auto leader = new Model(g_basesize, wounds());
        leader->addMissileWeapon(&m_deathsHead);
        leader->addMeleeWeapon(&m_plagueswordPlaguebringer);
        if (weapon == Prehensile_Proboscis) {
            leader->addMeleeWeapon(&m_proboscis);
        } else if (weapon == Foul_Mouthparts) {
            leader->addMeleeWeapon(&m_mouthparts);
        }
        leader->addMeleeWeapon(&m_venemousSting);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_deathsHead);
            model->addMeleeWeapon(&m_plaguesword);
            if (weapon == Prehensile_Proboscis) {
                model->addMeleeWeapon(&m_proboscis);
            } else if (weapon == Foul_Mouthparts) {
                model->addMeleeWeapon(&m_mouthparts);
            }
            model->addMeleeWeapon(&m_venemousSting);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (bellTollers) {
                model->setName("Bell Tollers");
                bellTollers = false;
            }

            addModel(model);
        }
    }

    PlagueDrones::~PlagueDrones() {
        m_bellTollerSlot.disconnect();
    }

    Unit *PlagueDrones::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Prehensile_Proboscis);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool bells = GetBoolParam("Bell Tollers", parameters, false);
        return new PlagueDrones(legion, numModels, weapons, iconBearer, bells, ComputePoints(parameters));
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
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Plague Drones", factoryMethod);
        }
    }

    Wounds PlagueDrones::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    void PlagueDrones::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        NurgleBase::computeBattleshockEffect(roll, numFled, numAdded);
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

    int PlagueDrones::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int PlagueDrones::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = NurgleBase::extraAttacks(attackingModel, weapon, target);

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
            if (distanceTo(unit) <= 6.0) return Rerolls::Ones;
        }
        return Rerolls::Ones;
    }

} // namespace Nurgle


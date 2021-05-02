/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/WitchAelves.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool WitchAelves::s_registered = false;

    WitchAelves::WitchAelves(Temple temple, int numModels, bool pairedKnives, bool hornblowers, bool standardBearers, int points) :
            DaughterOfKhaine(temple, "Witch Aelves", 6, g_wounds, 7, 6, false, points),
            m_sacrificialKnife(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 3, 4, 0, 1),
            m_sacrificialKnifeHag(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 2, 4, 0, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, WITCH_AELVES};
        m_weapons = {&m_sacrificialKnife, &m_sacrificialKnifeHag};
        m_battleFieldRole = Role::Battleline;

        m_pairedKnives = pairedKnives;
        m_runAndCharge = hornblowers;

        auto hag = new Model(g_basesize, wounds());
        hag->addMeleeWeapon(&m_sacrificialKnifeHag);
        addModel(hag);

        for (auto i = 1; i < numModels; i++) {
            auto witch = new Model(g_basesize, wounds());
            witch->addMeleeWeapon(&m_sacrificialKnife);
            if (standardBearers) {
                witch->setName(Model::StandardBearer);
                standardBearers = false;
            } else if (hornblowers) {
                witch->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(witch);
        }
    }

    Unit *WitchAelves::Create(const ParameterList &parameters) {
        auto temple = (Temple) GetEnumParam("Temple", parameters, g_temple[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool pairedKnives = GetBoolParam("Paired Knives", parameters, true);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        return new WitchAelves(temple, numModels, pairedKnives, hornblowers, standardBearers, ComputePoints(parameters));
    }

    void WitchAelves::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Paired Knives"),
                            BoolParameter("Hornblowers"),
                            BoolParameter("Standard Bearers"),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Witch Aelves", factoryMethod);
        }
    }

    int WitchAelves::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Paired Sacrificial Knives
        int attacks = DaughterOfKhaine::extraAttacks(attackingModel, weapon, target);
        if (m_pairedKnives) attacks++;

        // Frenzied Fervour
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 8.0);
        for (auto unit : units) {
            if (unit->hasKeyword(DAUGHTERS_OF_KHAINE) && unit->hasKeyword(HERO)) {
                attacks++;
                break;
            }
        }
        return attacks;
    }

    int WitchAelves::rollBattleshock() const {
        if (isNamedModelAlive(Model::StandardBearer)) {
            int r1 = Dice::RollD6();
            int r2 = Dice::RollD6();
            return std::min(r1, r2);
        }
        return DaughterOfKhaine::rollBattleshock();
    }

    int WitchAelves::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = DaughterOfKhaine::toSaveModifier(weapon, attacker);
        // Bladed Bucklers
        if (!m_pairedKnives) {
            modifier += 1;
        }
        return modifier;
    }

    Wounds WitchAelves::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = DaughterOfKhaine::computeReturnedDamage(weapon, saveRoll);
        // Bladed Bucklers
        if (!weapon->isMissile()) {
            // 1 mortal wound for each save of a 6
            wounds += {0, 1};
        }
        return wounds;
    }

    int WitchAelves::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace DaughtersOfKhaine

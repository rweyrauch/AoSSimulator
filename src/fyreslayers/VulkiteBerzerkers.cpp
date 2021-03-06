/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/VulkiteBerzerkers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool VulkiteBerzerkers::s_registered = false;

    VulkiteBerzerkers::VulkiteBerzerkers(Lodge lodge, int numModels, WeaponOption weapons, bool hornOfGrimnir, int points) :
            Fyreslayer(lodge, "Vulkite Berzerkers", 4, g_wounds, 7, 5, false, points),
            m_handaxe(Weapon::Type::Melee, "Fyresteel Handaxe", 1, 2, 3, 3, 0, 1),
            m_handaxeKarl(Weapon::Type::Melee, "Fyresteel Handaxe", 1, 3, 3, 3, 0, 1),
            m_warpick(Weapon::Type::Melee, "Fyresteel War-pick", 1, 2, 3, 4, -1, 1),
            m_warpickKarl(Weapon::Type::Melee, "Fyresteel War-pick", 1, 3, 3, 4, -1, 1),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, VULKITE_BERZERKERS};
        m_weapons = {&m_handaxe, &m_handaxeKarl, &m_warpick, &m_warpickKarl, &m_throwingAxe};
        m_battleFieldRole = Role::Battleline;

        m_weaponOption = weapons;
        m_hornOfGrimnir = hornOfGrimnir;

        auto karl = new Model(g_basesize, wounds());
        karl->addMissileWeapon(&m_throwingAxe);
        if (weapons == Handaxe_And_Shield || weapons == Paired_Handaxes) {
            karl->addMeleeWeapon(&m_handaxeKarl);
        } else if (weapons == Warpick_And_Shield) {
            karl->addMeleeWeapon(&m_warpickKarl);
        }
        addModel(karl);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_throwingAxe);
            if (weapons == Handaxe_And_Shield || weapons == Paired_Handaxes) {
                model->addMeleeWeapon(&m_handaxe);
            } else if (weapons == Warpick_And_Shield) {
                model->addMeleeWeapon(&m_warpick);
            }
            addModel(model);
        }
    }

    Unit *VulkiteBerzerkers::Create(const ParameterList &parameters) {
        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Handaxe_And_Shield);
        auto horn = GetBoolParam("Horn of Grimnir", parameters, false);
        return new VulkiteBerzerkers(lodge, numModels, weapons, horn, ComputePoints(parameters));
    }

    void VulkiteBerzerkers::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Handaxe_And_Shield, Warpick_And_Shield, Paired_Handaxes};
            static FactoryMethod factoryMethod = {
                    VulkiteBerzerkers::Create,
                    VulkiteBerzerkers::ValueToString,
                    VulkiteBerzerkers::EnumStringToInt,
                    VulkiteBerzerkers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Handaxe_And_Shield, weapons),
                            BoolParameter("Horn of Grimnir"),
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Vulkite Berzerkers", factoryMethod);
        }
    }

    std::string VulkiteBerzerkers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Handaxe_And_Shield) { return "Fyresteel Handaxe and Bladed Slingshield"; }
            else if (parameter.intValue == Warpick_And_Shield) { return "Fyresteel War-pick and Bladed Slingshield"; }
            else if (parameter.intValue == Paired_Handaxes) { return "Paired Fyresteel Handaxes"; }
        }
        return Fyreslayer::ValueToString(parameter);
    }

    int VulkiteBerzerkers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Fyresteel Handaxe and Bladed Slingshield") { return Handaxe_And_Shield; }
        else if (enumString == "Fyresteel War-pick and Bladed Slingshield") { return Warpick_And_Shield; }
        else if (enumString == "Paired Fyresteel Handaxes") { return Paired_Handaxes; }
        return Fyreslayer::EnumStringToInt(enumString);
    }

    Rerolls VulkiteBerzerkers::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if ((weapon->name() == m_handaxe.name()) && (m_weaponOption == Paired_Handaxes)) {
            return Rerolls::Failed;
        }
        return Fyreslayer::toHitRerolls(weapon, target);
    }

    int VulkiteBerzerkers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int VulkiteBerzerkers::chargeModifier() const {
        auto mod = Fyreslayer::chargeModifier();

        if (m_hornOfGrimnir) mod++;

        return mod;
    }

    void VulkiteBerzerkers::onRestore() {
        Fyreslayer::onRestore();
        m_usedBerserkFury = false;
        m_berserkFuryActive = false;
    }

    void VulkiteBerzerkers::onStartCombat(PlayerId player) {
        Fyreslayer::onStartCombat(player);

        if (!m_usedBerserkFury) {
            if (meleeTarget() && (distanceTo(meleeTarget()) < 3.0)) {
                m_usedBerserkFury = true;
                m_berserkFuryActive = true;
            }
        }
    }

    void VulkiteBerzerkers::onEndCombat(PlayerId player) {
        Fyreslayer::onEndCombat(player);

        m_berserkFuryActive = false;
    }

    void VulkiteBerzerkers::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Fyreslayer::onFriendlyModelSlain(numSlain, attacker, source);

        // Berserk Fury
        if (m_berserkFuryActive && (numSlain > 0)) {
            doPileIn();
            int enemiesSlain = 0;
            fight(numSlain, attacker, enemiesSlain);
        }
    }

    void VulkiteBerzerkers::onCharged() {
        Fyreslayer::onCharged();

        // Bladed Slingshield
        if ((m_weaponOption == Handaxe_And_Shield) || (m_weaponOption == Warpick_And_Shield)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 8.0);
            for (auto unit : units) {
                if (unit->remainingModels() > 0) {
                    Dice::RollResult rolls;
                    Dice::RollD6(remainingModels(), rolls);
                    unit->applyDamage({0, rolls.rollsGE(6), Wounds::Source::Ability}, this);
                }
            }
        }
    }

} //namespace Fyreslayers
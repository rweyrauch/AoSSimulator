/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestantOnStardrake.h>
#include <iostream>
#include <Board.h>
#include <Roster.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 170; // x105 oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 500;

    struct TableEntry {
        int m_move;
        int m_greatClawsToHit;
        int m_cavernousJawsBits;
    };

    const size_t g_numTableEntries = 4;
    static int g_woundThresholds[g_numTableEntries] = {4, 8, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 3, 3},
                    {11, 3, 2},
                    {10, 4, 2},
                    {8,  4, 1}
            };

    bool LordCelestantOnStardrake::s_registered = false;

    LordCelestantOnStardrake::LordCelestantOnStardrake() :
            StormcastEternal("Lord-Celestant on Stardrake", 12, g_wounds, 9, 3, true),
            m_celestineHammer(Weapon::Type::Melee, "Celestine Hammer", 2, 3, 3, 2, -1, RAND_D3),
            m_stormboundBlade(Weapon::Type::Melee, "Stormbound Blade", 2, 3, 3, 4, -1, 2),
            m_greatClaws(Weapon::Type::Melee, "Great Claws", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STARDRAKE, STORMCAST_ETERNAL, HERO, MONSTER, LORD_CELESTANT};
        m_weapons = {&m_celestineHammer, &m_stormboundBlade, &m_greatClaws};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;

        s_globalCastMod.connect(this, &LordCelestantOnStardrake::arcaneLineage, &m_connection);
    }

    LordCelestantOnStardrake::~LordCelestantOnStardrake() {
        m_connection.disconnect();
    }

    bool LordCelestantOnStardrake::configure(WeaponOption weapons) {
        m_weaponOption = weapons;

        auto model = new Model(g_basesize, wounds());
        if (weapons == Celestine_Hammer) {
            model->addMeleeWeapon(&m_celestineHammer);
        } else if (weapons == Stormbound_Blade) {
            model->addMeleeWeapon(&m_stormboundBlade);
        }
        model->addMeleeWeapon(&m_greatClaws);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *LordCelestantOnStardrake::Create(const ParameterList &parameters) {
        auto unit = new LordCelestantOnStardrake();
        auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, Celestine_Hammer);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordCelestantOnStardrake::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Celestine_Hammer, Stormbound_Blade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Celestine_Hammer, weapons),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Celestant on Stardrake", factoryMethod);
        }
    }

    std::string LordCelestantOnStardrake::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Celestine_Hammer) {
                return "Celestine Hammer";
            } else if (parameter.intValue == Stormbound_Blade) {
                return "Stormbound Blade";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }


    void LordCelestantOnStardrake::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_greatClaws.setToHit(g_damageTable[damageIndex].m_greatClawsToHit);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void LordCelestantOnStardrake::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int LordCelestantOnStardrake::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int LordCelestantOnStardrake::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                               const Unit *target) const {
        int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);

        // Inescapable Vengeance
        if (m_charged) {
            attacks += Dice::RollD3();
        }
        return attacks;
    }

    int LordCelestantOnStardrake::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Stormbound Blade
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_stormboundBlade.name())) {
            // 3 hits on an unmodified 6
            return 3;
        }
        return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Rerolls LordCelestantOnStardrake::toSaveRerolls(const Weapon * /*weapon*/) const {
        // Sigmarite Thundershield
        return Reroll_Ones;
    }

    Wounds LordCelestantOnStardrake::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = StormcastEternal::computeReturnedDamage(weapon, saveRoll);
        // Sigmarite Thundershield
        // 1 mortal wound for each save of a 6
        if (saveRoll == 6) {
            wounds += {0, 1};
        }
        return wounds;
    }

    void LordCelestantOnStardrake::onStartCombat(PlayerId player) {
        StormcastEternal::onStartCombat(player);

        // Cavernous Jaws
        if (m_meleeTarget) {
            auto numBites = g_damageTable[getDamageTableIndex()].m_cavernousJawsBits;
            int numToSlay = 0;
            for (auto i = 0; i < numBites; i++) {
                int roll = Dice::RollD6();
                if (roll > m_meleeTarget->wounds()) {
                    numToSlay++;
                }
            }
            m_meleeTarget->slay(numToSlay);
        }
    }

    Wounds LordCelestantOnStardrake::onEndCombat(PlayerId player) {
        auto wounds = StormcastEternal::onEndCombat(player);

        // Sweeping Tail
        {
            auto board = Board::Instance();

            PlayerId otherPlayer = PlayerId::Red;
            if (player == PlayerId::Red) {
                otherPlayer = PlayerId::Blue;
            }
            auto otherRoster = board->getPlayerRoster(otherPlayer);

            // find all enemy units within 3"
            for (auto ip = otherRoster->unitBegin(); ip != otherRoster->unitEnd(); ++ip) {
                auto dist = distanceTo(*ip);
                if (dist <= 3.0) {
                    auto roll = Dice::RollD6();
                    if (roll < (*ip)->remainingModels()) {
                        // inflict D3 mortal wounds
                        roll = Dice::RollD3();
                        Wounds mortalWounds = {0, roll};
                        (*ip)->applyDamage(mortalWounds);
                        wounds += mortalWounds;
                    }
                }
            }
        }
        return wounds;
    }

    void LordCelestantOnStardrake::onStartShooting(PlayerId player) {
        StormcastEternal::onStartShooting(player);

        // Lord of the Heavens
        // Decide: 'Roiling Thunder' or 'Rain of Stars'?
        bool preferRainOfStars = true;
        if (m_shootingTarget) {
            auto range = distanceTo(m_shootingTarget);
            if (range <= 18.0) {
                // Roiling Thunder
                preferRainOfStars = false;
            }
        }

        if (preferRainOfStars) {
            auto board = Board::Instance();

            PlayerId otherPlayer = PlayerId::Red;
            if (player == PlayerId::Red) {
                otherPlayer = PlayerId::Blue;
            }
            auto otherRoster = board->getPlayerRoster(otherPlayer);
            auto numUnits = Dice::RollD6();

            int unitsAffected = 0;
            for (auto ip = otherRoster->unitBegin(); ip != otherRoster->unitEnd(); ++ip) {
                int roll = Dice::RollD6();
                if (roll >= 4) {
                    Wounds wounds = {0, Dice::RollD3()};
                    (*ip)->applyDamage(wounds);
                }
                unitsAffected++;

                if (unitsAffected > numUnits) { break; }
            }
        } else // Roiling Thunder
        {
            auto numModels = m_shootingTarget->remainingModels();
            Dice::RollResult rolls;
            Dice::RollD6(numModels, rolls);
            int mortalWounds = rolls.numUnmodified6s();
            Wounds wounds = {0, mortalWounds};
            m_shootingTarget->applyDamage(wounds);
        }
    }

    int LordCelestantOnStardrake::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Celestine Hammer") {
            return Celestine_Hammer;
        } else if (enumString == "Stormbound Blade") {
            return Stormbound_Blade;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int LordCelestantOnStardrake::arcaneLineage(const Unit *target) {
        if (target->hasKeyword(WIZARD) && (distanceTo(target) <= 18.0)) {
            if (target->owningPlayer() != owningPlayer()) {
                // Enemy
                return -1;
            } else {
                // Friendly
                return 1;
            }
        }
        return 0;
    }

    int LordCelestantOnStardrake::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals
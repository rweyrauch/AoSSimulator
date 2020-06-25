/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/DrakeswornTemplar.h>
#include <iostream>
#include <Board.h>
#include <Roster.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 420;

    struct TableEntry {
        int m_move;
        int m_greatClawsToHit;
        int m_cavernousJawsBits;
    };

    const size_t NUM_TABLE_ENTRIES = 4;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 8, 12, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 3, 3},
                    {11, 3, 2},
                    {10, 4, 2},
                    {8,  4, 1}
            };

    bool DrakeswornTemplar::s_registered = false;

    DrakeswornTemplar::DrakeswornTemplar() :
            StormcastEternal("Drakesworn Templar", 12, WOUNDS, 9, 3, true),
            m_skyboltBow(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 3, 3, -1, 1),
            m_tempestAxe(Weapon::Type::Melee, "Tempest Axe", 2, 6, 3, 3, 0, 1),
            m_arcHammer(Weapon::Type::Melee, "Arc Hammer", 1, 2, 3, 3, -1, 3),
            m_stormlance(Weapon::Type::Melee, "Stormlance", 3, 3, 3, 3, -1, 2),
            m_greatClaws(Weapon::Type::Melee, "Great Claws", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STARDRAKE, STORMCAST_ETERNAL, MONSTER, HERO, DRAKESWORN_TEMPLAR};
        m_weapons = {&m_skyboltBow, &m_tempestAxe, &m_arcHammer, &m_stormlance, &m_greatClaws};
        m_hasMount = true;

        s_globalCastMod.connect(this, &DrakeswornTemplar::arcaneLineage, &m_connection);
    }

    DrakeswornTemplar::~DrakeswornTemplar() {
        m_connection.disconnect();
    }

    bool DrakeswornTemplar::configure(WeaponOption weapons, bool skyboltBow) {
        m_weaponOption = weapons;

        auto model = new Model(BASESIZE, wounds());
        if (skyboltBow) {
            model->addMissileWeapon(&m_skyboltBow);
        }
        if (weapons == TempestAxe) {
            model->addMeleeWeapon(&m_tempestAxe);
        } else if (weapons == ArcHammer) {
            model->addMeleeWeapon(&m_arcHammer);
        } else if (weapons == Stormlance) {
            model->addMeleeWeapon(&m_stormlance);
        }
        model->addMeleeWeapon(&m_greatClaws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *DrakeswornTemplar::Create(const ParameterList &parameters) {
        auto unit = new DrakeswornTemplar();
        auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, TempestAxe);
        auto skyboltBow = GetBoolParam("Skybolt Bow", parameters, true);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapons, skyboltBow);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DrakeswornTemplar::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {TempestAxe, ArcHammer, Stormlance};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", TempestAxe, weapons),
                            BoolParameter("Skybolt Bow"),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Drakesworn Templar", factoryMethod);
        }
    }

    std::string DrakeswornTemplar::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == TempestAxe) {
                return "Tempest Axe";
            } else if (parameter.intValue == ArcHammer) {
                return "Arc Hammer";
            } else if (parameter.intValue == Stormlance) {
                return "Stormlance";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }


    void DrakeswornTemplar::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_greatClaws.setToHit(g_damageTable[damageIndex].m_greatClawsToHit);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }


    void DrakeswornTemplar::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int DrakeswornTemplar::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void DrakeswornTemplar::onStartCombat(PlayerId player) {
        StormcastEternal::onStartCombat(player);

        // Cavernous Jaws
        if (m_meleeTarget) {
            auto numBites = g_damageTable[getDamageTableIndex()].m_cavernousJawsBits;
            int numToSlay = 0;
            for (auto i = 0; i < numBites; i++) {
                int roll = Dice::rollD6();
                if (roll > m_meleeTarget->wounds()) {
                    numToSlay++;
                }
            }
            m_meleeTarget->slay(numToSlay);
        }
    }

    Wounds DrakeswornTemplar::onEndCombat(PlayerId player) {
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
                    auto roll = Dice::rollD6();
                    if (roll < (*ip)->remainingModels()) {
                        // inflict D3 mortal wounds
                        roll = Dice::rollD3();
                        Wounds mortalWounds = {0, roll};
                        (*ip)->applyDamage(mortalWounds);

                        wounds += mortalWounds;
                    }
                }
            }
        }
        return wounds;
    }

    void DrakeswornTemplar::onStartShooting(PlayerId player) {
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
            auto numUnits = Dice::rollD6();

            int unitsAffected = 0;
            for (auto ip = otherRoster->unitBegin(); ip != otherRoster->unitEnd(); ++ip) {
                int roll = Dice::rollD6();
                if (roll >= 4) {
                    Wounds wounds = {0, Dice::rollD3()};
                    (*ip)->applyDamage(wounds);
                }
                unitsAffected++;

                if (unitsAffected > numUnits) { break; }
            }
        } else // Roiling Thunder
        {
            auto numModels = m_shootingTarget->remainingModels();
            Dice::RollResult rolls;
            Dice::rollD6(numModels, rolls);
            int mortalWounds = rolls.numUnmodified6s();
            Wounds wounds = {0, mortalWounds};
            m_shootingTarget->applyDamage(wounds);
        }
    }

    int DrakeswornTemplar::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Tempest Axe") {
            return TempestAxe;
        } else if (enumString == "Arc Hammer") {
            return ArcHammer;
        } else if (enumString == "Stormlance") {
            return Stormlance;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int DrakeswornTemplar::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_arcHammer.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Wounds DrakeswornTemplar::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((hitRoll == 6) && (weapon->name() == m_stormlance.name()) && (target->hasKeyword(MONSTER))) {
            return {0, Dice::rollD6()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int DrakeswornTemplar::arcaneLineage(const Unit *target) {
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

    int DrakeswornTemplar::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals
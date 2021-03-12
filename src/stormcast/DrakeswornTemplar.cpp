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
    static const int g_basesize = 170; // x105 oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 420;

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

    bool DrakeswornTemplar::s_registered = false;

    DrakeswornTemplar::DrakeswornTemplar() :
            MountedStormcastEternal("Drakesworn Templar", 12, g_wounds, 9, 3, true),
            m_skyboltBow(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 3, 3, -1, 1),
            m_tempestAxe(Weapon::Type::Melee, "Tempest Axe", 2, 6, 3, 3, 0, 1),
            m_arcHammer(Weapon::Type::Melee, "Arc Hammer", 1, 2, 3, 3, -1, 3),
            m_stormlance(Weapon::Type::Melee, "Stormlance", 3, 3, 3, 3, -1, 2),
            m_greatClaws(Weapon::Type::Melee, "Great Claws", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STARDRAKE, STORMCAST_ETERNAL, MONSTER, HERO, DRAKESWORN_TEMPLAR};
        m_weapons = {&m_skyboltBow, &m_tempestAxe, &m_arcHammer, &m_stormlance, &m_greatClaws};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_greatClaws.setMount(true);

        s_globalCastMod.connect(this, &DrakeswornTemplar::arcaneLineage, &m_connection);
    }

    DrakeswornTemplar::~DrakeswornTemplar() {
        m_connection.disconnect();
    }

    bool DrakeswornTemplar::configure(WeaponOption weapons, bool skyboltBow, MountTrait trait) {
        m_weaponOption = weapons;
        m_mountTrait = trait;

        auto model = new Model(g_basesize, wounds());
        if (skyboltBow) {
            model->addMissileWeapon(&m_skyboltBow);
        }
        if (weapons == Tempest_Axe) {
            model->addMeleeWeapon(&m_tempestAxe);
        } else if (weapons == Arc_Hammer) {
            model->addMeleeWeapon(&m_arcHammer);
        } else if (weapons == Stormlance) {
            model->addMeleeWeapon(&m_stormlance);
        }
        model->addMeleeWeapon(&m_greatClaws);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *DrakeswornTemplar::Create(const ParameterList &parameters) {
        auto unit = new DrakeswornTemplar();
        auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, Tempest_Axe);
        auto skyboltBow = GetBoolParam("Skybolt Bow", parameters, true);
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int)MountTrait::None);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapons, skyboltBow, trait);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DrakeswornTemplar::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Tempest_Axe, Arc_Hammer, Stormlance};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Tempest_Axe, weapons),
                            BoolParameter("Skybolt Bow"),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Mount Trait", g_stardrakeMountTrait[0], g_stardrakeMountTrait),
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
            if (parameter.intValue == Tempest_Axe) {
                return "Tempest Axe";
            } else if (parameter.intValue == Arc_Hammer) {
                return "Arc Hammer";
            } else if (parameter.intValue == Stormlance) {
                return "Stormlance";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }


    void DrakeswornTemplar::onWounded() {
        MountedStormcastEternal::onWounded();
        const int damageIndex = getDamageTableIndex();
        m_greatClaws.setToHit(g_damageTable[damageIndex].m_greatClawsToHit);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }


    void DrakeswornTemplar::onRestore() {
        MountedStormcastEternal::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    int DrakeswornTemplar::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        if (m_mountTrait == MountTrait::Star_Branded) woundsInflicted--;

        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void DrakeswornTemplar::onStartCombat(PlayerId player) {
        MountedStormcastEternal::onStartCombat(player);

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

    void DrakeswornTemplar::onEndCombat(PlayerId player) {
        MountedStormcastEternal::onEndCombat(player);

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
                auto dist = distanceTo(ip->get());
                if (dist <= 3.0) {
                    auto roll = Dice::RollD6();
                    if (roll < (*ip)->remainingModels()) {
                        // inflict D3 mortal wounds
                        roll = Dice::RollD3();
                        Wounds mortalWounds = {0, roll};
                        (*ip)->applyDamage(mortalWounds, this);
                    }
                }
            }
        }
    }

    void DrakeswornTemplar::onStartShooting(PlayerId player) {
        MountedStormcastEternal::onStartShooting(player);

        // Lord of the Heavens
        // Decide: 'Roiling Thunder' or 'Rain of Stars'?
        bool preferRainOfStars = true;
        if (m_shootingTarget) {
            auto range = distanceTo(m_shootingTarget);
            const auto roilingThunderRange = (m_mountTrait == MountTrait::Thunderlord) ? 24.0 : 18.0;
            if (range <= roilingThunderRange) {
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
                    (*ip)->applyDamage(wounds, this);
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
            m_shootingTarget->applyDamage(wounds, this);
        }
    }

    int DrakeswornTemplar::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Tempest Axe") {
            return Tempest_Axe;
        } else if (enumString == "Arc Hammer") {
            return Arc_Hammer;
        } else if (enumString == "Stormlance") {
            return Stormlance;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int DrakeswornTemplar::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_arcHammer.name())) {
            return 2;
        }
        return MountedStormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Wounds DrakeswornTemplar::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((hitRoll == 6) && (weapon->name() == m_stormlance.name()) && (target->hasKeyword(MONSTER))) {
            return {0, Dice::RollD6()};
        }
        return MountedStormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
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
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals
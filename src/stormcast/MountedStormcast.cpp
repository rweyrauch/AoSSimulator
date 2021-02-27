/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/MountedStormcast.h>
#include <Board.h>
#include <Roster.h>

namespace StormcastEternals {

    MountedStormcastEternal::MountedStormcastEternal(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
            StormcastEternal(name, move, wounds, bravery, save, fly) {
        s_globalToHitMod.connect(this, &MountedStormcastEternal::prideLeader, &m_prideLeaderConnection);
    }

    MountedStormcastEternal::~MountedStormcastEternal() {
        m_prideLeaderConnection.disconnect();
    }

    int MountedStormcastEternal::moveModifier() const {
        auto mod = StormcastEternal::moveModifier();
        if (m_mountTrait == MountTrait::Lithe_Limbed) mod++;
        return mod;
    }

    int MountedStormcastEternal::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((m_mountTrait == MountTrait::Keen_Clawed) && (weapon->isMount()) && (woundRoll == 6)) {
            return -3;
        }
        return StormcastEternal::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    void MountedStormcastEternal::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        StormcastEternal::onFriendlyModelSlain(numSlain, attacker, source);

        if ((source == Wounds::Source::Weapon_Melee) && (m_mountTrait == MountTrait::Savage_Loyalty)) {
            if (Dice::RollD6() >= 4) {
                attacker->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
            }
        }
    }

    int
    MountedStormcastEternal::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);

        if ((m_mountTrait == MountTrait::Pack_Leader) && (weapon->name() == "Claws and Fangs")) {
            auto guard = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), DRACOTHIAN_GUARD, 6.0);
            if ((guard != nullptr) && (guard->remainingModels() > 0)) {
                attacks += 2;
            }
        }
        return attacks;
    }

    Rerolls MountedStormcastEternal::runRerolls() const {
        if (m_mountTrait == MountTrait::Indefatigable) return Reroll_Failed;
        if (m_mountTrait == MountTrait::Swiftwing) return Reroll_Failed;
        return StormcastEternal::runRerolls();
    }

    int MountedStormcastEternal::prideLeader(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (m_mountTrait == MountTrait::Pride_Leader) {
            if (distanceTo(attacker) < 9.0) return 1;
        }
        return 0;
    }

    void MountedStormcastEternal::onEndCombat(PlayerId player) {
        StormcastEternal::onEndCombat(player);

        if (m_mountTrait == MountTrait::Lashing_Tail) {
            if ((meleeTarget() != nullptr) && (distanceTo(meleeTarget()) < 3.0)) {
                if (Dice::RollD6() >= 4) {
                    meleeTarget()->applyDamage({0, 1, Wounds::Source::Ability}, this);
                }
            }
        }
    }

    Wounds MountedStormcastEternal::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if (m_mountTrait == MountTrait::Steel_Pinions) {
            totalWounds = ignoreWounds(totalWounds, 6);
        }
        return Unit::applyWoundSave(totalWounds, attackingUnit);
    }

    void MountedStormcastEternal::onStartCombat(PlayerId player) {
        StormcastEternal::onStartCombat(player);

        if (m_mountTrait == MountTrait::Ear_Bursting_Roar) {
            auto enemy = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if ((enemy != nullptr) && (enemy->remainingModels() > 0) && (distanceTo(enemy) < 3.0)) {
                if (Dice::RollD6() >= 4) {
                    meleeTarget()->applyDamage({0, 1, Wounds::Source::Ability}, this);
                }
            }
        }
    }

} // namespace StormcastEternals

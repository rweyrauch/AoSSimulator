/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <utility>
#include <Weapon.h>
#include <AgeOfSigmarSim.h>
#include <Dice.h>

Weapon::Weapon(Type type, std::string name, int range, int attacks, int toHit, int toWound, int rend,
               int damage) noexcept :
        m_type(type),
        m_name(std::move(name)),
        m_range(range),
        m_attacks(attacks),
        m_toHit(toHit),
        m_toWound(toWound),
        m_rend(rend),
        m_damage(damage),
        m_hitsPerAttack(1),
        m_isActive(true) {
}

Weapon::Weapon(const Weapon &w) :
        m_type(w.m_type),
        m_name(w.m_name),
        m_range(w.m_range),
        m_attacks(w.m_attacks),
        m_toHit(w.m_toHit),
        m_toWound(w.m_toWound),
        m_rend(w.m_rend),
        m_damage(w.m_damage),
        m_hitsPerAttack(w.m_hitsPerAttack),
        m_isActive(w.m_isActive) {
}

Hits Weapon::rollToHit(int modifier, Rerolls rerolls, int extraAttacks) const {
    Dice::RollResult rollResult;

    const int totalAttacks = numAttacks(extraAttacks);

    const int toHit = m_toHit - modifier;

    int numHits = 0;
    if (rerolls == Rerolls::Ones) {
        Dice::RollD6(totalAttacks, 1, rollResult);
        numHits = rollResult.rollsGE(toHit);
    } else if (rerolls == Rerolls::Ones_And_Twos) {
        Dice::RollD6(totalAttacks, 2, rollResult);
        numHits = rollResult.rollsGE(toHit);
    } else if (rerolls == Rerolls::Failed) {
        Dice::RollD6(totalAttacks, rollResult);
        numHits = rollResult.rollsGE(toHit);
        int numFails = totalAttacks - numHits;
        if (numFails > 0) {
            rollResult.clearLT(toHit);
            Dice::RollResult rerollResult;
            Dice::RollD6(numFails, rerollResult);
            auto numRerolledHits = rerollResult.rollsGE(toHit);
            numHits += numRerolledHits;
            // merge roll results from rerolls into a single result.
            rollResult.add(rerollResult);
        }
    } else {
        Dice::RollD6(totalAttacks, rollResult);
        numHits = rollResult.rollsGE(toHit);
    }
    return {numHits, rollResult};
}

WoundingHits Weapon::rollToWound(int numHits, int modifier, Rerolls rerolls) const {
    Dice::RollResult rollResult;

    int totalHits = numHits;
    if (m_hitsPerAttack < 0) {
        totalHits = 0;
        for (int i = 0; i < numHits; i++) {
            totalHits += numTotalHits();
        }
    }
    const int toWound = m_toWound - modifier;

    int numWoundingHits;

    if (rerolls == Rerolls::Ones) {
        Dice::RollD6(totalHits, 1, rollResult);
        numWoundingHits = rollResult.rollsGE(toWound);
    } else if (rerolls == Rerolls::Ones_And_Twos) {
        Dice::RollD6(totalHits, 2, rollResult);
        numWoundingHits = rollResult.rollsGE(toWound);
    } else if (rerolls == Rerolls::Failed) {
        Dice::RollD6(totalHits, rollResult);
        numWoundingHits = rollResult.rollsGE(toWound);
        int numFails = totalHits - numWoundingHits;
        if (numFails > 0) {
            rollResult.clearLT(toWound);
            Dice::RollResult rerollResult;
            Dice::RollD6(numFails, rerollResult);
            auto numRerolledHits = rerollResult.rollsGE(toWound);
            numWoundingHits += numRerolledHits;
            // merge roll results from rerolls into a single result.
            rollResult.add(rerollResult);
        }
    } else {
        Dice::RollD6(totalHits, rollResult);
        numWoundingHits = rollResult.rollsGE(toWound);
    }

    return {numWoundingHits, rollResult};
}

int Weapon::numAttacks(int extraAttacks) const {
    return Dice::RollSpecial(m_attacks) + extraAttacks;
}

int Weapon::damage() const {
    return Dice::RollSpecial(m_damage);
}

int Weapon::numTotalHits() const {
    return Dice::RollSpecial(m_hitsPerAttack);
}

double Weapon::strength() const {
    if (m_strength < 0.0) {
        const auto effDamage = AverageRandomValue(m_damage);
        const auto effAttacks = AverageRandomValue(m_attacks);
        const auto effHitsPerAttack = AverageRandomValue(m_hitsPerAttack);
        int toSave4 = std::max(1, std::min(6, 4 + m_rend));
        double rawStrength =
                ((double) (6 - m_toHit) / 6.0) * ((double) (6 - m_toWound) / 6.0) * ((double) toSave4 / 6.0) *
                effDamage * effAttacks * effHitsPerAttack;
        rawStrength *= 100.0;
        rawStrength *= log((double) m_range / 6.0 + 3.0);
        m_strength = rawStrength;
    }

    return m_strength;
}

double Weapon::averageDamage(int /*againstSave*/) const {
    return AverageRandomValue(damage());
}

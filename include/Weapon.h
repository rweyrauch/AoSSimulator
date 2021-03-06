/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <string>
#include <AgeOfSigmarSim.h>
#include <Dice.h>
#include <bitset>

struct Hits {
    Hits(int ta, int n, const Dice::RollResult &r) : totalAttacks(ta), numHits(n), rolls(r) {}

    int totalAttacks = 0;
    int numHits = 0;
    Dice::RollResult rolls{};
};

struct WoundingHits {
public:
    WoundingHits(int th, int w, const Dice::RollResult &r) : totalHits(th), numWoundingHit(w), rolls(r) {}

    int totalHits = 0;
    int numWoundingHit = 0;
    Dice::RollResult rolls{};
};

class Weapon {
public:

    enum class Type {
        Missile,
        Melee
    };

    enum FlagBit {
        Artefact = 0,
        Preferred,
        IsMount,
    };

    Weapon() = default;

    Weapon(Type type, std::string name, int range, int attacks, int toHit, int toWound, int rend,
           int damage) noexcept;

    Weapon(const Weapon &w);

    void setHitsPerAttack(int numHits) { m_hitsPerAttack = numHits; m_strength = -1.0; }

    int numAttacks(int extraAttacks) const;

    Hits rollToHit(int modifier, Rerolls rerolls, int extraAttacks) const;

    WoundingHits rollToWound(int numHits, int modifier, Rerolls rerolls) const;

    const std::string &name() const { return m_name; }

    const char *nameC() const { return m_name.c_str(); }

    int range() const { return m_range; }

    void setRange(int r) { m_range = r; m_strength = -1.0; }

    int attacks() const { return m_attacks; }

    void setAttacks(int a) { m_attacks = a; m_strength = -1.0; }

    int toHit() const { return m_toHit; }

    void setToHit(int h) { m_toHit = h; m_strength = -1.0; }

    int toWound() const { return m_toWound; }

    void setToWound(int w) { m_toWound = w; m_strength = -1.0; }

    int rend() const { return m_rend; }

    void setRend(int r) { m_rend = r; m_strength = -1.0; }

    int damage() const;

    void setDamage(int d) { m_damage = d; m_strength = -1.0; }

    void setMount(bool isMount) { m_flags[IsMount] = isMount; }

    bool isMount() const { return isFlagSet(IsMount); }

    bool isMissile() const { return (m_type == Type::Missile); }

    bool isMelee() const { return (m_type == Type::Melee); }

    void activate(bool isActive = true) { m_isActive = isActive; }

    bool isActive() const { return m_isActive; }

    void setFlag(FlagBit bit) const {
        m_flags.set(bit);
    }

    void clearFlag(FlagBit bit) {
        m_flags.reset(bit);
    }

    bool isFlagSet(FlagBit bit) const {
        return m_flags[bit];
    }

    // Returns/computes the weapon's relative 'strength' heuristic.  The larger the 'strength'
    // value the more powerful the weapon is.
    double strength() const;

    double averageDamage(int againstSave = 4) const;

protected:

    int numTotalHits() const;

private:
    Type m_type = Type::Melee;
    std::string m_name;
    int m_range = 1;
    int m_attacks = 1;
    int m_toHit = 4;
    int m_toWound = 4;
    int m_rend = 0;
    int m_damage = 1;

    int m_hitsPerAttack = 1;
    bool m_isActive = true;

    mutable std::bitset<64> m_flags;

    mutable double m_strength = -1.0;
};


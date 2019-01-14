/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <WarhammerSim.h>
#include <Dice.h>

struct Hits
{
    Hits(int n, const Dice::RollResult& r) : numHits(n), rolls(r) {}
    int numHits;
    Dice::RollResult rolls;
};

struct WoundingHits
{
public:
    WoundingHits(int w, const Dice::RollResult& r) : numWoundingHit(w), rolls(r) {}
    int numWoundingHit;
    Dice::RollResult rolls;
};

class Weapon
{
public:

    enum class Type
    {
        Missile,
        Melee
    };

    Weapon() = default;
    Weapon(Type type, const std::string& name, int range, int attacks, int toHit, int toWound, int rend, int damage);
    Weapon(const Weapon& w);

    void setHitsPerAttack(int numHits) { m_hitsPerAttack = numHits; }

    Hits rollToHit(int modifier, Rerolls rerolls, int extraAttacks) const;
    WoundingHits rollToWound(int numHits, int modifier, Rerolls rerolls) const;

    const std::string& name() const { return m_name; }
    int range() const { return m_range; }
    void setRange(int r) { m_range = r; }
    int attacks() const { return m_attacks; }
    void setAttacks(int a) { m_attacks = a; }
    int toHit() const { return m_toHit; }
    void setToHit(int h) { m_toHit = h; }
    int toWound() const { return m_toWound; }
    void setToWound(int w) { m_toWound = w; }
    int rend() const { return m_rend; }
    void setRend(int r) { m_rend = r; }
    int damage() const;
    void setDamage(int d) { m_damage = d; }
    bool isMissile() const { return (m_type == Type::Missile); }

protected:

    int numAttacks(int extraAttacks) const;
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
};

#endif //WEAPON_H

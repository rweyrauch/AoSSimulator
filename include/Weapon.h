/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_WEAPON_H
#define WARHAMMERSIM_WEAPON_H

#include <string>
#include <WarhammerSim.h>

class Weapon
{
public:
    Weapon() = default;
    Weapon(const std::string& name, int range, int attacks, int toHit, int toWound, int rend, int damage);

    void setHitsPerAttack(int numHits) { m_hitsPerAttack = numHits; }

    int rollToHit(int modifier, Rerolls rerolls, int extraAttacks, HitModifier hitModifier) const;
    int rollToWound(int numHits, int modifier, Rerolls rerolls) const;

    const std::string& name() const { return m_name; }
    int range() const { return m_range; }
    int attacks() const { return m_attacks; }
    int toHit() const { return m_toHit; }
    int toWound() const { return m_toWound; }
    int rend() const { return m_rend; }
    int damage() const;

protected:

    int numAttacks(int extraAttacks) const;
    int numTotalHits() const;
    int rollSpecial(int number) const;

private:
    std::string m_name;
    int m_range = 1;
    int m_attacks = 1;
    int m_toHit = 4;
    int m_toWound = 4;
    int m_rend = 0;
    int m_damage = 1;

    int m_hitsPerAttack = 1;
};

#endif //WARHAMMERSIM_WEAPON_H

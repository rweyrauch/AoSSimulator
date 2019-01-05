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

class Weapon
{
public:
    Weapon() = default;
    Weapon(const std::string& name, int range, int attacks, int toHit, int toWound, int rend, int damage);

    int rollToHit() const;
    int rollToWound(int numHits) const;

    int rend() const { return m_rend; }
    int damage() const { return m_damage; }

private:
    std::string m_name;
    int m_range = 1;
    int m_attacks = 1;
    int m_toHit = 4;
    int m_toWound = 4;
    int m_rend = 0;
    int m_damage = 1;
};


#endif //WARHAMMERSIM_WEAPON_H

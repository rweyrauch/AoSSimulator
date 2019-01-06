/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_MODEL_H
#define WARHAMMERSIM_MODEL_H

#include <vector>
#include <Weapon.h>

class Model
{
public:
    Model() : m_baseSize(0) {}
    Model(int baseSize, int wounds) : m_baseSize(baseSize), m_woundsRemaining(wounds) {}

    int basesize() const { return m_baseSize; }

    void position(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
    float x() const { return m_x; }
    float y() const { return m_y; }
    float z() const { return m_z; }

    void addMeleeWeapon(const Weapon& weapon);
    void addMissileWeapon(const Weapon& weapon);

    std::vector<Weapon>::const_iterator meleeWeaponBegin() const { return m_melee.begin(); }
    std::vector<Weapon>::const_iterator meleeWeaponEnd() const { return m_melee.end(); }

    std::vector<Weapon>::const_iterator missileWeaponBegin() const { return m_missile.begin(); }
    std::vector<Weapon>::const_iterator missileWeaponEnd() const { return m_missile.end(); }

    const int& woundsRemaining() const { return m_woundsRemaining; }
    int& woundsRemaining() { return m_woundsRemaining; }

private:
    int m_baseSize = 0;
    float m_x = 0.0f, m_y = 0.0f, m_z = 0.0f;
    int m_woundsRemaining = 0;

    std::vector<Weapon> m_melee;
    std::vector<Weapon> m_missile;
};


#endif //WARHAMMERSIM_MODEL_H

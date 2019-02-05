/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Model.h>
#include <Weapon.h>

void Model::addMeleeWeapon(const Weapon *weapon)
{
    if (weapon == nullptr) return;

    m_melee.push_back(weapon);

    auto preStr = 0.0f;
    if (m_preferredWeapon)
        preStr = m_preferredWeapon->strength();
    if (weapon->strength() > preStr)
        m_preferredWeapon = weapon;
}

void Model::addMissileWeapon(const Weapon *weapon)
{
    if (weapon == nullptr) return;

    m_missile.push_back(weapon);

    auto preStr = 0.0f;
    if (m_preferredWeapon)
        preStr = m_preferredWeapon->strength();
    if (weapon->strength() > preStr)
        m_preferredWeapon = weapon;
}

void Model::restore()
{
    m_woundsRemaining = m_initialWounds;
    m_slain = false;
    m_fled = false;
}

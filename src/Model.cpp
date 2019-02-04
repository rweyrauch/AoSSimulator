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
    m_melee.push_back(weapon);

    m_preferredWeapon = weapon;
}

void Model::addMissileWeapon(const Weapon *weapon)
{
    m_missile.push_back(weapon);

    m_preferredWeapon = weapon;
}

void Model::restore()
{
    m_slain = false;
    m_fled = false;
}

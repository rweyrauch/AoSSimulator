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
    if (weapon == nullptr)
    { return; }
    m_melee.push_back(weapon);

    auto preStr = -1.0f;
    if (m_preferredWeapon)
    {
        preStr = m_preferredWeapon->strength();
    }
    if (weapon->strength() > preStr)
    {
        m_preferredWeapon = weapon;
    }
}

void Model::addMissileWeapon(const Weapon *weapon)
{
    if (weapon == nullptr)
    {
        return;
    }
    m_missile.push_back(weapon);

    auto preStr = -1.0f;
    if (m_preferredWeapon)
    {
        preStr = m_preferredWeapon->strength();
    }
    if (weapon->strength() > preStr)
    {
        m_preferredWeapon = weapon;
    }
}

void Model::restore()
{
    m_woundsRemaining = m_initialWounds;
    m_slain = false;
    m_fled = false;
}

float Model::distanceBetween(const Model& m0, const Model& m1)
{
    float distance = m0.position().distance(m1.position());
    distance -= m0.basesizeInches() / 2.0f;
    distance -= m1.basesizeInches() / 2.0f;
    return std::max(0.0f, distance);
}

void Model::slay()
{
    m_woundsRemaining = 0;
    m_slain = true;
}

int Model::applyWound(int wounds)
{
    m_woundsRemaining -= wounds;
    if (m_woundsRemaining <= 0)
    {
        m_woundsRemaining = 0;
        m_slain = true;
    }
    return m_woundsRemaining;
}

int Model::applyHealing(int wounds)
{
    m_woundsRemaining += wounds;
    if (m_woundsRemaining > m_initialWounds)
    {
        m_woundsRemaining = m_initialWounds;
    }
    return m_woundsRemaining;
}

bool Model::hasWeapon(const std::string &name) const
{
    for (auto ip : m_melee)
    {
        if (ip->name() == name)
            return true;
    }
    for (auto ip : m_missile)
    {
        if (ip->name() == name)
            return true;
    }
    return false;
}

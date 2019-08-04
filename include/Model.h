/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <MathUtils.h>
#include <Weapon.h>

class Model
{
public:
    Model() : m_baseSize_mm(0) {}
    Model(int baseSize, int wounds) :
        m_baseSize_mm(baseSize),
        m_initialWounds(wounds),
        m_woundsRemaining(wounds) {}

    int basesize() const { return m_baseSize_mm; }
    float basesizeInches() const { return m_baseSize_mm / 25.4f; }

    void setPosition(const Math::Point3& pos) { m_position = pos; }
    const Math::Point3& position() const { return m_position; }
    float x() const { return m_position.x; }
    float y() const { return m_position.y; }
    float z() const { return m_position.z; }

    void addMeleeWeapon(const Weapon* weapon);
    void addMissileWeapon(const Weapon* weapon);

    int numMeleeWeapons() const { return (int)m_melee.size(); }
    std::vector<const Weapon*>::const_iterator meleeWeaponBegin() const { return m_melee.begin(); }
    std::vector<const Weapon*>::const_iterator meleeWeaponEnd() const { return m_melee.end(); }

    int numMissileWeapons() const { return (int)m_missile.size(); }
    std::vector<const Weapon*>::const_iterator missileWeaponBegin() const { return m_missile.begin(); }
    std::vector<const Weapon*>::const_iterator missileWeaponEnd() const { return m_missile.end(); }

    bool hasWeapon(const std::string& name) const;

    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }

    int initialWounds() const { return m_initialWounds; }
    int woundsRemaining() const { return m_woundsRemaining; }
    int applyWound(int wounds);
    int applyHealing(int wounds);

    void slay();
    bool slain() const { return m_slain; }
    void flee() { m_fled = true; }
    bool fled() const { return m_fled; }

    void restore();

    const Weapon* preferredWeapon() const { return m_preferredWeapon; }

    static float distanceBetween(const Model& m0, const Model& m1);

private:
    int m_baseSize_mm = 0;
    Math::Point3 m_position = {0.0f, 0.0f, 0.0f};
    int m_initialWounds = 0;
    int m_woundsRemaining = 0;
    bool m_slain = false;
    bool m_fled = false;

    std::vector<const Weapon*> m_melee;
    std::vector<const Weapon*> m_missile;

    std::string m_name;

    const Weapon* m_preferredWeapon = nullptr;
};


#endif //MODEL_H

/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <memory>
#include <vector>
#include <MathUtils.h>
#include <Weapon.h>

class Model {
public:

    //
    // Common model names/roles.
    //
    static constexpr const char* StandardBearer = "Standard Bearer";
    static constexpr const char* BannerBearer = "Banner Bearer";
    static constexpr const char* IconBearer = "Icon Bearer";
    static constexpr const char* TotemBearer = "Totem Bearer";
    static constexpr const char* FlagBearer = "Flag Bearer";

    static constexpr const char* Hornblower = "Hornblower";
    static constexpr const char* Drummer = "Drummer";
    static constexpr const char* Musician = "Musician";
    static constexpr const char* Piper = "Piper";
    static constexpr const char* Trumpeter = "Trumpeter";

    Model() = delete;

    Model(int baseSize, int wounds) :
            m_baseSizeMm(baseSize),
            m_initialWounds(wounds),
            m_woundsRemaining(wounds) {}

    [[nodiscard]] int basesize() const { return m_baseSizeMm; }

    [[nodiscard]] double basesizeInches() const { return (double) m_baseSizeMm / 25.4; }

    void setPosition(const Math::Point3 &pos) { m_position = pos; }

    [[nodiscard]] const Math::Point3 &position() const { return m_position; }

    [[nodiscard]] double x() const { return m_position.x; }

    [[nodiscard]] double y() const { return m_position.y; }

    [[nodiscard]] double z() const { return m_position.z; }

    void setOrientation(const Math::Vector3 &dir) { m_orientation = dir; }

    [[nodiscard]] const Math::Vector3 &orientation() const { return m_orientation; }

    void addMeleeWeapon(const Weapon *weapon);

    void addMissileWeapon(const Weapon *weapon);

    [[nodiscard]] int numMeleeWeapons() const { return (int) m_melee.size(); }

    [[nodiscard]] std::vector<const Weapon *>::const_iterator meleeWeaponBegin() const { return m_melee.begin(); }

    [[nodiscard]] std::vector<const Weapon *>::const_iterator meleeWeaponEnd() const { return m_melee.end(); }

    [[nodiscard]] int numMissileWeapons() const { return (int) m_missile.size(); }

    [[nodiscard]] std::vector<const Weapon *>::const_iterator missileWeaponBegin() const { return m_missile.begin(); }

    [[nodiscard]] std::vector<const Weapon *>::const_iterator missileWeaponEnd() const { return m_missile.end(); }

    [[nodiscard]] bool hasWeapon(const std::string &name) const;

    void setName(const std::string &name) { m_name = name; }

    [[nodiscard]] const std::string &getName() const { return m_name; }

    [[nodiscard]] int initialWounds() const { return m_initialWounds; }

    [[nodiscard]] int woundsRemaining() const { return m_woundsRemaining; }

    int applyWound(int wounds);

    int applyHealing(int wounds);

    void slay();

    [[nodiscard]] bool slain() const { return m_slain; }

    void flee() { m_fled = true; }

    [[nodiscard]] bool fled() const { return m_fled; }

    void restore();

    [[nodiscard]] const Weapon *preferredWeapon() const { return m_preferredWeapon; }

    static double DistanceBetween(const Model *m0, const Model *m1);

private:

    int m_baseSizeMm = 0;
    Math::Point3 m_position = {0, 0, 0};
    Math::Vector3 m_orientation = {1, 0, 0};
    int m_initialWounds = 0;
    int m_woundsRemaining = 0;
    bool m_slain = false;
    bool m_fled = false;

    std::vector<const Weapon *> m_melee;
    std::vector<const Weapon *> m_missile;

    std::string m_name;

    const Weapon *m_preferredWeapon = nullptr;
};


/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THUNDERERS_H
#define THUNDERERS_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Thunderers : public Dispossessed
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    enum WeaponOptions
    {
        DuardinHandgun,
        BraceOfDuardinPistols
    };

    enum StandardOptions
    {
        None,
        RunicIcon,
        ClanBanner
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Thunderers();
    ~Thunderers() override = default;

    bool configure(int numModels, WeaponOptions veteranWeapon, bool duardinBucklers, StandardOptions standard, bool drummers);

protected:

    int rollRunDistance() const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_duardinBucklers = false;
    StandardOptions m_standard = None;
    bool m_drummers = false;

    Weapon m_duardinHandgun,
        m_duardinHandgunVeteran,
        m_braceOfDuardinPistols,
        m_duardinHandgunMelee,
        m_braceOfDuardinPistolsMelee;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Precision Fire                   Yes
// Runic Icon                       TODO
// Clan Banner                      Yes
// Duardin Bucklers                 TODO
// Drummer                          Yes
//

} // namespace Dispossessed

#endif //THUNDERERS_H
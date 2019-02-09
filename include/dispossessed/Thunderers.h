/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THUNDERERS_H
#define THUNDERERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Dispossessed
{

class Thunderers : public Unit
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

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Thunderers();
    ~Thunderers() override = default;

    bool configure(int numModels, WeaponOptions veteranWeapon, bool duardinBucklers, bool standardBearer, bool drummers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    bool m_duardinBucklers = false;
    bool m_standardBearer = false;
    bool m_drummers = false;

    Weapon m_duardinHandgun,
        m_duardinHandgunVeteran,
        m_braceOfDuardinPistols,
        m_duardinHandgunMelee,
        m_braceOfDuardinPistolsMelee;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Precision Fire                   No
// Runic Icon                       No
// Clan Banner                      No
// Duardin Bucklers                 No
//

} // namespace Dispossessed

#endif //THUNDERERS_H
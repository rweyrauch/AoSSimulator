/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARRIORS_H
#define WARRIORS_H

#include <Unit.h>
#include <Weapon.h>

namespace Dispossessed
{

class Warriors : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    enum WeaponOptions
    {
        DuardinAxeOrHammer,
        DoubleHandedDuardinAxe
    };

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Warriors();
    ~Warriors() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool duardinShields, bool standardBearer, bool hornblowers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    bool m_duardinShields = false;
    bool m_standardBearer = false;
    bool m_hornblowers = false;

    Weapon m_duardinAxeOrHammer,
        m_duardinAxeOrHammerVeteran,
        m_doubleHandedAxe,
        m_doubleHandedAxeVeteran;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Resolute in Defence              No
// Duardin Shields                  No
// Runic Icon                       No
// Clan Banner                      No
//

} // namespace Dispossessed

#endif //WARRIORS_H
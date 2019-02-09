/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONBREAKERS_H
#define IRONBREAKERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Dispossessed
{

class Ironbreakers : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    enum WeaponOptions
    {
        IronbreakerAxeOrHammer,
        DrakefirePistolAndCinderblastBomb,
        PairedDrakefirePistols
    };

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Ironbreakers();
    ~Ironbreakers() override = default;

    bool configure(int numModels, WeaponOptions ironbeardWeapons, bool iconBearer, bool drummer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    bool m_iconBearer = false;
    bool m_drummer = false;

    bool m_hasCinderblastBomb = false;

    Weapon m_drakefirePistol,
        m_drakefirePistolMelee,
        m_axeOrHammer,
        m_axeOrHammerIronbeard;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brace of Drakefire Pistols       No
// Cinderblast Bomb                 No
// Gromril Shields                  No
// Forge-proven Gromril Armour      No
//

} // namespace Dispossessed

#endif //IRONBREAKERS_H
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONBREAKERS_H
#define IRONBREAKERS_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Ironbreakers : public Dispossessed
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
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Ironbreakers();
    ~Ironbreakers() override = default;

    bool configure(int numModels, WeaponOptions ironbeardWeapons, bool iconBearer, bool drummer);

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int toSaveModifier(const Weapon *weapon) const override;
    void onStartShooting(PlayerId player) override;
    int rollRunDistance() const override;

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
// Brace of Drakefire Pistols       Yes
// Cinderblast Bomb                 Yes
// Gromril Shields                  Yes
// Forge-proven Gromril Armour      Yes
// Musician                         Yes
// Icon Bearer                      No
//

} // namespace Dispossessed

#endif //IRONBREAKERS_H
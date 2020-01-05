/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONDRAKES_H
#define IRONDRAKES_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Irondrakes : public Dispossessed
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 540;

    enum WeaponOptions
    {
        Drakegun,
        GrudgehammerTorpedo,
        DrakefirePistolAndCinderblastBomb,
        PairedDrakefirePistols
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Irondrakes();
    ~Irondrakes() override = default;

    bool configure(int numModels, WeaponOptions ironWardenWeapons, bool iconBearer, bool hornblower);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toSaveModifier(const Weapon *weapon) const override;
    void onStartShooting(PlayerId player) override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    int rollRunDistance() const override;

private:

    bool m_iconBearer = false;
    bool m_hornblower = false;
    bool m_hasCinderblastBomb = false;

    Weapon m_drakegun,
        m_drakegunWarden,
        m_grudgehammerTorpedo,
        m_drakefirePistol,
        m_drakefirePistolMelee,
        m_mailedFist;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brace of Drakefire Pistols       Yes
// Grudgehammer Torpedo             Yes
// Cinderblast Bomb                 Yes
// Forge-proven Gromril Armour      Yes
// Blaze Away                       Yes
// Hornblowers                      Yes
// Icon Bearer                      No
//

} // namespace Dispossessed

#endif //IRONDRAKES_H
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLREAPERS_H
#define SKULLREAPERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Skullreapers : public Unit
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 170;
    static const int POINTS_MAX_UNIT_SIZE = 680;

    enum WeaponOptions
    {
        GoreSlickBlades,
        Daemonblades
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Skullreapers();
    ~Skullreapers() override = default;

    bool configure(int numModels, WeaponOptions weapons, int numSplinecleaver, int numSoultearer, bool iconBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartCombat(PlayerId player) override;
    void onEndCombat(PlayerId player) override;
    void onSlain() override;

private:

    WeaponOptions m_weapons = GoreSlickBlades;
    bool m_iconBearer = false;

    int m_initialTargetModels = 0;
    int m_numEnemyModelsSlain = 0;

    Weapon m_goreslickBlades,
        m_daemonblades,
        m_spinecleaver,
        m_soultearer,
        m_viciousMutation;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Trial of Skulls                  Yes
// Daemonforged Weapons             Yes
// Frenzied Attacks                 Yes
// Murderous to the Last            Yes
//

} // namespace Khorne

#endif //SKULLREAPERS_H
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ALARIELLE_H
#define ALARIELLE_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Alarielle : public SylvanethBase
{
public:

    static const int BASESIZE = 160;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 600;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Alarielle();
    ~Alarielle() override = default;

    bool configure();
    int move() const override;

protected:

    void onStartHero(PlayerId player) override;
    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    void onWounded() override;
    int getDamageTableIndex() const;
    void onCharged() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_spearOfKurnoth,
        m_talonOfDwindling,
        m_beetleGreatAntlers;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Talon of the Dwindling           Yes
// Lifebloom                        Yes
// Sweeping Blows                   Yes
// Living Battering Ram             Yes
// Soul Amphorae                    Partial
// Metamorphosis                    No
// Ghyran's Wrath                   No
//

} // namespace Sylvaneth

#endif //ALARIELLE_H
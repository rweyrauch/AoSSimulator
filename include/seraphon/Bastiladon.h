/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BASTILADON_H
#define BASTILADON_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Bastiladon : public SeraphonBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    Bastiladon();
    ~Bastiladon() override = default;

    bool configure();

    int save() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_beam,
        m_javelins,
        m_ark,
        m_tail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Light of the Heavens             Yes
// Tide of Snakes                   Yes
//

} // namespace Seraphon

#endif //BASTILADON_H
/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKITTERSTRAND_H
#define SKITTERSTRAND_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class SkitterstrandArachnarok : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 160;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SkitterstrandArachnarok();
    ~SkitterstrandArachnarok() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    int getDamageTableIndex() const;

    Weapon m_chitinousLegs,
        m_monstrousFangs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ambush From Beyond               TODO
// Spider Venom                     Yes
// Wall Crawler                     Yes
//

} // namespace GloomspiteGitz

#endif //SKITTERSTRAND_H
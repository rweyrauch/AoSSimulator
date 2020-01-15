/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ALEGUZZLER_H
#define ALEGUZZLER_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class AleguzzlerGargant : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    AleguzzlerGargant();
    ~AleguzzlerGargant() override = default;

    int move() const override;

    bool configure();

protected:

    int getDamageTableIndex() const;

private:

    Weapon m_eadbutt,
        m_massiveClub,
        m_mightyKick;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drunken Stagger                  No
// Stuff 'Em In Me Bag              No
// Timber!                          No
//

} // namespace GloomspiteGitz

#endif //ALEGUZZLER_H
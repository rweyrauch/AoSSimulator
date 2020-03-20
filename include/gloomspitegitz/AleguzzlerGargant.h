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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    AleguzzlerGargant();
    ~AleguzzlerGargant() override = default;

    bool configure();

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    void onRestore() override;

private:

    Weapon m_eadbutt,
        m_massiveClub,
        m_mightyKick;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Drunken Stagger                  TODO
// Stuff 'Em In Me Bag              TODO
// Timber!                          TODO
//

} // namespace GloomspiteGitz

#endif //ALEGUZZLER_H
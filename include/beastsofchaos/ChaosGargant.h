/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSGARGANT_H
#define CHAOSGARGANT_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class ChaosGargant : public BeastsOfChaosBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ChaosGargant();
    ~ChaosGargant() override = default;

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
// Whipped into Frenzy              TODO
// Drunken Stagger                  TODO
// Stuff 'Em In Me Bag              TODO
// Timber!                          TODO
//

} // namespace GloomspiteGitz

#endif //CHAOSGARGANT_H